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
        DeviceMemory            (ptr_device, size, memory_type_index),
        _buffer_handle          (VK_NULL_HANDLE),
        _usage                  (usage),
        _queue_family_indicies  (1)
    {
        _queue_family_indicies[0] = queue_family_index;
        
        VkBufferCreateInfo buffer_info {
            .sType                  = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .size                   = size,
            .usage                  = usage,
            .sharingMode            = VK_SHARING_MODE_EXCLUSIVE,
            .queueFamilyIndexCount  = 1,
            .pQueueFamilyIndices    = &queue_family_index
        };

        assert(vkCreateBuffer(
            _ptr_device->getDeviceHandle(), 
            &buffer_info, 
            nullptr, 
            &_buffer_handle
        ) == VK_SUCCESS);

        assert(_buffer_handle != VK_NULL_HANDLE);

        assert(vkBindBufferMemory(
            _ptr_device->getDeviceHandle(), 
            _buffer_handle, 
            _device_memory_handle, 
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
        DeviceMemory            (ptr_device, size, memory_type_index),
        _buffer_handle          (VK_NULL_HANDLE),
        _usage                  (usage),
        _queue_family_indicies  (queue_family_indices)
    {   
        VkBufferCreateInfo buffer_info {
            .sType                  = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .pNext                  = nullptr,
            .flags                  = 0,
            .size                   = size,
            .usage                  = usage,
            .sharingMode            = VK_SHARING_MODE_CONCURRENT,
            .queueFamilyIndexCount  = static_cast<uint32_t>(queue_family_indices.size()),
            .pQueueFamilyIndices    = queue_family_indices.data()
        };

        assert(vkCreateBuffer(
            _ptr_device->getDeviceHandle(), 
            &buffer_info, 
            nullptr, 
            &_buffer_handle
        ) == VK_SUCCESS);

        assert(_buffer_handle != VK_NULL_HANDLE);

        assert(vkBindBufferMemory(
            _ptr_device->getDeviceHandle(), 
            _buffer_handle, 
            _device_memory_handle, 
            0
        ) == VK_SUCCESS);
    }

    Buffer::Buffer(Buffer&& buffer) :
        DeviceMemory            (move(buffer)),
        _buffer_handle          (VK_NULL_HANDLE),
        _usage                  (buffer._usage),
        _queue_family_indicies  (move(buffer._queue_family_indicies))
    {
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
}
