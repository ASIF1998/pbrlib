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

using namespace std;

namespace pbrlib
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    struct VulkanResources
    {
    public:
        VulkanResources();

    public:
        PtrInstance         _ptr_instance;          //!< Указатель на экземпляр Vulkan'а.
        PtrPhysicalDevice   _ptr_physical_device;   //!< Указатель на физическое устройство (GPU).
        PtrDevice           _ptr_device;            //!< Указатель на логическое устройство (GPU).
        PtrDeviceQueue      _ptr_device_queue;      //!< Указатель на очередь устройства.
        PtrCommandPool      _ptr_command_pool;      //!< Указатель на командный пул.
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class PBRLibResources;

    using PtrPBRLibResources = shared_ptr<PBRLibResources>;

    class PBRLibResources
    {
        friend class SceneView;

        PBRLibResources();

    public:
        ~PBRLibResources();
        
        static PtrPBRLibResources init();

    private:
        VulkanResources _vk_resources;
    };
}

#include "PBRLibResources.inl"

#endif /* PBRLibResources_hpp */
