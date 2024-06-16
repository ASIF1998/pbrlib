//
//  CameraBase.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 06/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef CameraBase_hpp
#define CameraBase_hpp

#include <pbrlib/Moving/Transform.hpp>

#include <pbrlib/SceneGraph/Component.hpp>

#include <vulkan/vulkan.h>

struct VkViewport;

using Viewport = VkViewport;

namespace pbrlib
{
    class CameraBase;

    using PtrCameraBase = shared_ptr<CameraBase>;
    
    class CameraBase :
        public Component<CameraBase>
    {
    protected:
        /**
         * @brief Конструктор.
         * 
         * @param component_name    название компонента.
         * @param view              преобразование, осуществляющая перевод из   
         *                          мирового пространства в пространство вида.
         * @param projection        преобразование осуществляющее проекцию.
         * @param pos               позиция.
         * @param eye               вектор направления взгляда.
         * @param z_near            расстояние до ближайшей плоскости отсечения по оси Z.
         * @param z_far             расстояние до дальней плоскости отсечения по оси Z.
        */
        CameraBase(
            const string_view   component_name,
            const Transform&    view,
            const Transform&    projection,
            const Vec3<float>&  pos,
            const Vec3<float>&  eye,
            float               z_near,
            float               z_far,
            const Viewport&     viewport
        );

    public:
        Transform&          getView()           noexcept;
        const Transform&    getView()           const noexcept;
        Transform&          getProjection()     noexcept;
        const Transform&    getProjection()     const noexcept;
        Transform           getViewProjection() const noexcept;
        
        Vec3<float>&        getPosition()   noexcept;
        const Vec3<float>&  getPosition()   const noexcept;
        Vec3<float>&        getDirection()  noexcept;
        const Vec3<float>&  getDirection()  const noexcept;
        
        float getNearClipp()    const noexcept;
        float getFarClipp()     const noexcept;

        Viewport&       getViewport() noexcept;
        const Viewport& getViewport() const noexcept;

        void setLookAt(
            const Vec3<float>& eye,
            const Vec3<float>& pos,
            const Vec3<float>& up
        );

        /**
         * @brief 
         *      Метод позволяющий установить расстояние до ближней 
         *      плоскости отсечения по оси Z.
         * 
         * @param near расстояние до ближней плоскости отсечения по оси Z.
        */
        void setNearClipp(float near);

        /**
         * @brief 
         *      Метод позволяющий установить расстояние до дальней 
         *      плоскости отсечения по оси Z.
         * 
         * @param far расстояние до дальней плоскости отсечения по оси Z.
        */
        void setFarClipp(float far);

        /**
         * @brief 
         *      Метод позволяющий установить расстояния до дальней 
         *      и ближней плоскостей отсечения по оси Z.
         * 
         * @param near  расстояние до ближней плоскости отсечения по оси Z.
         * @param far   расстояние до дальней плоскости отсечения по оси Z.
        */
        void setNearAndFarClipp(float near, float far);

        /**
         * @brief Метод позволяющий установить параметры области просмотра.
         * 
         * @param viewport параметры области просмотра.
        */
        void setViewport(const Viewport& viewport) noexcept;

        /**
         * @brief Метод позволяющий установить параметры области просмотра.
         * 
         * @param x         координата верхнего левого угла просмотра по оси X.
         * @param y         координата верхнего левого угла просмотра по оси Y.
         * @param width     ширина области просмотра.
         * @param height    высота области просмотра. 
         * @param min_depth минимальная глубина.
         * @param max_depth максимальная глубина.
        */
        void setViewport(
            float x,
            float y,
            float width,
            float height,
            float min_depth,
            float max_depth
        ) noexcept;

    protected:
        virtual Transform _calculateProjection() const = 0;

    protected:
        Transform   _view;
        Transform   _projection;
        Vec3<float> _pos;
        Vec3<float> _dir;
        float       _z_near;
        float       _z_far;
        Viewport    _viewport;
    };
}

#endif /* CameraBase_hpp */
