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
    class IRenderer;
    class Window;
    class PBRLibResources;

    using PtrIRenderer          = shared_ptr<IRenderer>;
    using PtrWindow             = shared_ptr<Window>;
    using PtrPBRLibResources    = shared_ptr<PBRLibResources>; 

    class SceneView
    {
    public:
        SceneView(const string_view scene_name, const PtrPBRLibResources& ptr_pbrlib_resources, const PtrWindow& ptr_window);

        Scene&              getScene()  noexcept;
        const Scene&        getScene()  const noexcept;
        PtrWindow&          getWindow() noexcept;
        const PtrWindow&    getWindow() const noexcept;

        void setRenderer(const PtrIRenderer& ptr_renderer);
        void drawScene(float delta_time);

    private:
        PtrPBRLibResources      _ptr_pbrlib_resources;
        PtrIRenderer            _ptr_renderer;          //!< Указатель на визуализатор.
        PtrWindow               _ptr_window;            //!< Указатель на окно.
        Scene                   _scene;                 //!< Сцена.
    };
}

#endif /* SceneView_hpp */
