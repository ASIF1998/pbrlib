#include <backend/renderer/frame_graph/filters/fxaa.hpp>

#include <backend/renderer/vulkan/pipeline_layout.hpp>
#include <backend/renderer/vulkan/compute_pipeline.hpp>
#include <backend/renderer/vulkan/device.hpp>
#include <backend/renderer/vulkan/image.hpp>
#include <backend/renderer/vulkan/command_buffer.hpp>
#include <backend/renderer/vulkan/gpu_marker_colors.hpp>
#include <backend/renderer/vulkan/check.hpp>

#include <backend/events.hpp>
#include <pbrlib/event_system.hpp>

#include <backend/profiling.hpp>

#include <backend/logger/logger.hpp>

#include <pbrlib/math/lerp.hpp>

#include  <algorithm>

namespace pbrlib::backend
{
    FXAA::FXAA(vk::Device& device, vk::Image& dst_image) :
        Filter ("fxaa", device, dst_image)
    { }

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

        EventSystem::on([this] (const events::UpdateFXAA& settings)
        {
            const auto& config = settings.settings;

            const auto span_max     = std::clamp(config.span_max, 0.0, 1.0);
            const auto reduce_min   = std::clamp(config.reduce_min, 0.0, 1.0);
            const auto reduce_mul   = std::clamp(config.reduce_mul, 0.0, 1.0);

            _config.span_max    = math::lerp(4.0f, 16.0f, span_max);
            _config.reduce_min  = math::lerp(1.0f / 256.0f, 1.0f / 64.0f, reduce_min);
            _config.reduce_mul  = math::lerp(1.0f / 16.0f, 1.0f / 4.0f, reduce_mul);
        });

        const auto [_, io_set_layout_handle] = IODescriptorSet();

        constexpr VkPushConstantRange push_constant_range =
        {
            .stageFlags = VK_SHADER_STAGE_COMPUTE_BIT,
            .offset     = 0,
            .size       = sizeof(Config)
        };

        _pipeline_layout_handle = vk::builders::PipelineLayout(device())
            .addSetLayout(io_set_layout_handle)
            .pushConstant(push_constant_range)
            .build();

        return createPipeline();
    }

    bool FXAA::createPipeline()
    {
        auto new_pipeline = vk::builders::ComputePipeline(device())
            .pipelineLayoutHandle(_pipeline_layout_handle)
            .shader("shaders/fxaa.glsl.comp")
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

            const auto [io_set_handle, _] = IODescriptorSet();
            vkCmdBindDescriptorSets(
                command_buffer_handle,
                VK_PIPELINE_BIND_POINT_COMPUTE,
                _pipeline_layout_handle,
                0, 1, &io_set_handle,
                0, nullptr
            );

            vkCmdPushConstants (
                command_buffer_handle,
                _pipeline_layout_handle,
                VK_SHADER_STAGE_COMPUTE_BIT,
                0, static_cast<uint32_t>(sizeof(Config)), &_config
            );

            dispatchCompute(command_buffer_handle);
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
