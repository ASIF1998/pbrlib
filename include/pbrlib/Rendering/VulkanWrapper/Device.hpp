//
//  Device.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 02/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef Device_hpp
#define Device_hpp

#include <vulkan/vulkan.h>

#include <vector>
#include <span>
#include <set>
#include <memory>

namespace pbrlib
{
    struct  PhysicalDevice;
    class   Instance;
}

namespace pbrlib
{
    class Device
    {
    public:
        /**
         * @brief Конструктор.
         * 
         * @param ptr_instance      указатель на экземпляр Vulkan'а.
         * @param physical_device   физическое устройство.
         * @param queue_infos       информация о создаваемых очередях логического устройства.
        */
        Device(
            const Instance*                             ptr_instance,
            const PhysicalDevice&                       physical_device, 
            std::span<const VkDeviceQueueCreateInfo>    queue_infos
        );

        /**
         * @brief Конструктор.
         * 
         * @param ptr_instance      указатель на экземпляр Vulkan'а
         * @param physical_device   физическое устройство.
         * @param queue_infos       информация о создаваемых очередях логического устройства.
         * @param layer_names       названия слоёв.
         * @param extension_names   названия расширений.
        */
        Device(
            const Instance*                             ptr_instance,
            const PhysicalDevice&                       physical_device, 
            std::span<const VkDeviceQueueCreateInfo>    queue_infos, 
            std::span<const char*>                      layer_names, 
            std::span<const char*>                      extension_names
        );

        Device(Device&& device);
        Device(const Device&) = delete;

        ~Device();

        Device& operator = (Device&&)       = delete;
        Device& operator = (const Device&)  = delete;

        const VkDevice&                             getDeviceHandle()       const noexcept;
        const std::vector<VkDeviceQueueCreateInfo>& getDeviceQueueInfo()    const noexcept;
        const Instance*                             getInstance()           const noexcept;

        /**
         * @brief Метод необходимый для ожидания завершения всех очередей на устройстве.
        */
        void waitIdle() const;

    private:
        void _create(
            const Instance*                             ptr_instance,
            const PhysicalDevice&                       physical_device,
            std::span<const VkDeviceQueueCreateInfo>    queue_infos,
            uint32_t                                    enabled_layer_count,
            const char* const*                          ptr_enabled_layers,
            uint32_t                                    enabled_extension_count,
            const char* const*                          ptr_extensions
        );

    private:
        const Instance*                         _ptr_instance;
        VkDevice                                _device_handle; //!< Дескриптор логического устройства.
        std::vector<VkDeviceQueueCreateInfo>    _queues_infos;  //!< Информация об используемых очередях устройства.
    };
}

#endif /* Device_hpp */
