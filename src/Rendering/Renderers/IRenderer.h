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

    using PtrIRenderer  = shared_ptr<IRenderer>;
    using PtrDevice     = shared_ptr<Device>;

    /**
     * @class IRenderer.
     * @brief Основной интерфейс для реализации рендера.
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
        virtual void init(const PtrWindow& ptr_window, const PtrDevice& ptr_device, const PtrPhysicalDevice& ptr_physical_device) = 0;

        /**
         * @brief Метод, отвечающий за отрисовку узла.
         * 
         * @param visible_list  узлы.
         * @param delta_time    количество пройденного времени с момента завершения последнего кадра,
        */
        virtual void draw(const Scene::VisibleList& visible_list, float delta_time) = 0;
    };
}

#endif /* IRenderer_h */
