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
     *      1) Добавить поддержку проверки семейства очередей на возможность осуществления показа.
     *      2) Доавить возмождность создавать командный пулы.
    */

    class Device
    {
    public:
        /**
         * Конструктор.
         * 
         * @param physical_device физическое устройство.
         * @param queue_info информация о создаваемых очередях логического устройства.
        */
        Device(PhysicalDevice physical_device, const vector<VkDeviceQueueCreateInfo>& queue_info);

        /**
         * Конструктор.
         * 
         * @param physical_device физическое устройство.
         * @param queue_info информация о создаваемых очередях логического устройства.
         * @param layer_names названия слоёв.
         * @param extension_names названия расщирений.
        */
        Device(PhysicalDevice physical_device, 
               const vector<VkDeviceQueueCreateInfo>& queue_info, 
               const vector<const char*>& layer_names, 
               const vector<const char*>& extension_names);

        ~Device();

        // VkDevice getHandle() const;
        // VkQueue getQueue(uint32_t famili_index, uint32_t index) const;
        // VkQueue getQueueByPresent() const;

        /**
         * @brief Метод необходимый для ожидания завершения всех очередей на устройстве.
        */
        void waitIdle() const;

    private:
        VkDevice _device_handle;
    };
}

#endif /* Device_hpp */
