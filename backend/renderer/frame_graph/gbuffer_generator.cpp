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

#include <pbrlib/scene/scene.hpp>

#include <pbrlib/math/matrix4x4.hpp>

#include <pbrlib/event_system.hpp>
#include <backend/events.hpp>

#include <array>

namespace pbrlib::backend
{
    struct GBufferDescriptorSetBindings
    {
        enum 
        {
            ePosUv,
            eNormalTangent,
            eMaterialIndices,
            eDepthBuffer
        };
    };
}

namespace pbrlib::backend
{
    GBufferGenerator::GBufferGenerator(vk::Device& device) :
        RenderPass(device)
    {
        createResultDescriptorSet();
    }

    void GBufferGenerator::initResultDescriptorSet()
    {
        _sampler_handle = device().createNearestSampler();

        const auto ptr_pos_uv_image         = colorOutputAttach(AttachmentsTraits<GBufferGenerator>::pos_uv);
        const auto ptr_normal_tangent_image = colorOutputAttach(AttachmentsTraits<GBufferGenerator>::normal_tangent);
        const auto ptr_material_index_image = colorOutputAttach(AttachmentsTraits<GBufferGenerator>::material_index);

        constexpr auto expected_image_layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        device().writeDescriptorSet ({
            .view_handle            = ptr_pos_uv_image->view_handle,
            .sampler_handle         = _sampler_handle,
            .set_handle             = _result_descriptor_set_handle,
            .expected_image_layout  = expected_image_layout,
            .binding                = GBufferDescriptorSetBindings::ePosUv
        });

        device().writeDescriptorSet ({
            .view_handle            = ptr_normal_tangent_image->view_handle,
            .sampler_handle         = _sampler_handle,
            .set_handle             = _result_descriptor_set_handle,
            .expected_image_layout  = expected_image_layout,
            .binding                = GBufferDescriptorSetBindings::eNormalTangent
        });
        
        device().writeDescriptorSet ({
            .view_handle            = ptr_material_index_image->view_handle,
            .sampler_handle         = _sampler_handle,
            .set_handle             = _result_descriptor_set_handle,
            .expected_image_layout  = expected_image_layout,
            .binding                = GBufferDescriptorSetBindings::eMaterialIndices
        });
        
        device().writeDescriptorSet ({
            .view_handle            = depthStencil()->view_handle,
            .sampler_handle         = _sampler_handle,
            .set_handle             = _result_descriptor_set_handle,
            .expected_image_layout  = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL,
            .binding                = GBufferDescriptorSetBindings::eDepthBuffer
        });
    }

    bool GBufferGenerator::init(const RenderContext& context, uint32_t width, uint32_t height)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        if (!RenderPass::init(context, width, height)) [[unlikely]]
        {
            log::error("[gbuffer-generator] failed initialize");
            return false;
        }

        EventSystem::on([this] ([[maybe_unused]] const events::RecompilePipeline& event)
        {
            createPipeline();
        });

        const auto ptr_image = colorOutputAttach(AttachmentsTraits<GBufferGenerator>::pos_uv);

        createRenderPass();

        constexpr VkPushConstantRange push_constant_range =
        {
            .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
            .offset     = 0,
            .size       = sizeof(GBufferPushConstantBlock)
        };

        const auto [_, mesh_manager_set_layout] = context.ptr_mesh_manager->descriptorSet();

        _pipeline_layout_handle = vk::builders::PipelineLayout(device())
            .pushConstant(push_constant_range)
            .addSetLayout(mesh_manager_set_layout)
            .build();

        createFramebuffer();
        initResultDescriptorSet();

