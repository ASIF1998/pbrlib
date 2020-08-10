//
//  vec3.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 22/01/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef vec3_hpp
#define vec3_hpp

#include <iostream>

#include "vec2.hpp"

using namespace std;

namespace pbrlib::math
{
    template<typename Type>
    struct Vec3
    {
    public:
        inline constexpr Vec3(Type xyz = static_cast<Type>(0));
        inline constexpr Vec3(Type x, Type y, Type z = static_cast<Type>(0));
        inline constexpr Vec3(const Vec2<Type>& vec2, Type z = static_cast<Type>(0));

        inline bool operator == (const Vec3& v) const;
        inline bool operator != (const Vec3& v) const;

        inline Vec3 operator + (const Vec3& v)  const;
        inline Vec3 operator - (const Vec3& v)  const;
        inline Vec3 operator * (Type s)         const;

        inline Vec3& operator += (const Vec3& v);
        inline Vec3& operator -= (const Vec3& v);
        inline Vec3& operator *= (Type s);

        inline Type&    operator [] (size_t i) noexcept;
        inline Type     operator [] (size_t i) const noexcept;

        /**
         * @brief Метод необходимый для вычисления квадрата длины вектора.
         * 
         * @return квадрат длины вектора.
        */
        inline Type lengthSquared() const;

        /**
         * @brief Метод необходимый для вычисления длины вектора.
         * 
         * @return длину векора.
        */
        inline Type length() const;

        /**
         * @brief Метод необходимый для нормирования вектора.
        */
        void normalize();

    public:
        union
        {
            struct
            {
                Type x;
                Type y;
                Type z;
            };
            
            struct
            {
                Type r;
                Type g;
                Type b;
            };

            Type xyz[3];
            Type rgb[3];
        };
    };

    /**
     * @brief Функция предназначенная для осуществления скалярного произведения.
     * 
     * @param v1 вектор.
     * @param v2 вектор.
     * @return результат скалярного произведения.
    */
    template<typename Type>
    inline Type dot(const Vec3<Type> v1, const Vec3<Type>& v2);

    /**
     * @brief Функция предназначенная для осуществления векторного произведения.
     * 
     * @param v1 вектор.
     * @param v2 вектор.
     * @return результат векторного произведения.
    */
    template<typename Type>
    inline Vec3<Type> cross(const Vec3<Type> v1, const Vec3<Type>& v2);

    /**
     * @brief Функция необходимая для нормирования вектора.
     * 
     * @param v вектор.
     * @return нормированный вектор v.
    */
    template<typename Type>
    Vec3<Type> normalize(const Vec3<Type>& v);

    template<typename Type>
    ostream& operator << (ostream& print, const Vec3<Type>& vec);
}

#include "vec3.inl"

#endif /* vec3_hpp */
