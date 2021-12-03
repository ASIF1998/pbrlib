//
//  Buffer.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 05/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include "Buffer.hpp"

#include <cassert>

namespace pbrlib
{
    Buffer::Buffer(
        const PtrDevice&    ptr_device, 
        VkDeviceSize        size, 
        VkBufferUsageFlags  usage, 
        uint32_t            memory_type_index, 
        uint32_t            queue_family_index
    ) :
        _buffer_handle          (VK_NULL_HANDLE),
        _usage                  (usage),
        _queue_family_indicies  (1),
        _ptr_device             (ptr_device),
        _ptr_device_memory      (nullptr),
        _size                   (size)
    {
        _queue_family_indicies[0] = queue_family_index;

        VkBufferCreateInfo buffer_info = { };
        buffer_info.sType                   = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        buffer_info.size                    = _size;
        buffer_info.usage                   = usage;
        buffer_info.sharingMode             = VK_SHARING_MODE_EXCLUSIVE;
        buffer_info.queueFamilyIndexCount   = 1;
        buffer_info.pQueueFamilyIndices     = &queue_family_index;

        assert(vkCreateBuffer(
            _ptr_device->getDeviceHandle(), 
            &buffer_info, 
            nullptr, 
            &_buffer_handle
        ) == VK_SUCCESS);

        assert(_buffer_handle != VK_NULL_HANDLE);

        VkMemoryRequirements buffer_mr = { };
        vkGetBufferMemoryRequirements(_ptr_device->getDeviceHandle(), _buffer_handle, &buffer_mr);

        _ptr_device_memory = DeviceMemory::make(_ptr_device, buffer_mr.size, memory_type_index);

        assert(vkBindBufferMemory(
            _ptr_device->getDeviceHandle(), 
            _buffer_handle, 
            _ptr_device_memory->getDeviceMemoryHandle(),
            0
        ) == VK_SUCCESS);
    }

    Buffer::Buffer(
        const PtrDevice&    ptr_device, 
        VkDeviceSize        size, 
        VkBufferUsageFlags  usage, 
        uint32_t            memory_type_index, 
        vector<uint32_t>    queue_family_indices
    ) :
        _buffer_handle          (VK_NULL_HANDLE),
        _usage                  (usage),
        _queue_family_indicies  (queue_family_indices),
        _ptr_device             (ptr_device),
        _ptr_device_memory      (nullptr),
        _size                   (size)
    {
        VkBufferCreateInfo buffer_info = { };
        buffer_info.sType                   = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        buffer_info.size                    = _size;
        buffer_info.usage                   = usage;
        buffer_info.sharingMode             = VK_SHARING_MODE_CONCURRENT;
        buffer_info.queueFamilyIndexCount   = static_cast<uint32_t>(queue_family_indices.size());
        buffer_info.pQueueFamilyIndices     = queue_family_indices.data();

        assert(vkCreateBuffer(
            _ptr_device->getDeviceHandle(), 
            &buffer_info, 
            nullptr, 
            &_buffer_handle
        ) == VK_SUCCESS);

        assert(_buffer_handle != VK_NULL_HANDLE);

        VkMemoryRequirements buffer_mr = { };
        vkGetBufferMemoryRequirements(_ptr_device->getDeviceHandle(), _buffer_handle, &buffer_mr);

        _ptr_device_memory = DeviceMemory::make(_ptr_device, buffer_mr.size, memory_type_index);

        assert(vkBindBufferMemory(
            _ptr_device->getDeviceHandle(), 
            _buffer_handle, 
            _ptr_device_memory->getDeviceMemoryHandle(),
            0
        ) == VK_SUCCESS);
    }

    Buffer::Buffer(Buffer&& buffer) :
        _buffer_handle          (VK_NULL_HANDLE),
        _usage                  (buffer._usage),
        _queue_family_indicies  (move(buffer._queue_family_indicies)),
        _ptr_device             (buffer._ptr_device),
        _ptr_device_memory      (nullptr),
        _size                   (buffer._size)
    {
        swap(_ptr_device_memory, buffer._ptr_device_memory);
        swap(_buffer_handle, buffer._buffer_handle);
    }

    Buffer::~Buffer()
    {
        if (_buffer_handle != VK_NULL_HANDLE) {
            vkDestroyBuffer(_ptr_device->getDeviceHandle(), _buffer_handle, nullptr);
        }
    }

    const VkBuffer& Buffer::getBufferHandle() const noexcept
    {
        return _buffer_handle;
    }

    PtrDevice& Buffer::getDevice() noexcept
    {
        return _ptr_device;
    }

    const PtrDevice& Buffer::getDevice() const noexcept
    {
        return _ptr_device;
    }

    PtrDeviceMemory& Buffer::getDeviceMemory() noexcept
    {
        return _ptr_device_memory;
    }

    const PtrDeviceMemory& Buffer::getDeviceMemory() const noexcept
    {
        return _ptr_device_memory;
    }

    VkDeviceSize Buffer::getSize() const noexcept
    {
        return _size;
    }
}
