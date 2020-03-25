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
        inline DeviceMemory(const PtrDevice& ptr_device) noexcept;

        /**
         * @brief Конструктор.
         * 
         * @param ptr_device        указатель на устройство.
         * @param size              размер требуемой памяти.
         * @param memory_type_index индекс типа памяти.
        */
        DeviceMemory(const PtrDevice& ptr_device, VkDeviceSize size, uint32_t memory_type_index);

        inline DeviceMemory(DeviceMemory&& device_memory);
        DeviceMemory(const DeviceMemory&) = delete;

        inline ~DeviceMemory();

        DeviceMemory& operator = (DeviceMemory&&)       = delete;
        DeviceMemory& operator = (const DeviceMemory&)  = delete;

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

        /**
         * @brief Метод позволяющий записать данные в память.
         * 
         * @param data данные которые нужно записать в память.
        */
        template<typename Container>
        inline void setData(const Container& data);

        inline MapStatus getMapStatus() const noexcept;

        inline uint8_t*                 getData()               noexcept;
        inline const uint8_t*           getData()               const noexcept;
        inline const VkDeviceMemory&    getDeviceMemoryHandle() const noexcept;
        inline PtrDevice&               getDevice()             noexcept;
        inline const PtrDevice&         getDevice()             const noexcept;

    protected:
        PtrDevice       _ptr_device;
        VkDeviceMemory  _device_memory_handle;
        VkDeviceSize    _memory_size;
        uint8_t*        _ptr_mapped_data;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inline DeviceMemory::DeviceMemory(const PtrDevice& ptr_device) noexcept :
        _ptr_device             (ptr_device),
        _device_memory_handle   (VK_NULL_HANDLE),
        _memory_size            (0),
        _ptr_mapped_data        (nullptr)
    {}

    inline DeviceMemory::DeviceMemory(DeviceMemory&& device_memory) :
        _ptr_device             (device_memory._ptr_device),
        _device_memory_handle   (VK_NULL_HANDLE),
        _memory_size            (device_memory._memory_size),
        _ptr_mapped_data        (device_memory._ptr_mapped_data)
    {
        swap(_device_memory_handle, device_memory._device_memory_handle);
    }

    inline DeviceMemory::~DeviceMemory()
    {
        vkFreeMemory(_ptr_device->getDeviceHandle(), _device_memory_handle, nullptr);
    }

    inline MapStatus DeviceMemory::getMapStatus() const noexcept
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

    inline const VkDeviceMemory& DeviceMemory::getDeviceMemoryHandle() const noexcept
    {
        return _device_memory_handle;
    }

    inline PtrDevice& DeviceMemory::getDevice() noexcept
    {
        return _ptr_device;
    }

    inline const PtrDevice& DeviceMemory::getDevice() const noexcept
    {
        return _ptr_device;
    }

    template<typename Container>
    inline void DeviceMemory::setData(const Container& data)
    {
        MapStatus is_memory_mapped = getMapStatus();

        if (is_memory_mapped == MapStatus::UNMAPPED) {
            map();
        }

        memcpy(_ptr_mapped_data, &data[0], sizeof(data[0]) * data.size());

        if (is_memory_mapped == MapStatus::UNMAPPED) {
            unmap();
        }
    }
}

#endif /* DeviceMemory_hpp */
