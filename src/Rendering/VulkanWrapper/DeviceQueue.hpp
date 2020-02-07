//
//  DeviceQueue.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 03/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef DeviceQueue_hpp
#define DeviceQueue_hpp

#include <vulkan/vulkan.h>

#include <memory>

using namespace std;

namespace pbrlib
{
    class Device;

    /**
     * TODO:
     *      1) Добавить метод позволяющий узнать поддержку показа семейством очередей;
     *      2) Добавить метод помещающий в очередь команду показа.
    */

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
}

#endif /* DeviceQueue_hpp */
