#pragma once

#include <vulkan/vulkan.h>

#include <map>

#include <string>
#include <string_view>

namespace pbrlib::vk
{
    class Device;
    class Image;
    class CommandBuffer;
}

namespace pbrlib
{
    class SceneItem;
}

namespace pbrlib
{
    class RenderPass
    {
    public:
        RenderPass() = default;

        RenderPass(RenderPass&& render_pass)        = delete;
        RenderPass(const RenderPass& render_pass)   = delete;

        virtual ~RenderPass() = default;

        RenderPass& operator = (RenderPass&& render_pass)       = delete;
        RenderPass& operator = (const RenderPass& render_pass)  = delete;

        virtual bool init(const vk::Device* ptr_device)                                     = 0;
        virtual void render(const SceneItem* ptr_item, vk::CommandBuffer& command_buffer)   = 0; 

        void addColorOutput(std::string_view name, const vk::Image* ptr_image);
        void depthStencil(const vk::Image* ptr_image);
        
        [[nodiscard]]
        const vk::Image* colorOutputAttach(std::string_view name);

        [[nodiscard]]
        const vk::Image* depthStencil() const noexcept;

    protected:
        std::map<std::string, const vk::Image*, std::less<void>> _color_output_images;

        const vk::Image* _ptr_depth_stencil_image = nullptr;
    };
}