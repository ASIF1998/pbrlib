#pragma once

#include <pbrlib/math/matrix4x4.hpp>
#include <pbrlib/math/vec3.hpp>

namespace pbrlib
{
    class Transform
    {
        friend Transform inverse(const Transform& t);
        friend Transform transpose(const Transform& t);

    public:
        Transform();
        Transform(const math::mat4& m);

		bool operator == (const Transform& t) const;
		bool operator != (const Transform& t) const;

		math::vec3  operator () (const math::vec3& v)   const;
		Transform   operator *  (const Transform& t)    const;

		bool identity() const;

		math::mat4&         getMatrix()         noexcept;
		const math::mat4&   getMatrix()         const noexcept;
		math::mat4          getInverseMatrix()  const noexcept;

		void setMatrix(const math::mat4& m);

		static Transform translate(const math::vec3& t);
		static Transform scale(const math::vec3& s);

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
        static Transform rotate(const math::vec3& axis, float theta);

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
            const math::vec3& pos, 
            const math::vec3& eye, 
            const math::vec3& up
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
        math::mat4 _m;
    };

    Transform inverse(const Transform& t);
    Transform transpose(const Transform& t);
}
