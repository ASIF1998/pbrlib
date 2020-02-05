//
//  Memory.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 05/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include "DeviceMemory.hpp"
#include "Device.hpp"

namespace pbrlib
{
    DeviceMemory::DeviceMemory(const shared_ptr<Device>& ptr_device, VkDeviceSize size, uint32_t memory_type_index) :
        _ptr_device(ptr_device),
        _device_memory_handle(VK_NULL_HANDLE),
        _memory_size(size),
        _ptr_mapped_data(nullptr)
    {
        VkMemoryAllocateInfo alloc_info {
            .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .pNext = nullptr,
            .allocationSize = _memory_size,
            .memoryTypeIndex = memory_type_index
        };

        assert(vkAllocateMemory(_ptr_device->getHandle(), &alloc_info, nullptr, &_device_memory_handle) == VK_SUCCESS);
        assert(_device_memory_handle != VK_NULL_HANDLE);
    }

    DeviceMemory::~DeviceMemory()
    {
        vkFreeMemory(_ptr_device->getHandle(), _device_memory_handle, nullptr);
    }

    void DeviceMemory::map()
    {
        if (!_ptr_mapped_data) {
            assert(vkMapMemory(_ptr_device->getHandle(), 
                               _device_memory_handle, 
                               0, 
                               _memory_size, 
                               0, 
                               reinterpret_cast<void**>( &_ptr_mapped_data)) == VK_SUCCESS);

            assert(_ptr_mapped_data);
        }
    }

    void DeviceMemory::unmap()
    {
        if (_ptr_mapped_data) {
            vkUnmapMemory(_ptr_device->getHandle(), _device_memory_handle);
            _ptr_mapped_data = nullptr;
        }
    }

    bool DeviceMemory::isMapped() const noexcept
    {
        return _ptr_mapped_data;
    }

    uint8_t* DeviceMemory::getData() noexcept
    {
        return _ptr_mapped_data;
    }

    const uint8_t* DeviceMemory::getData() const noexcept
    {
        return _ptr_mapped_data;
    }

    VkDeviceMemory DeviceMemory::getHandle() const noexcept
    {
        return _device_memory_handle;
    }

    shared_ptr<Device>& DeviceMemory::getDevice() noexcept
    {
        return _ptr_device;
    }

    const shared_ptr<Device>& DeviceMemory::getDevice() const noexcept
    {
        return _ptr_device;
    }
}
