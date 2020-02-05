//
//  Memory.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 05/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef DeviceMemory_hpp
#define DeviceMemory_hpp

#include <vulkan/vulkan.h>

#include <memory>

using namespace std;

namespace pbrlib
{
    class Device;

    class DeviceMemory
    {
    public:
        DeviceMemory(const shared_ptr<Device>& ptr_device, VkDeviceSize size, uint32_t memory_type_index);
        ~DeviceMemory();

        void map();
        void unmap();
        bool isMapped() const noexcept;

        uint8_t* getData() noexcept;
        const uint8_t* getData() const noexcept;
        VkDeviceMemory getHandle() const noexcept;
        shared_ptr<Device>& getDevice() noexcept;
        const shared_ptr<Device>& getDevice() const noexcept;

    protected:
        shared_ptr<Device> _ptr_device;
        VkDeviceMemory _device_memory_handle;
        VkDeviceSize _memory_size;
        uint8_t* _ptr_mapped_data;
    };
}

#endif /* DeviceMemory_hpp */
