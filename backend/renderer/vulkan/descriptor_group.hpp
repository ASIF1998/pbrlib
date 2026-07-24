#pragma once

#include <backend/renderer/vulkan/unique_handler.hpp>

#include <map>
#include <variant>

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

        void changeColorImagesLayout(CommandBuffer& command_buffer, VkImageLayout new_layout);

        [[nodiscard]] VkDescriptorSet       descriptorSetHandle()       noexcept;
        [[nodiscard]] VkDescriptorSetLayout descriptorSetLayoutHandle() noexcept;

    private:
        vk::DescriptorSetLayoutHandle   _set_layout;
        vk::DescriptorSetHandle         _set_handle;

        std::map<uint32_t, DescriptorBinderResource> _resources;
    };
}
