#pragma once

#include <pbrlib/math/matrix4x4.hpp>

#include <vulkan/vulkan.h>

#include <map>
#include <span>
#include <vector>
#include <tuple>

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
    class Device;
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
        
        virtual [[nodiscard]] bool rebuild() = 0;

        virtual void draw(vk::CommandBuffer& command_buffer);

        virtual [[nodiscard]] VkPipelineStageFlags2 srcStage() const noexcept = 0;
        virtual [[nodiscard]] VkPipelineStageFlags2 dstStage() const noexcept = 0;

        // virtual void                            descriptorSet(uint32_t set_id, VkDescriptorSet set_handle);
        // virtual [[nodiscard]] VkDescriptorSet   descriptorSet(uint32_t set_id) const;

        void addColorOutput(std::string_view name, vk::Image* ptr_image);

        void addColorInput (
            std::string_view        name, 
            vk::Image*              ptr_image, 
            VkImageLayout           new_layout, 
            VkPipelineStageFlags2   src_stage, 
            VkPipelineStageFlags2   dst_stage
        );

        void depthStencil(const vk::Image* ptr_image);
        
        [[nodiscard]]
        vk::Image* colorOutputAttach(std::string_view name);
        
        [[nodiscard]]
        const vk::Image* colorInputAttach(std::string_view name) const;

        [[nodiscard]]
        const vk::Image* depthStencil() const noexcept;
        
        void sync(vk::CommandBuffer& command_buffer);

    protected:
        virtual void render(vk::CommandBuffer& command_buffer) = 0;

    protected:
        using SynkData = std::tuple <
            vk::Image*, 
            VkImageLayout, 
            VkPipelineStageFlags2, 
            VkPipelineStageFlags2
        >;

        std::map<std::string, vk::Image*, std::less<void>>  _color_output_images;
        std::map<std::string, SynkData, std::less<void>>    _color_input_images;

        const vk::Image* _ptr_depth_stencil_image = nullptr;

        const RenderContext*    _ptr_context    = nullptr;
        vk::Device*             _ptr_device     = nullptr;
    };
}