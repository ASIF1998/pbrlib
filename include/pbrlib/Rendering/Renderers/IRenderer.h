//
//  IRenderer.h
//  PBRLib
//
//  Created by Асиф Мамедов on 02/06/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef IRenderer_h
#define IRenderer_h

#include <pbrlib/scene/Scene.hpp>

#include <span>

namespace pbrlib
{
    class IRenderer;
    class Device;
    class CameraBase;

    /**
     * @class IRenderer.
     * @brief Интерфейс для реализации рендера.
    */
    class IRenderer
    {
    public:
        inline virtual ~IRenderer()  noexcept
        {}

        /**
         * @brief Чисто виртуальный метод, предназначенный для инициализации визуализатора.
         * @details Этот метод вызывается из метода SceneView::setRenderer(...)
         * 
         * @param ptr_window            указатель на окно.
         * @param ptr_device            указатель на логическое устройство.
         * @param ptr_physical_device   указатель на физическое устройство.
        */
        virtual void init(
            std::shared_ptr<const Window>   ptr_window, 
            const Device*                   ptr_device, 
            const PhysicalDevice*           ptr_physical_device
        ) = 0;

        /**
         * @brief Метод, отвечающий за отрисовку.
         * 
         * @param ptr_camera        указатель на камеру.
         * @param visible_list      видимые узлы.
         * @param point_lights      точечные источники света.
         * @param spot_lights       прожекторные источники света.
         * @param direction_lights  направленные источники света.
         * @param delta_time        количество пройденного времени с момента завершения последнего кадра.
        */
        virtual void draw(
            std::shared_ptr<const SceneItem>            ptr_camera,
            const VisibleList&                          visible_list, 
            std::span<std::shared_ptr<SceneItem>>       point_lights,
            std::span<std::shared_ptr<SceneItem>>       spot_lights,
            std::span<std::shared_ptr<SceneItem>>       direction_lights,
            float                                       delta_time
        ) = 0;
    };
}

#endif /* IRenderer_h */
