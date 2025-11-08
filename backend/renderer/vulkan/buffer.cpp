#include <backend/renderer/vulkan/buffer.hpp>
#include <backend/renderer/vulkan/device.hpp>

#include <backend/utils/vulkan.hpp>

#include <backend/renderer/vulkan/gpu_marker_colors.hpp>

#include <unordered_set>

namespace pbrlib::backend::vk
{
    Buffer::Buffer(Device& device) noexcept :
        _device (device)
    { }

    Buffer::Buffer(Buffer&& buffer) noexcept :
        _device (buffer._device),
        usage   (buffer.usage)
    {
        std::swap(handle, buffer.handle);
        std::swap(size, buffer.size);
        std::swap(type, buffer.type);
    }

    Buffer& Buffer::operator = (Buffer&& buffer) noexcept
    {
        std::swap(handle, buffer.handle);
        std::swap(size, buffer.size);
        std::swap(type, buffer.type);
        std::swap(usage, buffer.usage);

        return *this;
    }

    void Buffer::writeToVram(const uint8_t* ptr_data, size_t data_size, VkDeviceSize offset)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        auto staging_buffer = builders::Buffer(_device)
            .name("staging-buffer")
            .size(data_size)
            .usage(VK_BUFFER_USAGE_TRANSFER_SRC_BIT)
            .addQueueFamilyIndex(_device.queue().family_index)
            .type(BufferType::eStaging)
            .build();

        staging_buffer.writeToRam(ptr_data, data_size, 0);

        auto command_buffer = _device.oneTimeSubmitCommandBuffer("uplaod-data-to-buffer");

        command_buffer.write([&staging_buffer, offset, data_size, this](VkCommandBuffer command_buffer_handle)
        {
            PBRLIB_PROFILING_VK_ZONE_SCOPED(_device, command_buffer_handle, "[vk-buffer] upalod-data-to-device-only-buffer");

            const VkBufferCopy copy 
            { 
                .srcOffset  = 0,
                .dstOffset  = offset,
                .size       = static_cast<VkDeviceSize>(data_size)
            };

            vkCmdCopyBuffer(command_buffer_handle, staging_buffer.handle, handle, 1, &copy);
        }, "[vk-buffer] upalod-data-to-device-only-buffer", marker_colors::write_data_in_buffer);

        _device.submit(command_buffer);
    }

    void Buffer::writeToRam(const uint8_t* ptr_data, size_t data_size, VkDeviceSize offset)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        VK_CHECK(vmaCopyMemoryToAllocation(
            _device.vmaAllocator(),
            ptr_data,
            handle.context<VmaAllocation>(), offset,
            data_size
        ));
    }

    void Buffer::write(const Buffer& buffer, VkDeviceSize offset_in_dst)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        auto command_buffer = _device.oneTimeSubmitCommandBuffer("command-buffer-for-copy");
        command_buffer.write([offset_in_dst, &buffer, this](VkCommandBuffer command_buffer_handle)
        {
            PBRLIB_PROFILING_VK_ZONE_SCOPED(_device, command_buffer_handle, "[vk-buffer] write-data-in-buffer");

            const VkBufferCopy copy 
            { 
                .srcOffset  = 0,
                .dstOffset  = offset_in_dst,
                .size       = buffer.size
            };

            vkCmdCopyBuffer(command_buffer_handle, buffer.handle, handle, 1, &copy);
        }, "[vk-buffer] write-data-in-buffer", marker_colors::write_data_in_buffer);

        _device.submit(command_buffer);
    }

    void Buffer::read(uint8_t* ptr_dst, VkDeviceSize size, VkDeviceSize offset_in_src)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        if (type == BufferType::eDeviceOnly)
            throw exception::RuntimeError("[vk-buffer] device only buffer don't support read value from CPU");

        void* ptr_src = nullptr;

        const auto allocation_handle = handle.context<VmaAllocation>();

        VK_CHECK(vmaMapMemory(_device.vmaAllocator(), allocation_handle, &ptr_src));
        memcpy(ptr_dst, reinterpret_cast<uint8_t*>(ptr_src) + offset_in_src, static_cast<size_t>(size));
        vmaUnmapMemory(_device.vmaAllocator(), allocation_handle);
    }
}

