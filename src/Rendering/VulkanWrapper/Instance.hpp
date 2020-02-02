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
        bool check(const string& name) const;

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
        bool check(const string& name) const;

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
        
        ~Instance();

        VkInstance getHandle() const;

        PhysicalDevice getGPUHandle(int type) const;
        vector<PhysicalDevice> getAllGPUHandle(int type) const;

        /**
         * @brief Статический метод проверяющий поддержку расширения.
         * 
         * @return true - если расширение поддерживается.
        */
        static bool isExtensionSupported(const string& name);

        /**
         * @brief Статический метод проверяющий поддержку слоя.
         * 
         * @return true - если слой поддерживается.
        */
        static bool isLayerSupported(const string& name);

        static vector<string> getExtensionNames();
        static vector<string> getLayerNames();

    private:
        VkInstance _instance_handle;
        vector<PhysicalDevice> _physical_device_handles;

        static VulkanInstanceExtensionSupported _supported_extensions;
        static VulkanInstanceLayerSupported _supported_layers;
    };
}

#endif /* Instance_hpp */
