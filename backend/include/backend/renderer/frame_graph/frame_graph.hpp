#pragma once

#include <backend/renderer/frame_graph/render_pass.hpp>

#include <backend/renderer/vulkan/image.hpp>
#include <backend/renderer/vulkan/surface.hpp>

#include <pbrlib/math/vec3.hpp>

#include <string>
#include <string_view>

#include <memory>
#include <vector>
#include <map>

#include <optional>

namespace pbrlib
{
    class   Window;
    struct  Config;
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
        void createResources();

        void present() const;
        void nextImage();

        void build();

    public:
        explicit FrameGraph(vk::Device* ptr_device, const Config& config);
        explicit FrameGraph(vk::Device* ptr_device, const Window* ptr_window);

        FrameGraph(FrameGraph&& frame_graph)        = delete;
        FrameGraph(const FrameGraph& frame_graph)   = delete;

        FrameGraph& operator = (FrameGraph&& frame_graph)       = delete;
        FrameGraph& operator = (const FrameGraph& frame_graph)  = delete;

        [[nodiscard]]
        const Size size() const;

        void draw(std::span<const SceneItem*> items);

    private:
        vk::Device* _ptr_device = nullptr;
        
        std::optional<vk::Image>    _image;
        std::optional<vk::Surface>  _surface;

        const vk::Image*    _ptr_output_image   = nullptr; 
        uint32_t            _image_index        = 0;

        std::unique_ptr<RenderPass> _ptr_render_pass;

        std::optional<vk::Image> _depth_buffer;
    };
}