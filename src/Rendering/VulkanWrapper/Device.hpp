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
#include <set>

#include <memory>

using namespace std;

namespace pbrlib
{
    struct  PhysicalDevice;
    class   Device;

    using PtrDevice = shared_ptr<Device>;

    class Device
    {
    public:
        /**
         * Конструктор.
         * 
         * @param physical_device   физическое устройство.
         * @param queue_info        информация о создаваемых очередях логического устройства.
        */
        Device(const PhysicalDevice& physical_device, const vector<VkDeviceQueueCreateInfo>& queue_info);

        /**
         * Конструктор.
         * 
         * @param physical_device   физическое устройство.
         * @param queue_info        информация о создаваемых очередях логического устройства.
         * @param layer_names       названия слоёв.
         * @param extension_names   названия расширений.
        */
        Device(
            const PhysicalDevice&                   physical_device, 
            const vector<VkDeviceQueueCreateInfo>&  queue_info, 
            const vector<const char*>&              layer_names, 
            const vector<const char*>&              extension_names
        );

        Device(Device&& device);
        Device(const Device&) = delete;

        ~Device();

        Device& operator = (Device&&)       = delete;
        Device& operator = (const Device&)  = delete;

        const VkDevice& getDeviceHandle() const noexcept;

        /**
         * @brief Метод необходимый для ожидания завершения всех очередей на устройстве.
        */
        void waitIdle() const;

    private:
        void _create(
            const PhysicalDevice&                   physical_device,
            const vector<VkDeviceQueueCreateInfo>&  queue_info,
            uint32_t                                enabled_layer_count,
            const char* const*                      ptr_enabled_layers,
            uint32_t                                enabled_extension_count,
            const char* const*                      ptr_extensions
        );

    private:
        VkDevice _device_handle;
    };
}

#endif /* Device_hpp */
