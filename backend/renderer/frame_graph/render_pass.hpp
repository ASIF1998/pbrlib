#pragma once

#include <backend/renderer/vulkan/descriptor_group.hpp>

#include <pbrlib/math/matrix4x4.hpp>

#include <map>
#include <unordered_map>
#include <span>
#include <vector>
#include <tuple>

#include <string>
#include <string_view>

#include <functional>

#include <limits>

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

        uint8_t flight_frame_index = std::numeric_limits<uint8_t>::max() - 1;
    };

    class Transition final
    {
    public:
        [[nodiscard]] vk::DescriptorGroupTransition* set(uint32_t set_id);
        [[nodiscard]] vk::DescriptorGroupTransition& addSet(uint32_t set_id);
        
        [[nodiscard]] bool has(uint32_t set_id) const; 

    private:
        std::unordered_map<uint32_t, vk::DescriptorGroupTransition> _sets;
    };

    class RenderPass
    {
        using ColorOutputImages = std::map <
            std::string,
            vk::Image*,
            std::less<void>
        >;

    public:
        struct ReservedSetSlots final
        {
            static constexpr uint32_t result_descriptor_set_id = std::numeric_limits<uint32_t>::max();
        };

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
        void depthStencil(vk::Image* ptr_image);

        [[nodiscard]] vk::Image*        colorOutputAttach(std::string_view name);
        [[nodiscard]] vk::Image*        depthStencil() noexcept;
        [[nodiscard]] const vk::Image*  depthStencil() const noexcept;

        [[nodiscard]] vk::Device&           device()    noexcept;
        [[nodiscard]] const RenderContext&  context()   const noexcept;

        [[nodiscard]] std::pair<uint32_t, uint32_t> size() const noexcept;

        void descriptorGroup(uint32_t set_id, const vk::DescriptorGroup& descriptor_group);

        [[nodiscard]] const vk::DescriptorGroup* descriptorGroup(uint32_t set_id) const;

        [[nodiscard]] virtual vk::DescriptorGroup*          resultDescriptorGroup() noexcept        = 0;
        [[nodiscard]] virtual const vk::DescriptorGroup*    resultDescriptorGroup() const noexcept  = 0;

    protected:
        virtual void render(vk::CommandBuffer& command_buffer)  = 0;
        virtual void sync(Transition& transition)               = 0;

    private:
        ColorOutputImages _color_output_images;

        vk::Image* _ptr_depth_stencil_image = nullptr;

        const RenderContext*    _ptr_context    = nullptr;
        vk::Device*             _ptr_device     = nullptr;

        uint32_t _width     = 0;
        uint32_t _height    = 0;

        std::map<uint32_t, const vk::DescriptorGroup*> _descriptor_groups;
    };
}
