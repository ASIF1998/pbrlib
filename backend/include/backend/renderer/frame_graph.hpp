#pragma once

#include <pbrlib/math/vec3.hpp>

#include <vulkan/vulkan.h>

#include <string>
#include <string_view>

#include <memory>
#include <vector>
#include <map>

namespace pbrlib
{
    struct IRenderPass;
}

namespace pbrlib
{
    struct Size
    {
        uint32_t width  = 0;
        uint32_t height = 0;
    };

    class FrameGraph
    {
    public:
        explicit FrameGraph(uint32_t width, uint32_t height);

        FrameGraph(FrameGraph&& frame_graph)        = delete;
        FrameGraph(const FrameGraph& frame_graph)   = delete;

        FrameGraph& operator = (FrameGraph&& frame_graph)       = delete;
        FrameGraph& operator = (const FrameGraph& frame_graph)  = delete;

        void render(VkCommandBuffer command_buffer_handle);

        [[nodiscard]]
        const Size& size() const noexcept;

    private:
        Size _size;
    };
}