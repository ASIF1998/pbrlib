#pragma once

#include <backend/renderer/vulkan/device.hpp>
#include <backend/renderer/vulkan/image.hpp>

#include <pbrlib/math/vec3.hpp>

#include <string>
#include <string_view>

#include <memory>
#include <vector>
#include <map>

namespace pbrlib
{
    struct  IRenderPass;
    class   Window;
}

namespace pbrlib
{
    struct Size final
    {
        uint32_t width  = 0;
        uint32_t height = 0;
    };

    class FrameGraph final
    {
    public:
        [[maybe_unused]]
        explicit FrameGraph(uint32_t width, uint32_t height);

        [[maybe_unused]]
        explicit FrameGraph(const Window* ptr_window);

        FrameGraph(FrameGraph&& frame_graph)        = delete;
        FrameGraph(const FrameGraph& frame_graph)   = delete;

        FrameGraph& operator = (FrameGraph&& frame_graph)       = delete;
        FrameGraph& operator = (const FrameGraph& frame_graph)  = delete;

        [[nodiscard]] vk::Device& device() noexcept;
        // vk::Image render();

        [[nodiscard]]
        const Size& size() const noexcept;

    private:
        vk::Device _device;
        
        Size _size;
    };
}