#include <backend/renderer/frame_graph/physically_based_lighting.hpp>
#include <backend/events.hpp>

#include <backend/renderer/vulkan/device.hpp>
#include <backend/renderer/vulkan/pipeline.hpp>
#include <backend/renderer/vulkan/pipeline_layout.hpp>
#include <backend/renderer/vulkan/command_buffer.hpp>
#include <backend/renderer/vulkan/gpu_marker_colors.hpp>

#include <backend/profiling.hpp>

#include <backend/logger/logger.hpp>
#include <backend/utils/paths.hpp>

namespace pbrlib::backend
{
    PhysicallyBasedLighting::PhysicallyBasedLighting(vk::Device& device, vk::Image& dst_image) noexcept :
        RenderPass(device)
    {
        _result_descriptor_group.emplace(
            device,
            vk::builders::DescriptorSetLayout(device)
                .addBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_COMPUTE_BIT),
            "[pbl] result descriptor set"
        );
    }

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

        auto ptr_gbuffer_set = descriptorGroup(gbuffer_set_id);

        if (!ptr_gbuffer_set) [[unlikely]]
            throw exception::InvalidState("[pbl] failed initialize - gbuffer descriptor group is empty");

        _pipeline_layout_handle = vk::builders::PipelineLayout(device())
            .addSetLayout(ptr_gbuffer_set->descriptorSetLayoutHandle())
            .build();

        return createPipeline();
    }

    bool PhysicallyBasedLighting::createPipeline()
    {
        auto new_pipeline = vk::builders::ComputePipeline(device())
            .pipelineLayoutHandle(_pipeline_layout_handle)
            .shader(PBRLIB_ABS_PATH("backend/shaders/physically_based_lighting.comp.slang"))
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

            vkCmdBindPipeline(command_buffer_handle, VK_PIPELINE_BIND_POINT_COMPUTE, _pipeline_handle);

            const std::array sets_descriptors
            {
                descriptorGroup(gbuffer_set_id)->descriptorSetHandle()
            };

            vkCmdBindDescriptorSets (
                command_buffer_handle,
                VK_PIPELINE_BIND_POINT_COMPUTE,
                _pipeline_layout_handle, 0,
                static_cast<uint32_t>(sets_descriptors.size()), sets_descriptors.data(),
                0, nullptr
            );

            const auto [width, height] = size();

            const auto local_size_x = width / device().workGroupSize();
            const auto local_size_y = height / device().workGroupSize();

            vkCmdDispatch(command_buffer_handle, local_size_x, local_size_y, 1);
        }, "[pbl] run-pipeline", vk::marker_colors::pbl);
    }

    VkPipelineStageFlags2 PhysicallyBasedLighting::srcStage() const noexcept
    {
        return _src_stage;
    }

    VkPipelineStageFlags2 PhysicallyBasedLighting::dstStage() const noexcept
    {
        return VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT;
    }

    vk::DescriptorGroup* PhysicallyBasedLighting::resultDescriptorGroup() noexcept
    {
        return &_result_descriptor_group.value();
    }

    const vk::DescriptorGroup* PhysicallyBasedLighting::resultDescriptorGroup() const noexcept
    {
        return &_result_descriptor_group.value();
    }

    void PhysicallyBasedLighting::sync(Transition& transition)
    {
        transition.addSet(gbuffer_set_id)
            .bind(0, _src_stage, dstStage(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
            .bind(1, _src_stage, dstStage(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
            .bind(2, _src_stage, dstStage(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
            .bind(3, _src_stage, dstStage(), VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL);
    }

    void PhysicallyBasedLighting::srcStage(VkPipelineStageFlags2 src_stage) noexcept
    {
        _src_stage = src_stage;
    }
}
