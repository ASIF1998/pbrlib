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
#include <functional>

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
    template<typename T>
    struct AttachmentsTraits final
    { };

    struct AttachmentMetadata final
    {
        std::string_view    name;
        VkFormat            format  = VK_FORMAT_UNDEFINED;
        VkImageUsageFlags   usage   = VK_IMAGE_USAGE_FLAG_BITS_MAX_ENUM;
    };

    template <typename RenderPass>
    concept HasAttachments = requires()
    {
        {AttachmentsTraits<RenderPass>::metadata()} -> std::convertible_to<std::span<const AttachmentMetadata>>;
    };

    template<typename T>
    struct InputDescriptorSetTraits final
    { };
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
        using SyncData = std::tuple <
            vk::Image*,
            VkImageLayout,
            VkPipelineStageFlags2,
            VkPipelineStageFlags2
        >;

        using ColorOutputImages = std::map <
            std::string, 
            vk::Image*, 
            std::less<void>
        >;

        using InputDescriptorSets = std::unordered_map <
            uint32_t, 
            std::pair<VkDescriptorSet, VkDescriptorSetLayout>
        >;

        void sync(vk::CommandBuffer& command_buffer);

    public:
        explicit RenderPass(vk::Device& device) noexcept;

        RenderPass(RenderPass&& render_pass)        = delete;
        RenderPass(const RenderPass& render_pass)   = delete;

        virtual ~RenderPass() = default;

        RenderPass& operator = (RenderPass&& render_pass)       = delete;
        RenderPass& operator = (const RenderPass& render_pass)  = delete;

        [[nodiscard]] virtual bool init(const RenderContext& context, uint32_t width, uint32_t height);

        virtual void draw(vk::CommandBuffer& command_buffer);

        [[nodiscard]] virtual VkPipelineStageFlags2 srcStage() const noexcept = 0;
        [[nodiscard]] virtual VkPipelineStageFlags2 dstStage() const noexcept = 0;

        void addColorOutput(std::string_view name, vk::Image* ptr_image);

        void addSyncImage (
            vk::Image*              ptr_image,
            VkImageLayout           new_layout,
            VkPipelineStageFlags2   src_stage,
            VkPipelineStageFlags2   dst_stage
        );

        void depthStencil(const vk::Image* ptr_image);

        [[nodiscard]] vk::Image*        colorOutputAttach(std::string_view name);
        [[nodiscard]] const vk::Image*  depthStencil() const noexcept;

        [[nodiscard]] vk::Device&           device() noexcept;
        [[nodiscard]] const RenderContext&  context() const noexcept;

        [[nodiscard]] std::pair<uint32_t, uint32_t> size() const noexcept;

        void descriptorSet(uint32_t set_id, VkDescriptorSet set_handle, VkDescriptorSetLayout set_layout);

        [[nodiscard]]
        std::pair<VkDescriptorSet, VkDescriptorSetLayout> descriptorSet(uint32_t set_id) const;

        [[nodiscard]]
        virtual std::pair<VkDescriptorSet, VkDescriptorSetLayout> resultDescriptorSet() const noexcept = 0;

    protected:
        virtual void render(vk::CommandBuffer& command_buffer) = 0;

    private:
        ColorOutputImages _color_output_images;

        std::vector<SyncData> _sync_images;

        const vk::Image* _ptr_depth_stencil_image = nullptr;

        const RenderContext*    _ptr_context    = nullptr;
        vk::Device*             _ptr_device     = nullptr;

        uint32_t _width     = 0;
        uint32_t _height    = 0;

        InputDescriptorSets _input_descriptor_sets;
    };
}
