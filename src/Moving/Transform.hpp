//
//  Transform.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 17/04/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef Transform_hpp
#define Transform_hpp

#include <iostream>

#include "../math/matrix4x4.hpp"
#include "../math/vec3.hpp"

using namespace std;

namespace pbrlib
{
    using namespace math;

    class AABB;

    class Transform
    {
    public:
        Transform();
        Transform(const Matrix4x4<float>& m);

        bool operator == (const Transform& t) const;
        bool operator != (const Transform& t) const;
        
        Vec3<float> operator () (const Vec3<float>& v)      const;
        AABB        operator () (const AABB& bbox)          const;
        Transform   operator *  (const Transform& t)        const;

        bool identity() const;

        Matrix4x4<float>&       getMatrix()         noexcept;
        const Matrix4x4<float>& getMatrix()         const noexcept;
        Matrix4x4<float>        getInverseMatrix()  const noexcept;

        void setMatrix(const Matrix4x4<float>& m);

        static Transform translate(const Vec3<float>& t);
        static Transform scale(const Vec3<float>& s);

        /**
         * @brief 
         *      Статический метод возвращающий трансформацию,
         *      которая осуществляет поворот вокруг оси OX.
         * 
         * @param theta угол в градусах.
         * @return Трансформацию, осуществляющую поворот вокруг оси OX.
        */
        static Transform rotateX(float theta);

        /**
         * @brief 
         *      Статический метод возвращающий трансформацию,
         *      которая осуществляет поворот вокруг оси OY.
         * 
         * @param theta угол в градусах.
         * @return Трансформацию, осуществляющую поворот вокруг оси OY.
        */
        static Transform rotateY(float theta);

        /**
         * @brief 
         *      Статический метод возвращающий трансформацию,
         *      которая осуществляет поворот вокруг оси OZ.
         * 
         * @param theta угол в градусах.
         * @return Трансформацию, осуществляющую поворот вокруг оси OZ.
        */
        static Transform rotateZ(float theta);

        /**
         * @brief 
         *      Статический метод возвращающий трансформацию,
         *      которая осуществляет поворот вокруг произвольной оси.
         * 
         * @param axis  ось вокруг которой нужно осуществлять поворот.
         * @param theta угол в градусах.
         * @return Трансформацию, осуществляющую поворот вокруг оси OX.
        */
        static Transform rotate(const Vec3<float>& axis, float theta);

        /**
         * @brief 
         *      Статический метод создающий трансформацию, которая 
         *      осуществляет перевод в пространства вида.
         * 
         * @param pos   позиция.
         * @param eye   координаты цели.
         * @param up    вектор направления вверх.
         * @return трансформация, которая осуществляет перевод в пространства вида.
        */
        static Transform lookAt(
            const Vec3<float>& pos, 
            const Vec3<float>& eye, 
            const Vec3<float>& up
        );

        /**
         * @brief 
         *      Статический метод создающий трансформацию, которая 
         *      осуществляет перспективную проекцию.
         * 
         * @param fovy      поле зрения по оси OY.
         * @param aspect    соотношение сторон.
         * @param z_near    координата ближней плоскости по оси OZ.
         * @param z_far     координата дальней плоскости по оси OZ.
        */
        static Transform perspective(
            float fovy, 
            float aspect, 
            float z_near, 
            float z_far
        );

    private:
        Matrix4x4<float> _m;

    private:
        static constexpr Matrix4x4<float> IDENTITY_MATRIX;

        friend Transform inverse(const Transform& t);
        friend Transform transpose(const Transform& t);

        friend ostream& operator << (ostream& print, const Transform& t);
    };

    Transform inverse(const Transform& t);
    Transform transpose(const Transform& t);
    
    ostream& operator << (ostream& print, const Transform& t);
}

#endif /* Transform_hpp */
