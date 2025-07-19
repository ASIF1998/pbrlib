#pragma once

#include <pbrlib/math/matrix4x4.hpp>

#include <vulkan/vulkan.h>

#include <map>
#include <span>
#include <vector>
#include <tuple>

#include <string>
#include <string_view>

#include <map>

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
        explicit RenderPass(vk::Device& device) noexcept;

        RenderPass(RenderPass&& render_pass)        = delete;
        RenderPass(const RenderPass& render_pass)   = delete;

        virtual ~RenderPass() = default;

        RenderPass& operator = (RenderPass&& render_pass)       = delete;
        RenderPass& operator = (const RenderPass& render_pass)  = delete;

        virtual [[nodiscard]] bool init(const RenderContext& context, uint32_t width, uint32_t height);
        
        virtual [[nodiscard]] bool rebuild(uint32_t width, uint32_t height) = 0;

        virtual void draw(vk::CommandBuffer& command_buffer);

        virtual [[nodiscard]] VkPipelineStageFlags2 srcStage() const noexcept = 0;
        virtual [[nodiscard]] VkPipelineStageFlags2 dstStage() const noexcept = 0;

        void descriptorSet(uint32_t set_id, VkDescriptorSet set_handle, VkDescriptorSetLayout set_layout);

        [[nodiscard]] 
        std::pair<VkDescriptorSet, VkDescriptorSetLayout> descriptorSet(uint32_t set_id) const;

        virtual [[nodiscard]] std::pair<VkDescriptorSet, VkDescriptorSetLayout> resultDescriptorSet() const noexcept = 0;

        void addColorOutput(std::string_view name, vk::Image* ptr_image);

        void addSyncImage (
            vk::Image*              ptr_image, 
            VkImageLayout           new_layout, 
            VkPipelineStageFlags2   src_stage, 
            VkPipelineStageFlags2   dst_stage
        );

        void depthStencil(const vk::Image* ptr_image);
        
        [[nodiscard]]
        vk::Image* colorOutputAttach(std::string_view name);
        
        [[nodiscard]]
        const vk::Image* depthStencil() const noexcept;
        
        void sync(vk::CommandBuffer& command_buffer);

    protected:
        virtual void render(vk::CommandBuffer& command_buffer) = 0;

    protected:
        using SyncData = std::tuple <
            vk::Image*, 
            VkImageLayout, 
            VkPipelineStageFlags2, 
            VkPipelineStageFlags2
        >;

        std::map<std::string, vk::Image*, std::less<void>>  _color_output_images;
        
        std::vector<SyncData> _sync_images;

        const vk::Image* _ptr_depth_stencil_image = nullptr;

        const RenderContext*    _ptr_context    = nullptr;
        vk::Device*             _ptr_device     = nullptr;

        std::map<uint32_t, std::pair<VkDescriptorSet, VkDescriptorSetLayout>> _input_descriptor_sets;

        uint32_t _width     = 0;
        uint32_t _height    = 0;
    };
}