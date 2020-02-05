//
//  PhysicalDevice.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 02/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef PhysicalDevice_hpp
#define PhysicalDevice_hpp

#include <vulkan/vulkan.h>

#include <set>
#include <vector>

#include <string>

#include <memory>

using namespace std;

namespace pbrlib
{
    class Device;

    struct PhysicalDevice
    {
    public:
        PhysicalDevice(VkPhysicalDevice physical_device_handle);

        bool isExtensionSupported(const string& name) const;
        bool isLayerSupported(const string& name) const;

        bool isFormatSupported(VkFormat format, VkImageType image_type, VkImageTiling image_tiling, VkImageUsageFlags image_usage) const;
        VkFormatProperties getFormatProperties(VkFormat format) const;

        /**
         * @brief Метод создающий логическое устройство.
         * 
         * @param queue_info информация о создаваемых очередях логического устройства.
         * @return логическое устройство.
        */
        shared_ptr<Device> makeDevice(const vector<VkDeviceQueueCreateInfo>& queue_info);

        /**
         * @brief Метод возвращающий индекс типа памяти.
         * @details Типом памяти является набор флагов VkMemoryPropertyFlags и индекс кучи.
         * 
         * @param type набор флагов VkMemoryPropertyFlags.
         * @return индекс типа памяти.
        */
        uint32_t getMemoryTypeIndex(uint32_t type) const;

        /**
         * @brief Метод возвращающий все индексы типа памяти.
         * @details Типом памяти является набор флагов VkMemoryPropertyFlags и индекс кучи.
         * 
         * @param type набор флагов VkMemoryPropertyFlags.
         * @return индексы типа памяти.
        */
        vector<uint32_t> getAllMemoryTypeIndicies(uint32_t type) const;

    public:
        VkPhysicalDevice physical_device_handle;

    private:
        set<string> _search_extension_names;
        set<string> _search_layer_names;
    };
}

#endif /* PhysicalDevice_hpp */
