#include <backend/renderer/frame_graph/compound_render_pass.hpp>

#include <backend/logger/logger.hpp>

#include <backend/profiling.hpp>

namespace pbrlib::backend
{
    bool CompoundRenderPass::init(vk::Device& device, const RenderContext& context)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        if (RenderPass::init(device, context)) 
        {
            for (auto& subpass: _subpasses)
            {
                if (!subpass->init(device, context))
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

    void CompoundRenderPass::render(size_t item_id, vk::CommandBuffer& command_buffer)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;
        /// @todo dont use
    }

    void CompoundRenderPass::draw(vk::CommandBuffer& command_buffer)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        for (const auto& ptr_subpass: _subpasses)
            ptr_subpass->draw(command_buffer);
    } 
}