        return createPipeline();
    }

    bool GBufferGenerator::createPipeline()
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        constexpr auto vert_shader = "shaders/gbuffer_generator/gbuffer_generator.glsl.vert";
        constexpr auto frag_shader = "shaders/gbuffer_generator/gbuffer_generator.glsl.frag";

        auto new_pipeline = vk::builders::GraphicsPipeline(device())
            .addStage(vert_shader, VK_SHADER_STAGE_VERTEX_BIT)
            .addStage(frag_shader, VK_SHADER_STAGE_FRAGMENT_BIT)
            .addAttachmentsState(false)
            .addAttachmentsState(false)
            .addAttachmentsState(false)
            .depthStencilTest(true)
            .pipelineLayoutHandle(_pipeline_layout_handle)
            .renderPassHandle(_render_pass_handle)
            .subpass(0)
            .build();

        _pipeline_handle = std::move(new_pipeline);

        return true;
    }
}

namespace pbrlib::backend
{
    void GBufferGenerator::createRenderPass()
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        const auto* ptr_pos_uv_attach   = colorOutputAttach(AttachmentsTraits<GBufferGenerator>::pos_uv);
        const auto* ptr_nor_tan_attach  = colorOutputAttach(AttachmentsTraits<GBufferGenerator>::normal_tangent);
        const auto* ptr_mat_idx_attach  = colorOutputAttach(AttachmentsTraits<GBufferGenerator>::material_index);

        _render_pass_handle = vk::builders::RenderPass(device())
            .addColorAttachment(ptr_pos_uv_attach, _final_attachments_layout)
            .addColorAttachment(ptr_nor_tan_attach, _final_attachments_layout)
            .addColorAttachment(ptr_mat_idx_attach, _final_attachments_layout)
            .depthAttachment(depthStencil(), VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL)
            .build();
    }

    void GBufferGenerator::createFramebuffer()
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        const auto ptr_pos_uv_attach    = colorOutputAttach(AttachmentsTraits<GBufferGenerator>::pos_uv);
        const auto ptr_nor_tan_attach   = colorOutputAttach(AttachmentsTraits<GBufferGenerator>::normal_tangent);
        const auto ptr_mat_idx_attach   = colorOutputAttach(AttachmentsTraits<GBufferGenerator>::material_index);

        const auto [width, height] = size();

        _framebuffer_handle = vk::builders::Framebuffer(device())
            .size(width, height)
            .layers(1)
            .renderPass(_render_pass_handle)
            .addAttachment(*ptr_pos_uv_attach)
            .addAttachment(*ptr_nor_tan_attach)
            .addAttachment(*ptr_mat_idx_attach)
            .addAttachment(*depthStencil())
            .build();
    }
}

