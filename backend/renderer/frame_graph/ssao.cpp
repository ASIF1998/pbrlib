#include <backend/renderer/frame_graph/ssao.hpp>

#include <backend/profiling.hpp>

#include <backend/logger/logger.hpp>

#include <backend/renderer/vulkan/device.hpp>
#include <backend/renderer/vulkan/render_pass.hpp>
#include <backend/renderer/vulkan/compute_pipeline.hpp>

namespace pbrlib::backend
{
    SSAO::~SSAO()
    {
        const auto device_handle = _ptr_device->device();

        vkDestroyPipeline(device_handle, _pipeline_handle, nullptr);
        vkFreeDescriptorSets(device_handle, _ptr_device->descriptorPool(), 1, &_descriptor_set_handle);
    }

    bool SSAO::init(vk::Device& device, const RenderContext& context)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        if (!RenderPass::init(device, context))
        {
            log::error("[ssao] failed initialize");
            return false;
        }

        const auto* ptr_result_attachment = colorOutputAttach(SSAOOutputAttachmentsNames::result);

        /// @todo
        // _pipeline_layout = vk::builders::PipelineLayout(*_ptr_device)
        //     .addSet()
        //         .addBinding(0, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1, VK_SHADER_STAGE_COMPUTE_BIT)
        //         .addBinding(1, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1, VK_SHADER_STAGE_COMPUTE_BIT)
        //     .build();

        /// @todo
        // _descriptor_set_handle = _ptr_device->allocateDescriptorSet(
        //     _pipeline_layout->sets_layout[0], 
        //     "[ssao] gbuffer-geometry"
        // );

        // const auto ptr_gbuffer_pos_uv           = colorInputAttach(SSAOInputAttachmentNames::pos_uv);
        // const auto ptr_gbuffer_normal_tangent   = colorInputAttach(SSAOInputAttachmentNames::normal_tangent);

        // const VkDescriptorImageInfo gbuffer_pos_uv_image_info 
        // {
        //     .imageView      = ptr_gbuffer_pos_uv->view_handle,
        //     .imageLayout    = ptr_gbuffer_pos_uv->layout
        // };

        // const VkWriteDescriptorSet write_gbuffer_pos_uv_image 
        // {
        //     .sType              = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        //     .dstSet             = _descriptor_set_handle,
        //     .dstBinding         = 0,
        //     .descriptorCount    = 1,
        //     .descriptorType     = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
        //     .pImageInfo         = &gbuffer_pos_uv_image_info
        // };

        // const VkDescriptorImageInfo gbuffer_normal_tangent_image_info 
        // {
        //     .imageView      = ptr_gbuffer_pos_uv->view_handle,
        //     .imageLayout    = ptr_gbuffer_pos_uv->layout
        // };

        // const VkWriteDescriptorSet write_gbuffer_normal_tangent_image 
        // {
        //     .sType              = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        //     .dstSet             = _descriptor_set_handle,
        //     .dstBinding         = 1,
        //     .descriptorCount    = 1,
        //     .descriptorType     = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
        //     .pImageInfo         = &gbuffer_normal_tangent_image_info
        // };

        // vkUpdateDescriptorSets(
        //     _ptr_device->device(),
        //     1, &write_gbuffer_pos_uv_image,
        //     0, nullptr
        // );
        
        // vkUpdateDescriptorSets(
        //     _ptr_device->device(),
        //     1, &write_gbuffer_normal_tangent_image,
        //     0, nullptr
        // );

        // return rebuild();
        return true;
    }

    bool SSAO::rebuild()
    {
        constexpr auto ssao_shader = "shaders/ssao/ssao.glsl.comp";

        auto prev_handle = _pipeline_handle;

        _pipeline_handle = vk::builders::ComputePipeline(*_ptr_device)
            .shader(ssao_shader)
            .pipelineLayoutHandle(_pipeline_layout->handle)
            .build();

        return true;
    }

    void SSAO::render(vk::CommandBuffer& command_buffer)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;
    }

    VkPipelineStageFlags2 SSAO::srcStage() const noexcept
    {
        return VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT;
    }

    VkPipelineStageFlags2 SSAO::dstStage() const noexcept
    {
        return VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT;
    }
}