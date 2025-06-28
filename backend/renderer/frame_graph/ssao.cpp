#include <backend/renderer/frame_graph/ssao.hpp>

#include <backend/profiling.hpp>

#include <backend/logger/logger.hpp>

namespace pbrlib::backend
{
    bool SSAO::init(vk::Device& device, const RenderContext& context)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        if (!RenderPass::init(device, context))
        {
            log::error("[ssao] failed initialize");
            return false;
        }

        return true;
    }

    bool SSAO::rebuild(vk::Device& device, const RenderContext& context)
    {
        return true;
    }

    void SSAO::render(size_t item_id, vk::CommandBuffer& command_buffer)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;
    }

    void SSAO::prePass(vk::CommandBuffer& command_buffer)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;
    }

    void SSAO::postPass(vk::CommandBuffer& command_buffer)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;
    }
}