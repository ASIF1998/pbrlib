//
//  IRenderer.h
//  PBRLib
//
//  Created by Асиф Мамедов on 02/06/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef IRenderer_h
#define IRenderer_h

#include "../../SceneGraph/Scene.hpp"

namespace pbrlib
{
    class IRenderer;
    class Device;
    class CameraBase;

    using PtrIRenderer  = shared_ptr<IRenderer>;
    using PtrDevice     = shared_ptr<Device>;

    /**
     * @class IRenderer.
     * @brief Интерфейс для реализации рендера.
    */
    class IRenderer
    {
    public:
        /**
         * @brief Чисто виртуальный метод, предназначенный для инициализации визуализатора.
         * @details Этот метод вызывается из метода SceneView::setRenderer(...)
         * 
         * @param ptr_window            указатель на окно.
         * @param ptr_device            указатель на логическое устройство.
         * @param ptr_physical_device   указатель на физическое устройство.
        */
        virtual void init(
            const PtrWindow&            ptr_window, 
            const PtrDevice&            ptr_device, 
            const PtrPhysicalDevice&    ptr_physical_device
        ) = 0;

        /**
         * @brief Метод, отвечающий за отрисовку.
         * 
         * @param camera            камера.
         * @param visible_list      видимые узлы.
         * @param point_lights      точечные источники света.
         * @param spot_lights       прожекторные источники света.
         * @param direction_lights  направленные источники света.
         * @param delta_time        количество пройденного времени с момента завершения последнего кадра.
        */
        virtual void draw(
            const CameraBase&               camera,
            const Scene::VisibleList&       visible_list, 
            const vector<Scene::PtrNode>    point_lights,
            const vector<Scene::PtrNode>    spot_lights,
            const vector<Scene::PtrNode>    direction_lights,
            float                           delta_time
        ) = 0;
    };
}

#endif /* IRenderer_h */
