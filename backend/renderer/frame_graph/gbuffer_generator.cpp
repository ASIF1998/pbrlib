#include <backend/renderer/frame_graph/gbuffer_generator.hpp>

#include <backend/renderer/vulkan/render_pass.hpp>
#include <backend/renderer/vulkan/shader_compiler.hpp>
#include <backend/renderer/vulkan/device.hpp>
#include <backend/renderer/vulkan/gpu_marker_colors.hpp>
#include <backend/renderer/vulkan/buffer.hpp>
#include <backend/renderer/vulkan/framebuffer.hpp>
#include <backend/renderer/vulkan/graphics_pipeline.hpp>
#include <backend/utils/vulkan.hpp>

#include <backend/scene/mesh_manager.hpp>

#include <backend/components.hpp>

#include <backend/utils/paths.hpp>

#include <backend/logger/logger.hpp>

#include <backend/profiling.hpp>

#include <pbrlib/scene/scene.hpp>

#include <pbrlib/math/matrix4x4.hpp>

#include <array>

namespace pbrlib::backend
{
    GBufferGenerator::~GBufferGenerator()
    {
        const auto device_handle = _ptr_device->device();

        vkDestroyFramebuffer(device_handle, _framebuffer_handle, nullptr);

        vkDestroyRenderPass(device_handle, _render_pass_handle, nullptr);
        vkDestroyPipeline(device_handle, _pipeline_handle, nullptr);
    }

    bool GBufferGenerator::init(vk::Device& device, const RenderContext& context)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        if (!RenderPass::init(device, context))
        {
            log::error("[gbuffer-generator] failed initialize");
            return false;
        }

        const auto ptr_image = colorOutputAttach(GBufferFinalAttachmentsName::pos_uv);

        /// @todo думаю это тоже стоит перенсти в RenderPass и инициализировать там
        _width  = ptr_image->width;
        _height = ptr_image->height;

        createRenderPass();

        constexpr VkPushConstantRange push_constant_range =
        {
            .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
            .offset     = 0,
            .size       = sizeof(GBufferPushConstantBlock)
        };

        _pipeline_layout = vk::PipelineLayout::Builder(*_ptr_device)
            .pushConstant(push_constant_range)
            .addSet()
                .addBinding(0, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT)
                .addBinding(1, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT)
            .build();

        createFramebuffer();

        return rebuild(device, context);
    }

    bool GBufferGenerator::rebuild(vk::Device& device, const RenderContext& context)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        constexpr auto vert_shader = "shaders/gbuffer_generator/gbuffer_generator.glsl.vert";
        constexpr auto frag_shader = "shaders/gbuffer_generator/gbuffer_generator.glsl.frag";

        auto prev_pipeline = _pipeline_handle;

        try
        {
            _pipeline_handle = vk::GraphicsPipelineBuilder(*_ptr_device)
                .addStage(vert_shader, VK_SHADER_STAGE_VERTEX_BIT)
                .addStage(frag_shader, VK_SHADER_STAGE_FRAGMENT_BIT)
                .addAttachmentsState(false)
                .addAttachmentsState(false)
                .addAttachmentsState(false)
                .depthStencilTest(true)
                .pipelineLayoutHandle(_pipeline_layout->handle)
                .renderPassHandle(_render_pass_handle)
                .subpass(0)
                .build();

            vkDestroyPipeline(_ptr_device->device(), prev_pipeline, nullptr);
        } 
        catch (std::exception& ex)
        {
            backend::log::error("[gbuffer-generate] failed rebuild: {}", ex.what());
        }

        return true;
    }
}

namespace pbrlib::backend
{
    void GBufferGenerator::createRenderPass()
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        const auto* ptr_pos_uv_attach   = colorOutputAttach(GBufferFinalAttachmentsName::pos_uv);
        const auto* ptr_nor_tan_attach  = colorOutputAttach(GBufferFinalAttachmentsName::normal_tangent);
        const auto* ptr_mat_idx_attach  = colorOutputAttach(GBufferFinalAttachmentsName::material_index);

        _render_pass_handle = vk::RenderPassBuilder(*_ptr_device)
            .addColorAttachment(ptr_pos_uv_attach, final_attachments_layout)
            .addColorAttachment(ptr_nor_tan_attach, final_attachments_layout)
            .addColorAttachment(ptr_mat_idx_attach, final_attachments_layout)
            .depthAttachment(_ptr_depth_stencil_image, VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL)
            .build();
    }

    void GBufferGenerator::createFramebuffer()
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        const auto ptr_pos_uv_attach    = colorOutputAttach(GBufferFinalAttachmentsName::pos_uv);
        const auto ptr_nor_tan_attach   = colorOutputAttach(GBufferFinalAttachmentsName::normal_tangent);
        const auto ptr_mat_idx_attach   = colorOutputAttach(GBufferFinalAttachmentsName::material_index);

        _framebuffer_handle = vk::FramebufferBuild(*_ptr_device)
            .size(_width, _height)
            .layers(1)
            .renderPass(_render_pass_handle)
            .addAttachment(*ptr_pos_uv_attach)
            .addAttachment(*ptr_nor_tan_attach)
            .addAttachment(*ptr_mat_idx_attach)
            .addAttachment(*_ptr_depth_stencil_image)
            .build();
    }
}

