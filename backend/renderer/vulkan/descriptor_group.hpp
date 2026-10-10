#pragma once

#include <backend/renderer/vulkan/unique_handler.hpp>

#include <map>
#include <unordered_map>
#include <variant>
#include <tuple>

#include <functional>

namespace pbrlib::backend::vk::builders
{
    class DescriptorSetLayout;
}

namespace pbrlib::backend::vk
{
    class CommandBuffer;
    class Image;
    class Buffer;
    class Device;
}

namespace pbrlib::backend::vk
{
    struct DescriptorImageInfo final
    {
        const Image&    image;
        VkSampler       sampler_handle          = VK_NULL_HANDLE;
        VkImageLayout   expected_image_layout   = VK_IMAGE_LAYOUT_UNDEFINED;
        uint32_t        binding                 = 0;
        uint32_t        array_element           = 0;
    };

    struct DescriptorBufferInfo final
    {
        const Buffer&   buffer;
        uint32_t        offset          = 0;
        uint32_t        size            = 0;
        uint32_t        binding         = 0;
        uint32_t        array_element   = 0;
    };

    class DescriptorGroupTransition final
    {
        static constexpr auto NoImageLayout = VK_IMAGE_LAYOUT_MAX_ENUM;

    public:
        using StageConfig  = std::tuple<VkPipelineStageFlags2, VkPipelineStageFlags2, VkImageLayout>;

        DescriptorGroupTransition& bind (
            uint32_t                bind_id,
            VkPipelineStageFlags2   src_stage,
            VkPipelineStageFlags2   dst_stage,
            VkImageLayout           image_layout = NoImageLayout
        );

        [[nodiscard]] std::optional<StageConfig>    config(int32_t bind_id)     const;
        [[nodiscard]] bool                          hasBind(uint32_t bind_id)   const;

    private:
        std::unordered_map<uint32_t, StageConfig> _binds;
    };

    class DescriptorGroup final
    {
        using DescriptorBinderResource = std::variant<const Buffer*, const Image*>;

    public:
        explicit DescriptorGroup (
            Device&                                 device,
            const builders::DescriptorSetLayout&    descriptor_set_layout_builder,
            std::string_view                        name = ""
        );

        void transition(CommandBuffer& command_buffer, const DescriptorGroupTransition& descriptor_group_transition) const;

        [[nodiscard]] const VkDescriptorSet&        descriptorSetHandle()       const noexcept;
        [[nodiscard]] const VkDescriptorSetLayout&  descriptorSetLayoutHandle() const noexcept;

        void write(const DescriptorImageInfo& descriptor_image_info);
        void write(const DescriptorBufferInfo& descriptor_buffer_info);

    private:
        Device& _device;

        DescriptorSetLayoutHandle   _set_layout;
        DescriptorSetHandle         _set_handle;

        std::map<uint32_t, DescriptorBinderResource> _resources;
    };
}
