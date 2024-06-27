//
//  PBRLibResources.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 16.09.2021.
//

#ifndef PBRLibResources_hpp
#define PBRLibResources_hpp

#include "Rendering/VulkanWrapper/Instance.hpp"
#include "Rendering/VulkanWrapper/PhysicalDevice.hpp"
#include "Rendering/VulkanWrapper/Device.hpp"
#include "Rendering/VulkanWrapper/DeviceQueue.hpp"
#include "Rendering/VulkanWrapper/CommandPool.hpp"

#include <memory>

namespace pbrlib
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    struct VulkanResources
    {
    public:
        VulkanResources();

    public:
        std::unique_ptr<const Instance>         _ptr_instance;          //!< Указатель на экземпляр Vulkan'а.
        std::unique_ptr<const PhysicalDevice>   _ptr_physical_device;   //!< Указатель на физическое устройство (GPU).
        std::unique_ptr<const Device>           _ptr_device;            //!< Указатель на логическое устройство (GPU).
        std::unique_ptr<const DeviceQueue>      _ptr_device_queue;      //!< Указатель на очередь устройства.
        std::unique_ptr<const CommandPool>      _ptr_command_pool;      //!< Указатель на командный пул.
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class PBRLibResources
    {
        friend class SceneView;

        PBRLibResources();

    public:
        ~PBRLibResources();
        
        static std::unique_ptr<PBRLibResources> init();

    private:
        VulkanResources _vk_resources;
    };
}

#include "PBRLibResources.inl"

#endif /* PBRLibResources_hpp */
