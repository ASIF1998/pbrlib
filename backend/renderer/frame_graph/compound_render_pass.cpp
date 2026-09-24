#include <backend/renderer/frame_graph/compound_render_pass.hpp>

#include <backend/logger/logger.hpp>
#include <backend/profiling.hpp>

namespace pbrlib::backend
{
    CompoundRenderPass::CompoundRenderPass(vk::Device& device) noexcept :
        RenderPass(device)
    { }

    bool CompoundRenderPass::init(const RenderContext& context, uint32_t width, uint32_t height)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        if (RenderPass::init(context, width, height)) [[likely]]
        {
            for (auto& subpass: _subpasses)
            {
                if (!subpass->init(context, width, height)) [[unlikely]]
                {
                    log::error("[compound-render-pass] failed initialize");
                    return false;
                }
            }

            if (!_subpasses.empty()) [[likely]]
                _result_descriptor_group = _subpasses.back()->resultDescriptorGroup();

            return true;
        }

        log::error("[compound-render-pass] failed initialize");

        return false;
    }

    void CompoundRenderPass::render(vk::CommandBuffer& command_buffer)
    { }

    void CompoundRenderPass::draw(vk::CommandBuffer& command_buffer)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        for (const auto& ptr_subpass: _subpasses)
            ptr_subpass->draw(command_buffer);
    }

    VkPipelineStageFlags2 CompoundRenderPass::srcStage() const noexcept
    {
        if (_subpasses.empty()) [[unlikely]]
            return VK_PIPELINE_STAGE_2_NONE;

        return _subpasses.front()->srcStage();
    }

    VkPipelineStageFlags2 CompoundRenderPass::dstStage() const noexcept
    {
        if (_subpasses.empty()) [[unlikely]]
            return VK_PIPELINE_STAGE_2_NONE;

        return _subpasses.back()->dstStage();
    }

    const vk::DescriptorGroup* CompoundRenderPass::resultDescriptorGroup() const noexcept
    {
        return _result_descriptor_group;
    }

    vk::DescriptorGroup* CompoundRenderPass::resultDescriptorGroup() noexcept
    {
        return _result_descriptor_group;
    }

    void CompoundRenderPass::sync(Transition& transition)
    { }
}
