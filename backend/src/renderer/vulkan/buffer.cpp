#include <backend/renderer/vulkan/buffer.hpp>
#include <backend/renderer/vulkan/device.hpp>

#include <backend/utils/vulkan.hpp>

#include <unordered_set>

namespace pbrlib::vk
{
    Buffer::Buffer(Device* ptr_device) :
        _ptr_device (ptr_device)
    { }

    Buffer::Buffer(Buffer&& buffer) :
        _ptr_device (buffer._ptr_device)
    {
        std::swap(handle, buffer.handle);
        std::swap(_allocation, buffer._allocation);
        std::swap(size, buffer.size);
    }

    Buffer::~Buffer()
    {
        if (handle != VK_NULL_HANDLE)
            vmaDestroyBuffer(_ptr_device->vmaAllocator(), handle, _allocation);
    }

    Buffer& Buffer::operator = (Buffer&& buffer)
    {
        _ptr_device = buffer._ptr_device;

        std::swap(handle, buffer.handle);
        std::swap(_allocation, buffer._allocation);
        std::swap(size, buffer.size);

        return *this;
    }

    void Buffer::write(const uint8_t* ptr_data, size_t size, VkDeviceSize offset)
    {
        auto temp_buffer = Buffer::Builder(_ptr_device)
            .size(size)
            .usage(VK_BUFFER_USAGE_TRANSFER_SRC_BIT)
            .addQueueFamilyIndex(_ptr_device->queue().family_index)
            .build();

        void* ptr_temp_buffer_memory = nullptr;

        VK_CHECK(
            vmaMapMemory(
                _ptr_device->vmaAllocator(),
                temp_buffer._allocation,
                &ptr_temp_buffer_memory
            )
        );

        memcpy(ptr_temp_buffer_memory, ptr_data, size);

        vmaUnmapMemory(_ptr_device->vmaAllocator(), temp_buffer._allocation);

        auto command_buffer = _ptr_device->oneTimeSubmitCommandBuffer(_ptr_device->queue(), "Copy data");
        command_buffer.write([offset, size, &temp_buffer, this](VkCommandBuffer command_buffer_handle)
        {
            VkBufferCopy copy = { };
            copy.dstOffset  = offset;
            copy.srcOffset  = 0;
            copy.size       = static_cast<VkDeviceSize>(size);

            vkCmdCopyBuffer(command_buffer_handle, temp_buffer.handle, handle, 1, &copy);
        });

        VkCommandBufferSubmitInfo buffer_submit_info = { };
        buffer_submit_info.sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO;
        buffer_submit_info.commandBuffer    = command_buffer.handle;

        VkSubmitInfo2 submit_info = { };
        submit_info.sType                   = VK_STRUCTURE_TYPE_SUBMIT_INFO_2;
        submit_info.pCommandBufferInfos     = &buffer_submit_info;
        submit_info.commandBufferInfoCount  = 1;

        VK_CHECK(
            vkQueueSubmit2(
                _ptr_device->queue().handle,
                1, &submit_info,
                VK_NULL_HANDLE
            )
        );

        VK_CHECK(vkDeviceWaitIdle(_ptr_device->device()));
    }
}

namespace pbrlib::vk
{
    Buffer::Builder::Builder(Device* ptr_device) :
        _ptr_device(ptr_device)
    { }

    Buffer::Builder& Buffer::Builder::addQueueFamilyIndex(uint32_t index)
    {
        _queues.push_back(index);
        return *this;
    }

    Buffer::Builder& Buffer::Builder::size(VkDeviceSize size) noexcept
    {
        _size = size;
        return *this;
    }

    Buffer::Builder& Buffer::Builder::usage(VkImageUsageFlags usage) noexcept
    {
        _usage = usage;
        return *this;
    }

    Buffer::Builder& Buffer::Builder::name(std::string_view buffer_name)
    {
        _name = buffer_name;
        return *this;
    }

    void Buffer::Builder::validate()
    {
        if (!_ptr_device)
            throw std::runtime_error("[Buffer::Builder] Not device");

        if (_queues.empty())
            throw std::runtime_error("[Buffer::Builder] Not queues");

        if (!_size)
            throw std::runtime_error("[Buffer::Builder] Size is 0");

        if (_usage == VK_BUFFER_USAGE_FLAG_BITS_MAX_ENUM)
            throw std::runtime_error("[Buffer::Builder] Invalid usage");
    }

    VkSharingMode Buffer::Builder::sharingMode() const
    {
        std::unordered_set<uint32_t> family_indices (std::begin(_queues), std::end(_queues));
        return family_indices.size() == 1 ? VK_SHARING_MODE_EXCLUSIVE : VK_SHARING_MODE_CONCURRENT; 
    }

    Buffer Buffer::Builder::build()
    {
        validate();

        Buffer buffer (_ptr_device);

        buffer.size = _size;

        VkBufferCreateInfo buffer_info = { };
        buffer_info.sType                   = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        buffer_info.pQueueFamilyIndices     = _queues.data();
        buffer_info.queueFamilyIndexCount   = static_cast<uint32_t>(_queues.size());
        buffer_info.size                    = _size;
        buffer_info.usage                   = _usage;
        buffer_info.sharingMode             = sharingMode();

        VmaAllocationCreateInfo alloc_info = { };
        alloc_info.usage    = VMA_MEMORY_USAGE_AUTO;
        alloc_info.flags    = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;
        alloc_info.priority = 1.0f;

        VK_CHECK(vmaCreateBuffer(
            _ptr_device->vmaAllocator(),
            &buffer_info,
            &alloc_info,
            &buffer.handle,
            &buffer._allocation,
            nullptr
        ));

        if (!_name.empty())
        {
            VkDebugUtilsObjectNameInfoEXT name_info = { };
            name_info.sType         = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
            name_info.objectHandle  = reinterpret_cast<uint64_t>(buffer.handle);
            name_info.pObjectName   = _name.c_str();
            name_info.objectType    = VK_OBJECT_TYPE_BUFFER;

            _ptr_device->setName(name_info);
        }

        return buffer;
    }
}