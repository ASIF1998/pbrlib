#pragma once

#include <backend/renderer/frame_graph/compound_render_pass.hpp>

#include <backend/renderer/vulkan/image.hpp>

#include <pbrlib/math/vec3.hpp>

#include <string>
#include <string_view>

#include <memory>
#include <vector>
#include <map>

#include <optional>

namespace pbrlib
{
    class Window;
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
        explicit FrameGraph(vk::Device* ptr_device, uint32_t width, uint32_t height);

        [[maybe_unused]]
        explicit FrameGraph(vk::Device* ptr_device, const Window* ptr_window);

        FrameGraph(FrameGraph&& frame_graph)        = delete;
        FrameGraph(const FrameGraph& frame_graph)   = delete;

        FrameGraph& operator = (FrameGraph&& frame_graph)       = delete;
        FrameGraph& operator = (const FrameGraph& frame_graph)  = delete;

        const vk::Image& draw();

        [[nodiscard]]
        const Size size() const;

    private:
        vk::Device* _ptr_device = nullptr;
        
        std::optional<vk::Image> _image;

        CompoundRenderPass _render_pass;
    };
}