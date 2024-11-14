#include <backend/renderer/frame_graph.hpp>
#include <backend/renderer/render_pass.hpp>

namespace pbrlib
{
    FrameGraph::FrameGraph(uint32_t width, uint32_t height)
    { 
        _size.width     = width;
        _size.height    = height;
    }

    void FrameGraph::render(VkCommandBuffer command_buffer_handle)
    {
        /// @todo
    }

    const Size& FrameGraph::size() const noexcept
    {
        return _size;
    }
}