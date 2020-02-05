//
//  Buffer.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 05/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include "Buffer.hpp"
#include "Device.hpp"

namespace pbrlib
{
    Buffer::Buffer(const shared_ptr<Device>& ptr_device, 
                   VkDeviceSize size, 
                   VkBufferUsageFlags usage, 
                   uint32_t memory_type_index, 
                   uint32_t queue_family_index) :
        DeviceMemory(ptr_device, size, memory_type_index),
        _buffer_handle(VK_NULL_HANDLE),
        _usage(usage),
        _queue_family_indicies(1)
    {
        _queue_family_indicies.push_back(queue_family_index);
        
        VkBufferCreateInfo buffer_info {
            .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .size = size,
            .usage = usage,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
            .queueFamilyIndexCount = 1,
            .pQueueFamilyIndices = &queue_family_index
        };

        assert(vkCreateBuffer(_ptr_device->getDeviceHandle(), &buffer_info, nullptr, &_buffer_handle) == VK_SUCCESS);
        assert(_buffer_handle != VK_NULL_HANDLE);
    }

    Buffer::Buffer(const shared_ptr<Device>& ptr_device, 
                   VkDeviceSize size, 
                   VkBufferUsageFlags usage, 
                   uint32_t memory_type_index, 
                   vector<uint32_t> queue_family_indicies) :
        DeviceMemory(ptr_device, size, memory_type_index),
        _buffer_handle(VK_NULL_HANDLE),
        _usage(usage),
        _queue_family_indicies(queue_family_indicies)
    {   
        VkBufferCreateInfo buffer_info {
            .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .size = size,
            .usage = usage,
            .sharingMode = VK_SHARING_MODE_CONCURRENT,
            .queueFamilyIndexCount = static_cast<uint32_t>(_queue_family_indicies.size()),
            .pQueueFamilyIndices = _queue_family_indicies.data()
        };

        assert(vkCreateBuffer(_ptr_device->getDeviceHandle(), &buffer_info, nullptr, &_buffer_handle) == VK_SUCCESS);
        assert(_buffer_handle != VK_NULL_HANDLE);
    }

    Buffer::~Buffer()
    {
        vkDestroyBuffer(_ptr_device->getDeviceHandle(), _buffer_handle, nullptr);
    }

    VkBuffer Buffer::getBufferHandle() const noexcept
    {
        return _buffer_handle;
    }
}
