//
//  Instance.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 01/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef Instance_hpp
#define Instance_hpp

#include <vector>
#include <set>

#include <string_view>
#include <string>

#include <memory>

#include "PhysicalDevice.hpp"

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

        /**
         * @brief Статический метод создающий экземпляр Vulkan'а.
         * 
         * @param app_name название приложения.
         * @param app_version номер приложения.
        */
        inline static shared_ptr<Instance> makeInstance(const string_view app_name, uint32_t app_version);

        /**
         * @brief Статический метод создающий экземпляр Vulkan'а.
         * 
         * @param app_name название приложения.
         * @param app_version номер приложения.
         * @param layer_names названия слоёв.
         * @param extension_names названия расширений.
        */
        inline static shared_ptr<Instance> makeInstance(const string_view app_name, 
                                                        uint32_t app_version,
                                                        const vector<const char*>& layer_names,
                                                        const vector<const char*>& extension_names);

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

    inline shared_ptr<Instance> Instance::makeInstance(const string_view app_name, uint32_t app_version)
    {
        return make_shared<Instance>(app_name, app_version);
    }

    inline shared_ptr<Instance> Instance::makeInstance(const string_view app_name, 
                                                       uint32_t app_version,
                                                       const vector<const char*>& layer_names,
                                                       const vector<const char*>& extension_names)
    {
        return make_shared<Instance>(app_name, app_version, layer_names, extension_names);
    }
}

#endif /* Instance_hpp */
