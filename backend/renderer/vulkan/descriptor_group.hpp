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
        using DescriptorBinderResource = std::variant<vk::Buffer*, vk::Image*>;

    public:
        explicit DescriptorGroup (
            Device&                                 device,
            const builders::DescriptorSetLayout&    descriptor_set_layout_builder,
            std::string_view                        name = ""
        );

        void add(uint32_t bind_id, vk::Buffer& buffer);
        void add(uint32_t bind_id, vk::Image& image);

        void modify(std::function<void(uint32_t, vk::Image&)> modifier);
        void modify(std::function<void(uint32_t, vk::Buffer&)> modifier);

        /// @todo remove
        void changeColorImagesLayout(CommandBuffer& command_buffer, VkImageLayout new_layout);

        void transition(CommandBuffer& command_buffer, const DescriptorGroupTransition& descriptor_group_transition) const;

        [[nodiscard]] const VkDescriptorSet&        descriptorSetHandle()       const noexcept;
        [[nodiscard]] const VkDescriptorSetLayout&  descriptorSetLayoutHandle() const noexcept;

    private:
        vk::DescriptorSetLayoutHandle   _set_layout;
        vk::DescriptorSetHandle         _set_handle;

        std::map<uint32_t, DescriptorBinderResource> _resources;
    };
}
