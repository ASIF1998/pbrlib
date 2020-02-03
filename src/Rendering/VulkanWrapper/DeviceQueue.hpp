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
         * @param device указатель на устройство.
         * @param family_index индекс семейства очередей.
         * @param index индекс очереди в семействе очередей.
        */
        DeviceQueue(const shared_ptr<Device>& device, uint32_t family_index, uint32_t index);
        ~DeviceQueue();

        shared_ptr<Device>& getDevice();
        const shared_ptr<Device>& getDevice() const;
        VkQueue getHandle() const noexcept;
        uint32_t getFamilyIndex() const noexcept;
        uint32_t getIndex() const noexcept;
        VkQueueFamilyProperties getFamilyProperties() const noexcept;

        /**
         * @brief Метод ожидающий пока все команды в очереди не будут выполнены.
        */
        void waitIdle() const;

    private:
        shared_ptr<Device> _ptr_device;
        VkQueue _queue_handle;
        uint32_t _family_index;
        uint32_t _index;
    };
}

#endif /* DeviceQueue_hpp */
