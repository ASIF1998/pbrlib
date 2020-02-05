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

    enum class MapStatus
    {
        MAPPED,
        UNMAPPED
    };

    class DeviceMemory
    {
    public:
        DeviceMemory(const shared_ptr<Device>& ptr_device, VkDeviceSize size, uint32_t memory_type_index);
        ~DeviceMemory();

        /**
         * @brief Метод отображающий память устройства в адресное пространство CPU.
         * @details 
         *      Данный объект должен быть выделен из кучи, обладающий 
         *      флагом VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT.
        */
        void map();

        /**
         * @brief Метод завершающий отображение памяти.
        */
        void unmap();

        MapStatus isMapped() const noexcept;

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
