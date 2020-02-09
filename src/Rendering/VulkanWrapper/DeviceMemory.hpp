//
//  Memory.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 05/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef DeviceMemory_hpp
#define DeviceMemory_hpp

#include <memory>

#include "Device.hpp"

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
        /**
         * @brief Конструктор.
         * 
         * @param ptr_device указатель на устройство.
        */
        inline DeviceMemory(const shared_ptr<Device>& ptr_device) noexcept;

        /**
         * @brief Конструктор.
         * 
         * @param ptr_device указатель на устройство.
         * @param size размер требуемой памяти.
         * @param memory_type_index индекс типа памяти.
        */
        DeviceMemory(const shared_ptr<Device>& ptr_device, VkDeviceSize size, uint32_t memory_type_index);
        inline ~DeviceMemory();

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

        inline MapStatus isMapped() const noexcept;

        inline uint8_t* getData() noexcept;
        inline const uint8_t* getData() const noexcept;
        inline VkDeviceMemory getDeviceMemoryHandle() const noexcept;
        inline shared_ptr<Device>& getDevice() noexcept;
        inline const shared_ptr<Device>& getDevice() const noexcept;

    protected:
        shared_ptr<Device> _ptr_device;
        VkDeviceMemory _device_memory_handle;
        VkDeviceSize _memory_size;
        uint8_t* _ptr_mapped_data;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inline DeviceMemory::DeviceMemory(const shared_ptr<Device>& ptr_device) noexcept :
        _ptr_device(ptr_device),
        _device_memory_handle(VK_NULL_HANDLE),
        _memory_size(0),
        _ptr_mapped_data(nullptr)
    {}

    inline DeviceMemory::~DeviceMemory()
    {
        vkFreeMemory(_ptr_device->getDeviceHandle(), _device_memory_handle, nullptr);
    }

    inline MapStatus DeviceMemory::isMapped() const noexcept
    {
        return (_ptr_mapped_data ? MapStatus::MAPPED : MapStatus::UNMAPPED);
    }

    inline uint8_t* DeviceMemory::getData() noexcept
    {
        return _ptr_mapped_data;
    }

    inline const uint8_t* DeviceMemory::getData() const noexcept
    {
        return _ptr_mapped_data;
    }

    inline VkDeviceMemory DeviceMemory::getDeviceMemoryHandle() const noexcept
    {
        return _device_memory_handle;
    }

    inline shared_ptr<Device>& DeviceMemory::getDevice() noexcept
    {
        return _ptr_device;
    }

    inline const shared_ptr<Device>& DeviceMemory::getDevice() const noexcept
    {
        return _ptr_device;
    }
}

#endif /* DeviceMemory_hpp */
