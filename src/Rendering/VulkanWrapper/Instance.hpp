//
//  Instance.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 01/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef Instance_hpp
#define Instance_hpp

#include <vulkan/vulkan.h>

#include <vector>

#include <string_view>
#include <string>

#include <set>

using namespace std;

namespace pbrlib
{
    struct PhysicalDevice;

    /**
     * @class VulkanInstanceExtensionSupported.
     * @brief 
     *      Данный класс необходим для проверки поддержки 
     *      расширений экземпляра.
    */
    class VulkanInstanceExtensionSupported
    {
    public:
        VulkanInstanceExtensionSupported();

        /**
         * @brief Метод проверяющий поддержку расширения.
         * 
         * @param name название расширения.
         * @return true - если расширение поддерживается.
        */
        inline bool check(const string& name) const;

    private:
        set<string> _extension_supported;
    };

    /**
     * @class VulkanInstanceLayerSupported.
     * @brief 
     *      Данный класс необходим для проверки поддержки 
     *      слоёв экземпляра.
    */
    class VulkanInstanceLayerSupported
    {
    public:
        VulkanInstanceLayerSupported();

        /**
         * @brief Метод проверяющий поддержку слоя.
         * 
         * @param name название слоя.
         * @return true - если слой поддерживается.
        */
        inline bool check(const string& name) const;

    private:
        set<string> _layer_supported;
    };

    class Instance
    {
    public:
        /**
         * @brief Конструктор.
         * 
         * @param app_name название приложения.
         * @param app_version номер приложения.
        */
        Instance(const string_view app_name, uint32_t app_version);

        /**
         * @brief Конструктор.
         * 
         * @param app_name название приложения.
         * @param app_version номер приложения.
         * @param layer_names названия слоёв.
         * @param extension_names названия расширений.
        */
        Instance(const string_view app_name, 
                 uint32_t app_version, 
                 const vector<const char*>& layer_names, 
                 const vector<const char*>& extension_names);
        
        inline ~Instance();

        inline VkInstance getHandle() const;

        PhysicalDevice& getPhysicalDevice(int type);
        vector<PhysicalDevice> getAllPhysicalDevice(int type) const;

        /**
         * @brief Статический метод проверяющий поддержку расширения.
         * 
         * @return true - если расширение поддерживается.
        */
        inline static bool isExtensionSupported(const string& name);

        /**
         * @brief Статический метод проверяющий поддержку слоя.
         * 
         * @return true - если слой поддерживается.
        */
        inline static bool isLayerSupported(const string& name);

        static vector<string> getExtensionNames();
        static vector<string> getLayerNames();

    private:
        VkInstance _instance_handle;
        vector<PhysicalDevice> _physical_device_handles;

        static VulkanInstanceExtensionSupported _supported_extensions;
        static VulkanInstanceLayerSupported _supported_layers;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inline bool VulkanInstanceExtensionSupported::check(const string& name) const
    {
        return _extension_supported.find(name) != _extension_supported.end();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////    
    inline bool VulkanInstanceLayerSupported::check(const string& name) const
    {
        return _layer_supported.find(name) != _layer_supported.end();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inline Instance::~Instance()
    {
        vkDestroyInstance(_instance_handle, nullptr);
    }

    inline VkInstance Instance::getHandle() const
    {
        return _instance_handle;
    }

    inline bool Instance::isExtensionSupported(const string& name)
    {
        return _supported_extensions.check(name);
    }

    inline bool Instance::isLayerSupported(const string& name)
    {
        return _supported_layers.check(name);
    }
}

#endif /* Instance_hpp */
