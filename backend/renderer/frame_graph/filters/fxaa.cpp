#include <backend/renderer/frame_graph/filters/fxaa.hpp>

#include <backend/renderer/vulkan/pipeline_layout.hpp>
#include <backend/renderer/vulkan/compute_pipeline.hpp>
#include <backend/renderer/vulkan/device.hpp>

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