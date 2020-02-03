//
//  DeviceQueue.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 03/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include "DeviceQueue.hpp"

#include "Device.hpp"

namespace pbrlib
{
    DeviceQueue::DeviceQueue(const shared_ptr<Device>& ptr_device, uint32_t family_index, uint32_t index) :
        _ptr_device(ptr_device),
        _queue_handle(VK_NULL_HANDLE),
        _family_index(family_index),
        _index(index)
    {
        vkGetDeviceQueue(_ptr_device->getHandle(), _family_index, _index, &_queue_handle);
        assert(_queue_handle != VK_NULL_HANDLE);
    }

    DeviceQueue::~DeviceQueue()
    {
        /// Деструктор должен завершиться только после того,
        /// как все команды в данной очереди будут выполнены.
        vkQueueWaitIdle(_queue_handle);
    }

    shared_ptr<Device>& DeviceQueue::getDevice() 
    {
        return _ptr_device;
    }

    const shared_ptr<Device>& DeviceQueue::getDevice() const
    {
        return _ptr_device;
    }

    VkQueue DeviceQueue::getHandle() const noexcept
    {
        return _queue_handle;
    }

    uint32_t DeviceQueue::getFamilyIndex() const noexcept
    {
        return _family_index;
    }

    uint32_t DeviceQueue::getIndex() const noexcept
    {
        return _index;
    }

    void DeviceQueue::waitIdle() const
    {
        vkQueueWaitIdle(_queue_handle);
    }
}
