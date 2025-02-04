#include <backend/renderer/frame_graph/compound_render_pass.hpp>

namespace pbrlib
{
    bool CompoundRenderPass::init(const vk::Device* ptr_device)
    {
        for (auto& subpass: _subpasses)
        {
            if (!subpass->init(ptr_device))
                return false;
        }

        return true;
    }

    void CompoundRenderPass::render(vk::Image& result)
    {
        /// @todo 
    }
}