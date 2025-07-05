#include <backend/renderer/frame_graph/compound_render_pass.hpp>

#include <backend/logger/logger.hpp>

#include <backend/profiling.hpp>

namespace pbrlib::backend
{
    bool CompoundRenderPass::init(vk::Device& device, const RenderContext& context)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        if (RenderPass::init(device, context)) [[likely]]
        {
            for (auto& subpass: _subpasses)
            {
                if (!subpass->init(device, context)) [[unlikely]]
                {
                    log::error("[compound-render-pass] failed initialize");
                    return false;
                }
            }

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

    bool CompoundRenderPass::rebuild() 
    {
        bool res = true;
        for (auto& subpass: _subpasses)
            res &= subpass->rebuild();

        return res;
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
}