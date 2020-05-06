//
//  ICamera.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 06/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef ICamera_hpp
#define ICamera_hpp

#include "../../Moving/Transform.hpp"

namespace pbrlib
{
    class ICamera
    {
    protected:
        /**
         * @brief Конструктор.
         * 
         * @param view          преобразование, осуществляющая перевод из   
         *                      мирового пространства в пространство вида.
         * @param projection    преобразование осуществляющее проекцию.
         * @param pos           позиция.
         * @param eye           вектор направления взгляда.
         * @param z_near        расстояние до ближайшей плоскости отсечения по оси Z.
         * @param z_far         расстояние до дальней плоскости отсечения по оси Z.
        */
        ICamera(
            const Transform&    view,
            const Transform&    projection,
            const Vec3<float>&  pos,
            const Vec3<float>&  eye,
            float               z_near,
            float               z_far
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

    protected:
        virtual Transform _calculateProjection() const = 0;

    protected:
        Transform   _view;
        Transform   _projection;
        Vec3<float> _pos;
        Vec3<float> _dir;
        float       _z_near;
        float       _z_far;
    };
}

#endif /* ICamera_hpp */
