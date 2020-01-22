//
//  vec2.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 21/01/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef vec2_hpp
#define vec2_hpp

namespace pbrlib::math
{
    template<typename Type>
    struct Vec2
    {
    public:
        inline Vec2(Type xy = static_cast<Type>(0));
        inline Vec2(Type x, Type y);

        inline bool operator == (const Vec2& v) const noexcept;
        inline bool operator != (const Vec2& v) const noexcept;
        
        inline Vec2 operator + (const Vec2& v) const noexcept;
        inline Vec2 operator - (const Vec2& v) const noexcept;
        inline Vec2 operator * (Type s) const noexcept;

        inline Vec2& operator += (const Vec2& v) noexcept;
        inline Vec2& operator -= (const Vec2& v) noexcept;
        inline Vec2& operator *= (Type s) noexcept;

        inline Type& operator [] (size_t i) noexcept;
        inline Type operator [] (size_t i) const noexcept;

        /**
         * @brief Метод необходимый для вычисления квадрата длины вектора.
         * 
         * @return квадрат длины векора.
        */
        inline Type lengthSquared() const noexcept;

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
            };

            Type xy[2];
        };
    };

    /**
     * @brief Функция необходимая для нормирования вектора.
     * 
     * @param v вектор.
     * @return нормированный вектор v.
    */
    template<typename Type>
    Vec2<Type> normalize(const Vec2<Type>& v);
}

#include "vec2.inl"

#endif /* vec2_hpp */
