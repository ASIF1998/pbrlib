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

#include "Scene.hpp"

namespace pbrlib
{
    class IRenderer;
    class Window;
    class PhysicalDevice;

    using PtrIRenderer      = shared_ptr<IRenderer>;
    using PtrPhysicalDevice = shared_ptr<PhysicalDevice>;

    class SceneView
    {
    public:
        SceneView(const string_view scene_name, const Window& window);

        void setRenderer(const PtrIRenderer& ptr_renderer);

        PtrInstance&        getInstance()   noexcept;
        const PtrInstance&  getInstance()   const noexcept;
        PtrDevice&          getDevice()     noexcept;
        const PtrDevice&    getDevice()     const noexcept;
        PtrIRenderer&       getRenderer()   noexcept;
        const PtrIRenderer& getRenderer()   const noexcept;
        Scene&              getScene()      noexcept;
        const Scene&        getScene()      const noexcept;

        void drawScene(Window& window, float delta_time);

    private:
        PtrInstance         _ptr_instance;          //!< Указатель на экземпляр Vulkan'а.
        PtrDevice           _ptr_device;            //!< Указатель на логическое устройство (GPU).
        PtrPhysicalDevice   _ptr_physical_device;   //!< Указатель на физичестое устройство (GPU).

        vector<VkDeviceQueueCreateInfo> _queues_infos; //!< Информация об используемый очередях устройства.

        PtrIRenderer    _ptr_renderer;  //!< Указатель на визуализатор.
        Scene           _scene;         //!< Сцена.
    };
}

#endif /* SceneView_hpp */
