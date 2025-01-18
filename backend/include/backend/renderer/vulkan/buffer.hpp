#pragma once

#include <vma/vk_mem_alloc.h>
#include <vulkan/vulkan.h>

#include <vector>
#include <span>

#include <string>
#include <string_view>

namespace pbrlib::vk
{
    class Device;
}

namespace pbrlib::vk
{
    class Buffer final
    {
        explicit Buffer(Device* ptr_device);

        void write(const uint8_t* ptr_data, size_t size, VkDeviceSize offset);

    public:
        class Builder;

    public:
        Buffer(Buffer&& buffer);
        Buffer(const Buffer& buffer) = delete;

        ~Buffer();

        Buffer& operator = (Buffer&& buffer);
        Buffer& operator = (const Buffer& buffer) = delete;

        template<typename T>
        void write(std::span<const T> data, VkDeviceSize offset)
        {
            write(reinterpret_cast<const uint8_t*>(data.data()), data.size_bytes(), offset);
        }

        template<typename T>
        void write(const T& obj, VkDeviceSize offset)
        {
            write(reinterpret_cast<const uint8_t*>(&obj), sizeof(T), offset);
        }

        VkBuffer        handle  = VK_NULL_HANDLE;
        VkDeviceSize    size    = 0;

    private:
        Device*         _ptr_device;
        VmaAllocation   _allocation = VK_NULL_HANDLE;
    };

    class Buffer::Builder final
    {
        void validate();

        VkSharingMode sharingMode() const;

    public:
        explicit Builder(Device* ptr_device);

        Builder(Builder&& builder)      = delete;
        Builder(const Builder& builder) = delete;

        Builder& operator = (Builder&& builder)         = delete;
        Builder& operator = (const Builder& builder)    = delete;

        Builder& addQueueFamilyIndex(uint32_t index);
        Builder& size(VkDeviceSize size)                noexcept;
        Builder& usage(VkImageUsageFlags usage)         noexcept;

        [[maybe_unused]] Builder& name(std::string_view buffer_name);

        [[nodiscard]] Buffer build();

    private:
        Device* _ptr_device;

        std::vector<uint32_t> _queues;

        VkDeviceSize        _size   = 0;
        VkImageUsageFlags   _usage  = VK_BUFFER_USAGE_FLAG_BITS_MAX_ENUM;

        std::string _name;
    };
}
