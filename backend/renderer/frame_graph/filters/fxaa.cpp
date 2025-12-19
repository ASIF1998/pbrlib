#include <backend/renderer/frame_graph/filters/fxaa.hpp>

#include <backend/renderer/vulkan/pipeline_layout.hpp>
#include <backend/renderer/vulkan/compute_pipeline.hpp>
#include <backend/renderer/vulkan/device.hpp>
#include <backend/renderer/vulkan/image.hpp>
#include <backend/renderer/vulkan/command_buffer.hpp>
#include <backend/renderer/vulkan/gpu_marker_colors.hpp>
#include <backend/utils/vulkan.hpp>

#include <backend/events.hpp>
#include <pbrlib/event_system.hpp>

#include <backend/profiling.hpp>

#include <backend/logger/logger.hpp>

namespace pbrlib::backend
{
    FXAA::FXAA(vk::Device& device) :
        Filter (device)
    { 
        _descriptor_set_layout_handle = vk::builders::DescriptorSetLayout(device)
            .addBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_COMPUTE_BIT)
            .addBinding(1, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1, VK_SHADER_STAGE_COMPUTE_BIT)
            .build();

        _descriptor_set_handle = device.allocateDescriptorSet (
            _descriptor_set_layout_handle,
            "[fxaa] input descriptor set"
        );
    }

    bool FXAA::init(const RenderContext& context, uint32_t width, uint32_t height)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        if (!RenderPass::init(context, width, height)) [[unlikely]]
        {
            log::error("[fxaa] failed initialize");
            return false;
        }

        EventSystem::on([this] ([[maybe_unused]] const events::RecompilePipeline& init) 
        {
            createPipeline();
        });

        _pipeline_layout_handle = vk::builders::PipelineLayout(device())
            .addSetLayout(_descriptor_set_layout_handle)
            .build();

        const auto& input_image         = srcImage();
        const auto  ptr_output_image    = colorOutputAttach(AttachmentsTraits<FXAA>::result);

        constexpr VkSamplerCreateInfo sampler_create_info 
        {
            .sType          = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
            .magFilter      = VK_FILTER_NEAREST,
            .minFilter      = VK_FILTER_NEAREST,
            .addressModeU   = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
            .addressModeV   = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
            .addressModeW   = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
            .borderColor    = VK_BORDER_COLOR_INT_OPAQUE_BLACK
        };

        VK_CHECK(vkCreateSampler(
            device().device(),
            &sampler_create_info,
            nullptr, 
            &_sampler_handle.handle()
        ));

        device().writeDescriptorSet ({
            .view_handle            = input_image.view_handle.handle(),
            .sampler_handle         = _sampler_handle,
            .set_handle             = _descriptor_set_handle,
            .expected_image_layout  = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
            .binding                = 0
        });
        
        device().writeDescriptorSet ({
            .view_handle            = ptr_output_image->view_handle.handle(),
            .set_handle             = _descriptor_set_handle,
            .expected_image_layout  = VK_IMAGE_LAYOUT_GENERAL,
            .binding                = 1
        });

        return createPipeline();
    }

    bool FXAA::createPipeline()
    {
        auto new_pipeline = vk::builders::ComputePipeline(device())
            .pipelineLayoutHandle(_pipeline_layout_handle)
            .shader("shaders/fxaa/fxaa.glsl.comp")
            .build();

        _pipeline_handle = std::move(new_pipeline);

        return true;
    }

    void FXAA::render(vk::CommandBuffer& command_buffer)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        command_buffer.write([this] (VkCommandBuffer command_buffer_handle)
        {
            vkCmdBindPipeline(command_buffer_handle, VK_PIPELINE_BIND_POINT_COMPUTE, _pipeline_handle);

            vkCmdBindDescriptorSets(
                command_buffer_handle, 
                VK_PIPELINE_BIND_POINT_COMPUTE, 
                _pipeline_layout_handle, 
                0, 1, &_descriptor_set_handle.handle(), 
                0, nullptr
            );

            const auto [width, height] = size();

            const auto group_count_x = width / device().workGroupSize();
            const auto group_count_y = height / device().workGroupSize();

            vkCmdDispatch(command_buffer_handle, group_count_x, group_count_y, 1);
        }, "[fxaa] run-pipeline", vk::marker_colors::fxaa);
    }

    VkPipelineStageFlags2 FXAA::srcStage() const noexcept
    {
        return VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT;
    }

    VkPipelineStageFlags2 FXAA::dstStage() const noexcept
    {
        return VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT;
    }

    std::pair<VkDescriptorSet, VkDescriptorSetLayout> FXAA::resultDescriptorSet() const noexcept
    {
        return std::make_pair(VK_NULL_HANDLE, VK_NULL_HANDLE);
    }
}