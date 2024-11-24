#include <pbrlib/rendering/window.hpp>

#include <backend/renderer/frame_graph.hpp>
#include <backend/renderer/render_pass.hpp>

namespace pbrlib
{
    FrameGraph::FrameGraph(uint32_t width, uint32_t height)
    { 
        _size.width     = width;
        _size.height    = height;

        _device.init(nullptr);
    }

    FrameGraph::FrameGraph(const Window* ptr_window)
    {
        const auto [width, height] = ptr_window->size();

        _size.width     = static_cast<uint32_t>(width);
        _size.height    = static_cast<uint32_t>(height);

        _device.init(ptr_window);
    }

    // vk::Image FrameGraph::render()
    // {
    //     /// @todo
    // }

    const Size& FrameGraph::size() const noexcept
    {
        return _size;
    }
}