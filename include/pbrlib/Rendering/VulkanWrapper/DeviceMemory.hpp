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

namespace pbrlib
{
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
        DeviceMemory(const Device* ptr_device) noexcept;

        /**
         * @brief Конструктор.
         * 
         * @param ptr_device        указатель на устройство.
         * @param size              размер требуемой памяти.
         * @param memory_type_index индекс типа памяти.
        */
        DeviceMemory(const Device* ptr_device, VkDeviceSize size, uint32_t memory_type_index);

        DeviceMemory(DeviceMemory&& device_memory);
        DeviceMemory(const DeviceMemory&) = delete;

        ~DeviceMemory();

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

        template<typename Pointer>
        inline void setData(const Pointer& ptr, size_t size);

        MapStatus getMapStatus() const noexcept;

        uint8_t*                getData()               noexcept;
        const uint8_t*          getData()               const noexcept;
        const VkDeviceMemory&   getDeviceMemoryHandle() const noexcept;
        const Device*           getDevice()             const noexcept;
        VkDeviceSize            getSize()               const noexcept;

        static std::unique_ptr<DeviceMemory> make(const Device* ptr_device);
        static std::unique_ptr<DeviceMemory> make(const Device* ptr_device, VkDeviceSize size, uint32_t memory_type_index);

    protected:
        const Device*   _ptr_device;
        VkDeviceMemory  _device_memory_handle;
        VkDeviceSize    _memory_size;
        uint8_t*        _ptr_mapped_data;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

    template<typename Pointer>
    inline void DeviceMemory::setData(const Pointer& ptr, size_t size)
    {
        if (ptr) {
            MapStatus is_memory_mapped = getMapStatus();

            if (is_memory_mapped == MapStatus::UNMAPPED) {
                map();
            }
            
            memcpy(_ptr_mapped_data, ptr, sizeof(ptr[0]) * size);

            if (is_memory_mapped == MapStatus::UNMAPPED) {
                unmap();
            }
        }
    }
}

#endif /* DeviceMemory_hpp */