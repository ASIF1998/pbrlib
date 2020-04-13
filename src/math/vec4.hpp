//
//  vec4.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 29/01/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef vec4_hpp
#define vec4_hpp

#include <iostream>

#include <xmmintrin.h>

using namespace std;

namespace pbrlib::math
{
    template<typename Type>
    struct Vec4
    {
    public:
        inline constexpr Vec4();
        inline constexpr Vec4(Type xyzw);
        inline constexpr Vec4(Type x, Type y, Type z, Type w);

        inline bool operator == (const Vec4& v) const;
        inline bool operator != (const Vec4& v) const;

        inline Vec4 operator + (const Vec4& v)  const;
        inline Vec4 operator - (const Vec4& v)  const;
        inline Vec4 operator * (Type s)         const;

        inline Vec4& operator += (const Vec4& v);
        inline Vec4& operator -= (const Vec4& v);
        inline Vec4& operator *= (Type s);

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
        inline void normalize();

    public:
        union
        {
            struct
            {
                Type x;
                Type y;
                Type z;
                Type w;
            };

            Type xyzw[4];
        };
    };

    template<>
    struct Vec4<float>
    {
    public:
        inline constexpr    Vec4()                                      noexcept;
        inline              Vec4(float xyzw)                            noexcept;
        inline constexpr    Vec4(__m128 xyzw)                           noexcept;
        inline constexpr    Vec4(float x, float y, float z, float w)    noexcept;

        inline bool operator == (const Vec4& v) const noexcept;
        inline bool operator != (const Vec4& v) const noexcept;

        inline Vec4 operator + (const Vec4& v)  const noexcept;
        inline Vec4 operator - (const Vec4& v)  const noexcept;
        inline Vec4 operator * (float s)        const noexcept;

        inline Vec4& operator += (const Vec4& v)    noexcept;
        inline Vec4& operator -= (const Vec4& v)    noexcept;
        inline Vec4& operator *= (float s)          noexcept;

        inline float&   operator [] (size_t i) noexcept;
        inline float    operator [] (size_t i) const noexcept;

        /**
         * @brief Метод необходимый для вычисления квадрата длины вектора.
         * 
         * @return квадрат длины вектора.
        */
        inline float lengthSquared() const noexcept;

        /**
         * @brief Метод необходимый для вычисления длины вектора.
         * 
         * @return длину векора.
        */
        inline float length() const;

        /**
         * @brief Метод необходимый для нормирования вектора.
        */
        inline void normalize();

    public:
        union
        {
            struct 
            {
                float x;
                float y;
                float z;
                float w;
            };

            float   xyzw[4];
            __m128  xyzw_simd;
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
    inline Type dot(const Vec4<Type> v1, const Vec4<Type>& v2);
    
    template<>
    inline float dot<float>(const Vec4<float> v1, const Vec4<float>& v2);

    /**
     * @brief Функция необходимая для нормирования вектора.
     * 
     * @param v вектор.
     * @return нормированный вектор v.
    */
    template<typename Type>
    inline Vec4<Type> normalize(const Vec4<Type>& v);

    template<typename Type>
    ostream& operator << (ostream& print, const Vec4<Type>& vec);
}

#include "vec4.inl"

#endif /* vec4_hpp */
