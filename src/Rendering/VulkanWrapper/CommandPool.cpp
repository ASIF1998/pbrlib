//
//  CommandPool.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 04/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include "CommandPool.hpp"

#include "Device.hpp"

namespace pbrlib
{
    CommandPool::CommandPool(const shared_ptr<Device>& ptr_device, uint32_t queue_family_index) :
        _ptr_device(ptr_device),
        _command_pool_handle(VK_NULL_HANDLE),
        _queue_family_index(queue_family_index)
    {
        VkCommandPoolCreateInfo command_pool_info {
            .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .queueFamilyIndex = _queue_family_index
        };
        
        assert(vkCreateCommandPool(_ptr_device->getHandle(), &command_pool_info, nullptr, &_command_pool_handle) == VK_SUCCESS);
        assert(_command_pool_handle != VK_NULL_HANDLE);
    }

    CommandPool::~CommandPool()
    {
        vkDestroyCommandPool(_ptr_device->getHandle(), _command_pool_handle, nullptr);
    }

    shared_ptr<Device>& CommandPool::getDevice() noexcept
    {
        return _ptr_device;
    }

    const shared_ptr<Device>& CommandPool::getDevice() const noexcept
    {
        return _ptr_device;
    }

    uint32_t CommandPool::getFamilyIndex() const noexcept
    {
        return _queue_family_index;
    }

    VkCommandPool CommandPool::getHandle() const noexcept
    {
        return _command_pool_handle;
    }
}
