//
//  DeviceQueue.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 03/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef DeviceQueue_hpp
#define DeviceQueue_hpp

#include "Surface.hpp"

#include <memory>

using namespace std;

namespace pbrlib
{
    class Device;
    
    class DeviceQueue
    {
    public:
        /**
         * @brief Конструктор.
         * 
         * @param ptr_device указатель на устройство.
         * @param family_index индекс семейства очередей.
         * @param index индекс очереди в семействе очередей.
        */
        DeviceQueue(const shared_ptr<Device>& ptr_device, uint32_t family_index, uint32_t index);
        inline ~DeviceQueue();

        inline shared_ptr<Device>& getDevice();
        inline const shared_ptr<Device>& getDevice() const;
        inline VkQueue getQueueHandle() const noexcept;
        inline uint32_t getFamilyIndex() const noexcept;
        inline uint32_t getIndex() const noexcept;

        /**
         * @brief Метод ожидающий пока все команды в очереди не будут выполнены.
        */
        inline void waitIdle() const;

        inline static bool isPresentSuppoerted(uint32_t queue_family_index, const PhysicalDevice& physocal_device, const Surface& surface);

    private:
        shared_ptr<Device> _ptr_device;
        VkQueue _queue_handle;
        uint32_t _family_index;
        uint32_t _index;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inline DeviceQueue::~DeviceQueue()
    {
        /// Деструктор должен завершиться только после того,
        /// как все команды в данной очереди будут выполнены.
        vkQueueWaitIdle(_queue_handle);
    }

    inline shared_ptr<Device>& DeviceQueue::getDevice() 
    {
        return _ptr_device;
    }

    inline const shared_ptr<Device>& DeviceQueue::getDevice() const
    {
        return _ptr_device;
    }

    inline VkQueue DeviceQueue::getQueueHandle() const noexcept
    {
        return _queue_handle;
    }

    inline uint32_t DeviceQueue::getFamilyIndex() const noexcept
    {
        return _family_index;
    }

    inline uint32_t DeviceQueue::getIndex() const noexcept
    {
        return _index;
    }

    inline void DeviceQueue::waitIdle() const
    {
        vkQueueWaitIdle(_queue_handle);
    }

    inline bool DeviceQueue::isPresentSuppoerted(uint32_t queue_family_index, const PhysicalDevice& physocal_device, const Surface& surface)
    {
        VkBool32 res;

        vkGetPhysicalDeviceSurfaceSupportKHR(physocal_device.physical_device_handle, queue_family_index, surface.getSurfaceHandle(), &res);
        return res == VK_TRUE;
    }
}

#endif /* DeviceQueue_hpp */
