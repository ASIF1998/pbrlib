#pragma once

#include <backend/renderer/frame_graph/render_pass.hpp>
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
    struct Config;
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
        explicit FrameGraph(vk::Device* ptr_device, const Config& config);

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

        std::unique_ptr<IRenderPass> _ptr_render_pass;
    };
}