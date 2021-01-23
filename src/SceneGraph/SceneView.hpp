//
//  SceneView.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 02/06/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef SceneView_hpp
#define SceneView_hpp

#include "../Rendering/VulkanWrapper/Instance.hpp"
#include "../Rendering/VulkanWrapper/Device.hpp"
#include "../Rendering/VulkanWrapper/CommandPool.hpp"

#include "Scene.hpp"

namespace pbrlib
{
    class 	IRenderer;
    class   Window;
    struct  PhysicalDevice;
    class   DeviceQueue;

    using PtrIRenderer      = shared_ptr<IRenderer>;
    using PtrPhysicalDevice = shared_ptr<PhysicalDevice>;
    using PtrWindow         = shared_ptr<Window>;
    using PtrDeviceQueue    = shared_ptr<DeviceQueue>;

    class SceneView
    {
    private:
        struct VulkanResources
        {
        public:
            VulkanResources();

        public:
            PtrInstance         ptr_instance;           //!< Указатель на экземпляр Vulkan'а.
            PtrDevice           ptr_device;             //!< Указатель на логическое устройство (GPU).
            PtrPhysicalDevice   ptr_physical_device;    //!< Указатель на физическое устройство (GPU).
            PtrDeviceQueue      ptr_device_queue;       //!< Указатель на очередь устройства.
            PtrCommandPool      ptr_command_pool;       //!< Указатель на командный пул.
        };

    public:
        SceneView(const string_view scene_name, const PtrWindow& ptr_window);

        Scene&              getScene()  noexcept;
        const Scene&        getScene()  const noexcept;
        PtrWindow&          getWindow() noexcept;
        const PtrWindow&    getWindow() const noexcept;

        void setRenderer(const PtrIRenderer& ptr_renderer);
        void drawScene(float delta_time);

    private:
        static VulkanResources  _vulkan_resources; //!< Необходимые объекты Vulkan.

        PtrIRenderer            _ptr_renderer;  //!< Указатель на визуализатор.
        PtrWindow               _ptr_window;    //!< Указатель на окно.
        Scene                   _scene;         //!< Сцена.
    };
}

#endif /* SceneView_hpp */