namespace pbrlib::backend
{
    void GBufferGenerator::setupColorAttachmentsLayout()
    {
        colorOutputAttach(AttachmentsTraits<GBufferGenerator>::pos_uv)->changeLayout(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
        colorOutputAttach(AttachmentsTraits<GBufferGenerator>::normal_tangent)->changeLayout(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
        colorOutputAttach(AttachmentsTraits<GBufferGenerator>::material_index)->changeLayout(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
    }

    void GBufferGenerator::beginPass(vk::CommandBuffer& command_buffer)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        setupColorAttachmentsLayout();

        _push_constant_block.projection_view = context().projection * context().view;

        command_buffer.write([this] (VkCommandBuffer command_buffer_handle)
        {
            PBRLIB_PROFILING_VK_ZONE_SCOPED(device(), command_buffer_handle, "[gbuffer-generator] pre-pass");

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

            const auto [width, height] = size();

            const VkRect2D area 
            {
                0, 
                0, 
                width, 
                height 
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
                .width      = static_cast<float>(width),
                .height     = static_cast<float>(height),
                .minDepth   = 0.0f,
                .maxDepth   = 1.0
            };

            const auto [descriptor_set, _] = context().ptr_mesh_manager->descriptorSet();

            vkCmdBeginRenderPass2(command_buffer_handle, &render_pass_begin_info, &subpass_begin_info);
            vkCmdBindPipeline(command_buffer_handle, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipeline_handle);
            vkCmdBindDescriptorSets(command_buffer_handle, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipeline_layout_handle, 0, 1, &descriptor_set, 0, nullptr);
            vkCmdSetViewport(command_buffer_handle, 0, 1, &viewport);
            vkCmdSetScissor(command_buffer_handle, 0, 1, &area);
        }, "[gbuffer-generator] begin-pass", vk::marker_colors::graphics_pipeline);
    }

    void GBufferGenerator::render(vk::CommandBuffer& command_buffer)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        beginPass(command_buffer);

        for (const auto ptr_item: context().items)
        {
            const auto& tag = ptr_item->getComponent<pbrlib::components::Tag>();
    
            command_buffer.write([this, ptr_item] (VkCommandBuffer command_buffer_handle)
            {
                PBRLIB_PROFILING_VK_ZONE_SCOPED(device(), command_buffer_handle, "[gbuffer-generator] run-pipeline");
    
                const auto& renderable = ptr_item->getComponent<components::Renderable>();
    
                _push_constant_block.instance_id    = renderable.instance_id;
                _push_constant_block.material_index = renderable.material_id;
    
                vkCmdPushConstants (
                    command_buffer_handle, 
                    _pipeline_layout_handle, 
                    VK_SHADER_STAGE_VERTEX_BIT, 
                    0, sizeof(GBufferPushConstantBlock), &_push_constant_block
                );
    
                const auto& index_buffer = context().ptr_mesh_manager->indexBuffer(renderable.instance_id);
    
                vkCmdBindIndexBuffer(command_buffer_handle, index_buffer.handle, 0, VK_INDEX_TYPE_UINT32);
                vkCmdDrawIndexed(command_buffer_handle, static_cast<uint32_t>(renderable.index_count), 1, 0, 0, 0);
            }, std::format("[gbuffer-pass] run-pipeline: {}", tag.name), vk::marker_colors::graphics_pipeline);
        }

        endPass(command_buffer);
    }

    void GBufferGenerator::endPass(vk::CommandBuffer& command_buffer)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        command_buffer.write([this] (VkCommandBuffer command_buffer_handle)
        {
            PBRLIB_PROFILING_VK_ZONE_SCOPED(device(), command_buffer_handle, "[gbuffer-generator] post-pass");

            vkCmdEndRenderPass(command_buffer_handle);
        }, "[gbuffer-generator] end-pass", vk::marker_colors::graphics_pipeline);

        colorOutputAttach(AttachmentsTraits<GBufferGenerator>::pos_uv)->layout          = _final_attachments_layout;
        colorOutputAttach(AttachmentsTraits<GBufferGenerator>::normal_tangent)->layout  = _final_attachments_layout;
        colorOutputAttach(AttachmentsTraits<GBufferGenerator>::material_index)->layout  = _final_attachments_layout;
    }

    VkPipelineStageFlags2 GBufferGenerator::srcStage() const noexcept
    {
        return VK_PIPELINE_STAGE_2_VERTEX_SHADER_BIT;
    }

    VkPipelineStageFlags2 GBufferGenerator::dstStage() const noexcept
    {
        return VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT;
    }

    void GBufferGenerator::createResultDescriptorSet()
    {
        _result_descriptor_set_layout_handle = vk::builders::DescriptorSetLayout(device())
            .addBinding(GBufferDescriptorSetBindings::ePosUv, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_COMPUTE_BIT)
            .addBinding(GBufferDescriptorSetBindings::eNormalTangent, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_COMPUTE_BIT)
            .addBinding(GBufferDescriptorSetBindings::eMaterialIndices, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_COMPUTE_BIT)
            .addBinding(GBufferDescriptorSetBindings::eDepthBuffer, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_COMPUTE_BIT)
            .build();

        _result_descriptor_set_handle = device().allocateDescriptorSet (
            _result_descriptor_set_layout_handle, 
            "[gbuffer-generator] descritor set with results"
        );
    }

    auto GBufferGenerator::resultDescriptorSet() const noexcept
        -> std::pair<VkDescriptorSet, VkDescriptorSetLayout>
    {
        return std::make_pair (
            _result_descriptor_set_handle.handle(), 
            _result_descriptor_set_layout_handle.handle()
        );
    }
}
