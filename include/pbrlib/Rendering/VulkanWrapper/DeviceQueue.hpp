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
#include "Swapchain.hpp"

#include <memory>

using namespace std;

namespace pbrlib
{
    class Device;
    class CommandBuffer;
    class DeviceQueue;

    using PtrDeviceQueue = shared_ptr<DeviceQueue>;
    
    class DeviceQueue
    {
    public:
        /**
         * @brief Конструктор.
         * 
         * @param ptr_device    указатель на устройство.
         * @param family_index  индекс семейства очередей.
         * @param index         индекс очереди в семействе очередей.
        */
        DeviceQueue(const PtrDevice& ptr_device, uint32_t family_index, uint32_t index);

        DeviceQueue(DeviceQueue&& device_queue);
        DeviceQueue(const DeviceQueue&) = delete;

        ~DeviceQueue();

        DeviceQueue& operator = (DeviceQueue&&)         = delete;
        DeviceQueue& operator = (const DeviceQueue&)    = delete;

        void submit(const CommandBuffer& command_buffer);

        PtrDevice&           getDevice()         noexcept;
        const PtrDevice&     getDevice()         const noexcept; 
        const VkQueue&       getQueueHandle()    const noexcept;
        uint32_t             getFamilyIndex()    const noexcept;
        uint32_t             getIndex()          const noexcept;

        /**
         * @brief Метод позволяющий показать пользователю изображение.
         * 
         * @param ptr_swapchain указатель на список показа.
         * @param image_index   индекс изображения в списке показа.
        */
        void setPresent(
            const PtrSwapchain& ptr_swapchain,
            uint32_t            image_index
        );

        /**
         * @brief Метод позволяющий показать пользователю изображение.
         * 
         * @param swapchain     список показа.
         * @param image_index   индекс изображения в списке показа.
        */
        void setPresent(
            const Swapchain&    swapchain,
            uint32_t            image_index
        );

        /**
         * @brief Метод ожидающий пока все команды в очереди не будут выполнены.
        */
        void waitIdle() const;

        static bool isPresentSuppoerted(
            uint32_t                queue_family_index, 
            const PhysicalDevice&   physical_device, 
            const Surface&          surface
        );

        /**
         * @brief Статический метод, необходимый для создания указателя на очередь устройства.
         * 
         * @param ptr_device    указатель на устройство.
         * @param family_index  индекс семейства очередей.
         * @param index         индекс очереди в семействе очередей.
        */
        static PtrDeviceQueue make(const PtrDevice& ptr_device, uint32_t family_index, uint32_t index);

    private:
        PtrDevice   _ptr_device;
        VkQueue     _queue_handle;
        uint32_t    _family_index;
        uint32_t    _index;
    };
}

#endif /* DeviceQueue_hpp */
