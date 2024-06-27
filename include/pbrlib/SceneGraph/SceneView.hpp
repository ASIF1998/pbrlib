//
//  SceneView.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 02/06/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef SceneView_hpp
#define SceneView_hpp

#include <pbrlib/Rendering/VulkanWrapper/Instance.hpp>
#include <pbrlib/Rendering/VulkanWrapper/Device.hpp>
#include <pbrlib/Rendering/VulkanWrapper/CommandBuffer.hpp>

#include "Scene.hpp"

namespace pbrlib
{
    class IRenderer;
    class PBRLibResources;
}

namespace pbrlib
{
    class SceneView
    {
    public:
        SceneView(const std::string_view scene_name, std::shared_ptr<const PBRLibResources> ptr_pbrlib_resources, std::shared_ptr<Window> ptr_window);

        Scene&                          getScene()  noexcept;
        const Scene&                    getScene()  const noexcept;
        std::shared_ptr<const Window>   getWindow() const noexcept;

        void setRenderer(std::shared_ptr<IRenderer> ptr_renderer);
        void drawScene(float delta_time);

    private:
        std::shared_ptr<const PBRLibResources>  _ptr_pbrlib_resources;
        std::shared_ptr<IRenderer>              _ptr_renderer;          //!< Указатель на визуализатор.
        std::shared_ptr<Window>                 _ptr_window;            //!< Указатель на окно.
        Scene                                   _scene;                 //!< Сцена.
    };
}

#endif /* SceneView_hpp */
