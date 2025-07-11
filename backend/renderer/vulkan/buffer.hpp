#pragma once

#include <backend/renderer/vulkan/utils.hpp>

#include <vma/vk_mem_alloc.h>
#include <vulkan/vulkan.h>

#include <vector>
#include <span>

#include <string>
#include <string_view>

namespace pbrlib::backend::vk
{
    class Device;
}

namespace pbrlib::backend::vk
{
    enum class BufferType
    {
        eDeviceOnly,    //!< Any resources that you frequently write and read on GPU.
        eStaging,       //!< A "staging" buffer than you want to map and fill from CPU code, then use as a source of transfer to some GPU resource.
        eReadback       //!< Buffers for data written by or transferred from the GPU that you want to read back on the CPU.
    };

    class Buffer final
    {
        explicit Buffer(Device& device);

        void writeToVram(const uint8_t* ptr_data, size_t size, VkDeviceSize offset);
        void writeToRam(const uint8_t* ptr_data, size_t size, VkDeviceSize offset);

    public:
        class Builder;

    public:
        Buffer(Buffer&& buffer);
        Buffer(const Buffer& buffer) = delete;

        ~Buffer();

        Buffer& operator = (Buffer&& buffer);
        Buffer& operator = (const Buffer& buffer) = delete;

        template<typename T>
        void write(std::span<const T> data, VkDeviceSize offset_in_dst)
        {
            const auto ptr_data = reinterpret_cast<const uint8_t*>(data.data());

            if (type == BufferType::eStaging)
                writeToRam(ptr_data, data.size_bytes(), offset_in_dst);
            else 
                writeToVram(ptr_data, data.size_bytes(), offset_in_dst);
        }

        template<NotSequenceContainer T>
        void write(const T& obj, VkDeviceSize offset_in_dst)
        {
            const auto size     = sizeof(T);
            const auto ptr_data = reinterpret_cast<const uint8_t*>(&obj);

            if (type == BufferType::eStaging)
                writeToRam(ptr_data, size, offset_in_dst);
            else 
                writeToVram(ptr_data, size, offset_in_dst);
        }

        void write(const Buffer& buffer, VkDeviceSize offset_in_dst);

        template<typename T>
        void read(T& dst, VkDeviceSize offset_in_src)
        {
            read(reinterpret_cast<uint8_t*>(&dst), sizeof(T), offset_in_src);
        }

        void read(uint8_t* ptr_dst, VkDeviceSize size, VkDeviceSize offset_in_src);

        VkDeviceAddress address() const;

        VkBuffer        handle  = VK_NULL_HANDLE;
        VkDeviceSize    size    = 0;

        BufferType type;

    private:
        Device&         _device;
        VmaAllocation   _allocation = VK_NULL_HANDLE;
    };

    class Buffer::Builder final
    {
        void validate();

        VkSharingMode sharingMode() const;

    public:
        explicit Builder(Device& device);

        Builder(Builder&& builder)      = delete;
        Builder(const Builder& builder) = delete;

        Builder& operator = (Builder&& builder)         = delete;
        Builder& operator = (const Builder& builder)    = delete;

        Builder& addQueueFamilyIndex(uint32_t index);
        Builder& size(VkDeviceSize size)                noexcept;
        Builder& usage(VkImageUsageFlags usage)         noexcept;
        Builder& type(BufferType buffer_type)           noexcept;

        Builder& name(std::string_view buffer_name);

        [[nodiscard]] Buffer build();

    private:
        Device& _device;

        std::vector<uint32_t> _queues;

        VkDeviceSize        _size   = 0;
        VkImageUsageFlags   _usage  = VK_BUFFER_USAGE_FLAG_BITS_MAX_ENUM;

        BufferType _type = BufferType::eDeviceOnly;

        std::string _name;
    };
}
