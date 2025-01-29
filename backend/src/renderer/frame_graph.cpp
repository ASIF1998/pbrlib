#include <pbrlib/rendering/window.hpp>

#include <backend/renderer/frame_graph.hpp>
#include <backend/renderer/render_pass.hpp>

namespace pbrlib
{
    FrameGraph::FrameGraph(vk::Device* ptr_device, uint32_t width, uint32_t height) :
        _ptr_device(ptr_device)
    {
        if (!ptr_device) 
            throw std::runtime_error("[FrameGraph] Pointer to device is null");

        _size.width     = width;
        _size.height    = height;
    }

    FrameGraph::FrameGraph(vk::Device* ptr_device, const Window* ptr_window) :
        _ptr_device (ptr_device)
    {
        if (!ptr_device) 
            throw std::runtime_error("[FrameGraph] Pointer to device is null");

        const auto [width, height] = ptr_window->size();

        _size.width     = static_cast<uint32_t>(width);
        _size.height    = static_cast<uint32_t>(height);
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