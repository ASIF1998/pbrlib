//
//  Memory.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 05/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <pbrlib/Rendering/VulkanWrapper/DeviceMemory.hpp>

#include <cassert>

namespace pbrlib
{
    DeviceMemory::DeviceMemory(const PtrDevice& ptr_device) noexcept :
        _ptr_device             (ptr_device),
        _device_memory_handle   (VK_NULL_HANDLE),
        _memory_size            (0),
        _ptr_mapped_data        (nullptr)
    {}

    DeviceMemory::DeviceMemory(const PtrDevice& ptr_device, VkDeviceSize size, uint32_t memory_type_index) :
        _ptr_device             (ptr_device),
        _device_memory_handle   (VK_NULL_HANDLE),
        _memory_size            (size),
        _ptr_mapped_data        (nullptr)
    {
        VkMemoryAllocateInfo alloc_info = { };
        alloc_info.sType            = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        alloc_info.allocationSize   = _memory_size;
        alloc_info.memoryTypeIndex  = memory_type_index;

        assert(vkAllocateMemory(
            _ptr_device->getDeviceHandle(),
            &alloc_info,
            nullptr,
            &_device_memory_handle
        ) == VK_SUCCESS);

        assert(_device_memory_handle != VK_NULL_HANDLE);
    }

    DeviceMemory::DeviceMemory(DeviceMemory&& device_memory) :
        _ptr_device             (move(device_memory._ptr_device)),
        _device_memory_handle   (VK_NULL_HANDLE),
        _memory_size            (device_memory._memory_size),
        _ptr_mapped_data        (device_memory._ptr_mapped_data)
    {
        std::swap(_device_memory_handle, device_memory._device_memory_handle);
    }

    DeviceMemory::~DeviceMemory()
    {
        vkFreeMemory(_ptr_device->getDeviceHandle(), _device_memory_handle, nullptr);
    }

    void DeviceMemory::map()
    {
        if (!_ptr_mapped_data && _device_memory_handle != VK_NULL_HANDLE) {
            assert(vkMapMemory(
                _ptr_device->getDeviceHandle(),
                _device_memory_handle,
                0,
                _memory_size,
                0,
                reinterpret_cast<void**>(&_ptr_mapped_data)
            ) == VK_SUCCESS);

            assert(_ptr_mapped_data);
        }
    }

    void DeviceMemory::unmap()
    {
        if (_ptr_mapped_data) {
            vkUnmapMemory(_ptr_device->getDeviceHandle(), _device_memory_handle);
            _ptr_mapped_data = nullptr;
        }
    }

    MapStatus DeviceMemory::getMapStatus() const noexcept
    {
        return (_ptr_mapped_data ? MapStatus::MAPPED : MapStatus::UNMAPPED);
    }

    uint8_t* DeviceMemory::getData() noexcept
    {
        return _ptr_mapped_data;
    }

    const uint8_t* DeviceMemory::getData() const noexcept
    {
        return _ptr_mapped_data;
    }

    const VkDeviceMemory& DeviceMemory::getDeviceMemoryHandle() const noexcept
    {
        return _device_memory_handle;
    }

    PtrDevice& DeviceMemory::getDevice() noexcept
    {
        return _ptr_device;
    }

    const PtrDevice& DeviceMemory::getDevice() const noexcept
    {
        return _ptr_device;
    }

    VkDeviceSize DeviceMemory::getSize() const noexcept
    {
        return _memory_size;
    }

    PtrDeviceMemory DeviceMemory::make(const PtrDevice& ptr_device)
    {
        return make_unique<DeviceMemory>(ptr_device);
    }

    PtrDeviceMemory DeviceMemory::make(const PtrDevice& ptr_device, VkDeviceSize size, uint32_t memory_type_index)
    {
        return make_unique<DeviceMemory>(ptr_device, size, memory_type_index);
    }
}
