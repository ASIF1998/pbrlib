#include <backend/renderer/frame_graph/physically_based_lighting.hpp>
#include <backend/events.hpp>

#include <backend/renderer/vulkan/device.hpp>
#include <backend/renderer/vulkan/compute_pipeline.hpp>
#include <backend/renderer/vulkan/pipeline_layout.hpp>
#include <backend/renderer/vulkan/command_buffer.hpp>
#include <backend/renderer/vulkan/gpu_marker_colors.hpp>

#include <backend/profiling.hpp>

#include <backend/logger/logger.hpp>

namespace pbrlib::backend
{
    PhysicallyBasedLighting::PhysicallyBasedLighting(vk::Device& device, vk::Image& dst_image) noexcept :
        RenderPass(device)
    { }

    bool PhysicallyBasedLighting::init(const RenderContext& context, uint32_t width, uint32_t height)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        if (!RenderPass::init(context, width, height))
        {
            log::error("[pbl] failed initialize");
            return false;
        }

        on([this] (const events::RecompilePipeline& event)
        {
            if (!createPipeline())
                log::error("[pbl] failed rebuild pipeline");
        });

        _pipeline_layout_handle = vk::builders::PipelineLayout(device())
            .build();

        return createPipeline();
    }

    bool PhysicallyBasedLighting::createPipeline()
    {
        auto new_pipeline = vk::builders::ComputePipeline(device())
            .pipelineLayoutHandle(_pipeline_layout_handle)
            .shader("shaders/physically_based_lighting.comp.glsl")
            .build();

        _pipeline_handle = std::move(new_pipeline);

        return true;
    }

    void PhysicallyBasedLighting::render(vk::CommandBuffer& command_buffer)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        command_buffer.write([this] (const auto command_buffer_handle)
        {
            PBRLIB_PROFILING_VK_ZONE_SCOPED(device(), command_buffer_handle, "[pbl] run-pipeline");

            /// @todo
        }, "[pbl] run-pipeline", vk::marker_colors::pbl);
    }

    VkPipelineStageFlags2 PhysicallyBasedLighting::srcStage() const noexcept
    {
        return VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT;
    }

    VkPipelineStageFlags2 PhysicallyBasedLighting::dstStage() const noexcept
    {
        return VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT;
    }

    std::pair<VkDescriptorSet, VkDescriptorSetLayout> PhysicallyBasedLighting::resultDescriptorSet() const noexcept
    {
        return std::make_pair(VK_NULL_HANDLE, VK_NULL_HANDLE);
    }
}
