#pragma once

#include <backend/renderer/vulkan/buffer.hpp>

#include <pbrlib/math/matrix4x4.hpp>

#include <vulkan/vulkan.h>

#include <map>
#include <span>
#include <vector>

#include <string>
#include <string_view>

#include <optional>

namespace pbrlib
{
    class SceneItem;
}

namespace pbrlib::backend
{
    class MaterialManager;
    class MeshManager;
}

namespace pbrlib::backend::vk
{
    class Image;
    class CommandBuffer;
}

namespace pbrlib::backend
{
    struct RenderContext final
    {
        std::span<const SceneItem*> items;

        math::mat4 projection;
        math::mat4 view;

        const MaterialManager*  ptr_material_manager    = nullptr;
        const MeshManager*      ptr_mesh_manager        = nullptr;
    };

    class RenderPass
    {
    public:
        RenderPass() = default;

        RenderPass(RenderPass&& render_pass)        = delete;
        RenderPass(const RenderPass& render_pass)   = delete;

        virtual ~RenderPass() = default;

        RenderPass& operator = (RenderPass&& render_pass)       = delete;
        RenderPass& operator = (const RenderPass& render_pass)  = delete;

        virtual [[nodiscard]] bool init(vk::Device& device, const RenderContext& context);
        virtual [[nodiscard]] bool rebuild(vk::Device& device, const RenderContext& context) = 0;

        virtual void draw(vk::CommandBuffer& command_buffer);

        void addColorOutput(std::string_view name, vk::Image* ptr_image);
        void depthStencil(const vk::Image* ptr_image);
        
        [[nodiscard]]
        vk::Image* colorOutputAttach(std::string_view name);

        [[nodiscard]]
        const vk::Image* depthStencil() const noexcept;

    protected:
        virtual void prePass(vk::CommandBuffer& command_buffer);
        virtual void render(size_t item_id , vk::CommandBuffer& command_buffer) = 0; 
        virtual void postPass(vk::CommandBuffer& command_buffer);

    protected:
        std::map<std::string, vk::Image*, std::less<void>> _color_output_images;

        const vk::Image* _ptr_depth_stencil_image = nullptr;

        const RenderContext*    _ptr_context    = nullptr;
        vk::Device*             _ptr_device     = nullptr;
    };
}