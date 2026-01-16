#pragma once

#include <backend/renderer/vulkan/utils.hpp>
#include <backend/renderer/vulkan/unique_handler.hpp>

#include <vector>
#include <span>

#include <string>
#include <string_view>

namespace pbrlib::backend::vk
{
    class Device;
}

namespace pbrlib::backend::vk::builders
{
    class Buffer;
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
        friend class builders::Buffer;

        explicit Buffer(Device& device) noexcept;

        void writeToVram(const uint8_t* ptr_data, size_t size, VkDeviceSize offset);
        void writeToRam(const uint8_t* ptr_data, size_t size, VkDeviceSize offset);

    public:
        Buffer(Buffer&& buffer) noexcept;
        Buffer(const Buffer& buffer) = delete;

        Buffer& operator = (Buffer&& buffer) noexcept;
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

        BufferHandle handle;
        VkDeviceSize size   = 0;

        BufferType type;

        VkBufferUsageFlags usage  = VK_BUFFER_USAGE_FLAG_BITS_MAX_ENUM;

    private:
        Device& _device;
    };
}

namespace pbrlib::backend::vk::builders
{
    class Buffer final
    {
        void validate();

        VkSharingMode sharingMode() const;

    public:
        explicit Buffer(Device& device);

        Buffer(Buffer&& builder)      = delete;
        Buffer(const Buffer& builder) = delete;

        Buffer& operator = (Buffer&& builder)         = delete;
        Buffer& operator = (const Buffer& builder)    = delete;

        Buffer& addQueueFamilyIndex(uint32_t index);
        Buffer& size(VkDeviceSize size)                noexcept;
        Buffer& usage(VkImageUsageFlags usage)         noexcept;
        Buffer& type(BufferType buffer_type)           noexcept;

        Buffer& name(std::string_view buffer_name);

        [[nodiscard]] vk::Buffer build();

    private:
        Device& _device;

        std::vector<uint32_t> _queues;

        VkDeviceSize        _size   = 0;
        VkBufferUsageFlags  _usage  = VK_BUFFER_USAGE_FLAG_BITS_MAX_ENUM;

        BufferType _type = BufferType::eDeviceOnly;

        std::string _name;
    };
}
