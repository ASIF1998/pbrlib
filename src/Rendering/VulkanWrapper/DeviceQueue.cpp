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
    DeviceQueue::DeviceQueue(
        const PtrDevice&            ptr_device, 
        uint32_t                    family_index, 
        uint32_t                    index
    ) :
        _ptr_device     (ptr_device),
        _queue_handle   (VK_NULL_HANDLE),
        _family_index   (family_index),
        _index          (index)
    {
        vkGetDeviceQueue(_ptr_device->getDeviceHandle(), _family_index, _index, &_queue_handle);
        assert(_queue_handle != VK_NULL_HANDLE);
    }
}