namespace pbrlib::backend
{
    void GBufferGenerator::setupColorAttachmentsLayout()
    {
        colorOutputAttach(GBufferFinalAttachmentsName::pos_uv)->changeLayout(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
        colorOutputAttach(GBufferFinalAttachmentsName::normal_tangent)->changeLayout(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
        colorOutputAttach(GBufferFinalAttachmentsName::material_index)->changeLayout(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
    }

    void GBufferGenerator::prePass(vk::CommandBuffer& command_buffer)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        RenderPass::prePass(command_buffer);

        setupColorAttachmentsLayout();

        _push_constant_block.projection_view = _ptr_context->projection * _ptr_context->view; 

        command_buffer.write([this] (VkCommandBuffer command_buffer_handle)
        {
            PBRLIB_PROFILING_VK_ZONE_SCOPED(*_ptr_device, command_buffer_handle, "[gbuffer-generator] pre-pass");

            constexpr VkClearValue pos_clear_value
            {
                .color
                {
                    .float32 = {0.0f, 0.0f, 0.0f, 1.0f}
                }
            };
            
            constexpr VkClearValue nor_tan_clear_value
            {
                .color
                {
                    .float32 = {0.0f, 0.0f, 0.0f, 1.0f}
                }
            };
            
            constexpr VkClearValue material_index_clear_value
            {
                .color
                {
                    .float32 = {0.0f, 0.0f, 0.0f, 0.0f}
                }
            };

            constexpr VkClearValue depth_clear_value
            {
                .depthStencil = {1.0f, 0}
            };

            constexpr std::array clear_values 
            {
                pos_clear_value, 
                nor_tan_clear_value,
                material_index_clear_value,
                depth_clear_value
            };

            const VkRect2D area 
            {
                0, 
                0, 
                _width, 
                _height 
            };

            const VkRenderPassBeginInfo render_pass_begin_info
            {
                .sType              = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
                .renderPass         = _render_pass_handle,
                .framebuffer        = _framebuffer_handle,
                .renderArea         = area,
                .clearValueCount    = static_cast<uint32_t>(clear_values.size()),
                .pClearValues       = clear_values.data()
            };

            constexpr VkSubpassBeginInfo subpass_begin_info
            {
                .sType      = VK_STRUCTURE_TYPE_SUBPASS_BEGIN_INFO,
                .contents   = VK_SUBPASS_CONTENTS_INLINE
            };

            const VkViewport viewport
            {
                .width      = static_cast<float>(_width),
                .height     = static_cast<float>(_height),
                .minDepth   = 0.0f,
                .maxDepth   = 1.0
            };

            const auto descriptor_set = _ptr_context->ptr_mesh_manager->descriptorSet();

            vkCmdBeginRenderPass2(command_buffer_handle, &render_pass_begin_info, &subpass_begin_info);
            vkCmdBindPipeline(command_buffer_handle, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipeline_handle);
            vkCmdBindDescriptorSets(command_buffer_handle, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipeline_layout->handle, 0, 1, &descriptor_set, 0, nullptr);
            vkCmdSetViewport(command_buffer_handle, 0, 1, &viewport);
            vkCmdSetScissor(command_buffer_handle, 0, 1, &area);
        }, "[gbuffer-generator] pre-pass", vk::marker_colors::graphics_pipeline);
    }

    void GBufferGenerator::render(size_t item_id, vk::CommandBuffer& command_buffer)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        const auto ptr_item = _ptr_context->items[item_id];

        const auto& tag = ptr_item->getComponent<pbrlib::component::Tag>();

        command_buffer.write([this, item_id, ptr_item] (VkCommandBuffer command_buffer_handle)
        {
            PBRLIB_PROFILING_VK_ZONE_SCOPED(*_ptr_device, command_buffer_handle, "[gbuffer-generator] run-pipeline");

            const auto& renderable = ptr_item->getComponent<component::Renderable>();

            _push_constant_block.instance_id    = renderable.instance_id;
            _push_constant_block.material_index = renderable.material_id;

            vkCmdPushConstants(
                command_buffer_handle, 
                _pipeline_layout->handle, 
                VK_SHADER_STAGE_VERTEX_BIT, 
                0, sizeof(GBufferPushConstantBlock), &_push_constant_block
            );

            const auto& index_buffer = _ptr_context->ptr_mesh_manager->indexBuffer(renderable.instance_id);

            vkCmdBindIndexBuffer(command_buffer_handle, index_buffer.handle, 0, VK_INDEX_TYPE_UINT32);
            vkCmdDrawIndexed(command_buffer_handle, static_cast<uint32_t>(renderable.index_count), 1, 0, 0, 0);
        }, std::format("[gbuffer-pass] run-pipeline: {}", tag.name), vk::marker_colors::graphics_pipeline);
    }

    void GBufferGenerator::postPass(vk::CommandBuffer& command_buffer)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        RenderPass::postPass(command_buffer);

        command_buffer.write([this] (VkCommandBuffer command_buffer_handle)
        {
            PBRLIB_PROFILING_VK_ZONE_SCOPED(*_ptr_device, command_buffer_handle, "[gbuffer-generator] post-pass");

            vkCmdEndRenderPass(command_buffer_handle);
        }, "[gbuffer-generator] post-pass", vk::marker_colors::graphics_pipeline);

        colorOutputAttach(GBufferFinalAttachmentsName::pos_uv)->layout          = final_attachments_layout;
        colorOutputAttach(GBufferFinalAttachmentsName::normal_tangent)->layout  = final_attachments_layout;
        colorOutputAttach(GBufferFinalAttachmentsName::material_index)->layout  = final_attachments_layout;
    }
}