namespace pbrlib::backend::vk
{
    VkDeviceAddress Buffer::address() const
    {
        const VkBufferDeviceAddressInfo buffer_device_address_info 
        { 
            .sType  = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO,
            .buffer = handle
        };

        return vkGetBufferDeviceAddress(_device.device(), &buffer_device_address_info);
    }
}

namespace pbrlib::backend::vk::builders
{
    Buffer::Buffer(Device& device) :
        _device (device)
    { }

    Buffer& Buffer::addQueueFamilyIndex(uint32_t index)
    {
        _queues.push_back(index);
        return *this;
    }

    Buffer& Buffer::size(VkDeviceSize size) noexcept
    {
        _size = size;
        return *this;
    }

    Buffer& Buffer::usage(VkImageUsageFlags usage) noexcept
    {
        _usage = usage;
        return *this;
    }

    Buffer& Buffer::name(std::string_view buffer_name)
    {
        _name = buffer_name;
        return *this;
    }

    Buffer& Buffer::type(BufferType buffer_type)  noexcept
    {
        _type = buffer_type;
        return *this;
    }

    void Buffer::validate()
    {
        if (_queues.empty())
            throw exception::InvalidState("[vk-buffer::builder] not queues");

        if (!_size)
            throw exception::InvalidState("[vk-buffer::builder] size is 0");

        if (_usage == VK_BUFFER_USAGE_FLAG_BITS_MAX_ENUM)
            throw exception::InvalidState("[vk-buffer::builder] invalid usage");
    }

    VkSharingMode Buffer::sharingMode() const
    {
        std::unordered_set<uint32_t> family_indices (std::cbegin(_queues), std::cend(_queues));
        return family_indices.size() == 1 ? VK_SHARING_MODE_EXCLUSIVE : VK_SHARING_MODE_CONCURRENT; 
    }

    vk::Buffer Buffer::build()
    {
        validate();

        vk::Buffer buffer (_device);

        buffer.size     = _size;
        buffer.type     = _type;
        buffer.usage    = _usage;

        const VkBufferCreateInfo buffer_info 
        { 
            .sType                  = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .size                   = _size,
            .usage                  = _usage,
            .sharingMode            = sharingMode(),
            .queueFamilyIndexCount  = static_cast<uint32_t>(_queues.size()),
            .pQueueFamilyIndices    = _queues.data()
        };

        VmaAllocationCreateInfo alloc_info 
        { 
            .usage = VMA_MEMORY_USAGE_AUTO
        };

        if (_type == BufferType::eDeviceOnly)
            alloc_info.flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;
        else if (_type == BufferType::eStaging)
            alloc_info.flags =  VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |  VMA_ALLOCATION_CREATE_MAPPED_BIT | VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;
        else 
            alloc_info.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT;

        VkBuffer        buffer_handle       = VK_NULL_HANDLE;
        VmaAllocation   allocation_handle   = VK_NULL_HANDLE;
        
        VK_CHECK(vmaCreateBuffer(
            _device.vmaAllocator(),
            &buffer_info,
            &alloc_info,
            &buffer_handle,
            &allocation_handle,
            nullptr
        ));

        buffer.handle = BufferHandle(buffer_handle, allocation_handle);

        if (!_name.empty())
        {
            const VkDebugUtilsObjectNameInfoEXT name_info 
            { 
                .sType          = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT,
                .objectType     = VK_OBJECT_TYPE_BUFFER,
                .objectHandle   = reinterpret_cast<uint64_t>(buffer.handle.handle()),
                .pObjectName    = _name.c_str()
            };

            _device.setName(name_info);
        }

        return buffer;
    }
}