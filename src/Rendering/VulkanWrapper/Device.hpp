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

using namespace std;

namespace pbrlib
{
    struct PhysicalDevice;

    /**
     * TODO: 
     *      1) Добавить возможность создавать командный пулы.
    */

    class Device
    {
    public:
        /**
         * Конструктор.
         * 
         * @param physical_device   физическое устройство.
         * @param queue_info        информация о создаваемых очередях логического устройства.
        */
        inline Device(const PhysicalDevice& physical_device, const vector<VkDeviceQueueCreateInfo>& queue_info);

        /**
         * Конструктор.
         * 
         * @param physical_device   физическое устройство.
         * @param queue_info        информация о создаваемых очередях логического устройства.
         * @param layer_names       названия слоёв.
         * @param extension_names   названия расширений.
        */
        inline Device(
            const PhysicalDevice&                   physical_device, 
            const vector<VkDeviceQueueCreateInfo>&  queue_info, 
            const vector<const char*>&              layer_names, 
            const vector<const char*>&              extension_names
        );

        inline Device(Device&& device);
        Device(const Device&) = delete;

        inline ~Device();

        Device& operator = (Device&&)       = delete;
        Device& operator = (const Device&)  = delete;

        inline const VkDevice& getDeviceHandle() const noexcept;

        /**
         * @brief Метод необходимый для ожидания завершения всех очередей на устройстве.
        */
        inline void waitIdle() const;

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

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inline Device::Device(const PhysicalDevice& physical_device, const vector<VkDeviceQueueCreateInfo>& queue_info) :
        _device_handle(VK_NULL_HANDLE)
    {
        _create(physical_device, queue_info, 0, nullptr, 0, nullptr);
    }

    inline Device::Device(
        const PhysicalDevice&                   physical_device, 
        const vector<VkDeviceQueueCreateInfo>&  queue_info, 
        const vector<const char*>&              layer_names, 
        const vector<const char*>&              extension_names
    ) :
        _device_handle(VK_NULL_HANDLE)
    {
        _create(
            physical_device, queue_info,
            static_cast<uint32_t>(layer_names.size()),
            layer_names.data(),
            static_cast<uint32_t>(extension_names.size()),
            extension_names.data()
        );
    }

    inline Device::Device(Device&& device) :
        _device_handle(VK_NULL_HANDLE)
    {
        swap(_device_handle, device._device_handle);
    }

    inline Device::~Device()
    {
        if (_device_handle != VK_NULL_HANDLE) {
            vkDestroyDevice(_device_handle, nullptr);
        }
    }

    inline const VkDevice& Device::getDeviceHandle() const noexcept
    {
        return _device_handle;
    }

    inline void Device::waitIdle() const
    {
        vkDeviceWaitIdle(_device_handle);
    }
}

#endif /* Device_hpp */
