#pragma once

namespace pbrlib::math
{
    template<typename Type>
    struct Vec2
    {
    public:
        inline constexpr Vec2();
        inline constexpr Vec2(Type xy);
        inline constexpr Vec2(Type x, Type y);

        inline bool operator == (const Vec2& v) const;
        inline bool operator != (const Vec2& v) const;
        
        inline Vec2 operator + (const Vec2& v)  const;
        inline Vec2 operator - (const Vec2& v)  const;
        inline Vec2 operator * (Type s)         const;

        inline Vec2& operator += (const Vec2& v);
        inline Vec2& operator -= (const Vec2& v);
        inline Vec2& operator *= (Type s);

        inline Type& operator [] (size_t i) noexcept;
        inline Type operator [] (size_t i)  const noexcept;

        /**
         * @brief Метод необходимый для вычисления квадрата длины вектора.
         * 
         * @return квадрат длины векора.
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
            };
            
            struct
            {
                Type r;
                Type g;
            };

            Type xy[2];
            Type rg[2];
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

namespace pbrlib::math
{
    using vec2 = Vec2<float>;
}

#include "vec2.inl"
