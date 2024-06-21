//
//  PhysicalDevice.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 02/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef PhysicalDevice_hpp
#define PhysicalDevice_hpp

#include <set>
#include <vector>

#include <string>

#include <memory>

#include "Device.hpp"

namespace pbrlib
{
    struct PhysicalDevice;

    using PtrPhysicalDevice = std::shared_ptr<PhysicalDevice>;

    struct PhysicalDevice
    {
    public:
        class Memory
        {
        public:
            Memory(VkPhysicalDevice physical_device_handle);

            uint32_t getMemoryType(VkMemoryPropertyFlags property_flags) const;

        private:
            VkPhysicalDeviceMemoryProperties _memory_properties;
        };

    public:
        PhysicalDevice(VkPhysicalDevice physical_device_handle);

        bool isExtensionSupported(const std::string& name)  const;
        bool isLayerSupported(const std::string& name)      const;

        bool isFormatSupported(
            VkFormat            format, 
            VkImageType         image_type, 
            VkImageTiling       image_tiling, 
            VkImageUsageFlags   image_usage
        ) const;

        VkFormatProperties getFormatProperties(VkFormat format) const;

        /**
         * @brief Метод создающий логическое устройство.
         * 
         * @param queue_info информация о создаваемых очередях логического устройства.
         * @return Логическое устройство.
        */
        PtrDevice makeDevice(const PtrInstance& ptr_instance, std::span<const VkDeviceQueueCreateInfo> queue_info) const;
        
        PtrDevice makeDevice(
            const PtrInstance&                      ptr_instance,
            std::span<const VkDeviceQueueCreateInfo>     queue_info,
            std::span<const char*>                       layer_names,
            std::span<const char*>                       extension_names
        ) const;

        /**
         * @brief Метод возвращающий индекс типа памяти.
         * @details Типом памяти является набор флагов VkMemoryPropertyFlags и индекс кучи.
         * 
         * @param type набор флагов VkMemoryPropertyFlags.
         * @return Индекс типа памяти.
        */
        uint32_t getMemoryTypeIndex(uint32_t type) const;

        /**
         * @brief Метод возвращающий все индексы типа памяти.
         * @details Типом памяти является набор флагов VkMemoryPropertyFlags и индекс кучи.
         * 
         * @param type набор флагов VkMemoryPropertyFlags.
         * @return индексы типа памяти.
        */
        std::vector<uint32_t> getAllMemoryTypeIndicies(uint32_t type) const;

        std::vector<VkQueueFamilyProperties> getQueueFamilyProperties() const;

    public:
        VkPhysicalDevice        physical_device_handle;
        PhysicalDevice::Memory  memory;

    private:
        std::set<std::string> _search_extension_names;
        std::set<std::string> _search_layer_names;
    };
}

#endif /* PhysicalDevice_hpp */
