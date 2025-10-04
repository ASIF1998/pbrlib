#pragma once

#include <cstdint>

namespace pbrlib::math
{
    template<typename Type>
    struct Vec2 final
    {
    public:
        inline constexpr Vec2()                 noexcept;
        inline constexpr Vec2(Type xy)          noexcept;
        inline constexpr Vec2(Type x, Type y)   noexcept;

        inline bool operator == (const Vec2& v) const noexcept;
        inline bool operator != (const Vec2& v) const noexcept;
        
        inline Vec2 operator + (const Vec2& v)  const noexcept;
        inline Vec2 operator - (const Vec2& v)  const noexcept;
        inline Vec2 operator * (Type s)         const noexcept;

        inline Vec2& operator += (const Vec2& v)    noexcept;
        inline Vec2& operator -= (const Vec2& v)    noexcept;
        inline Vec2& operator *= (Type s)           noexcept;

        inline Type& operator [] (size_t i) noexcept;
        inline Type operator [] (size_t i)  const noexcept;

        inline Type lengthSquared() const noexcept;
        inline Type length()        const noexcept;

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

    template<typename Type>
    Vec2<Type> normalize(const Vec2<Type>& v);
}

#include "vec2.inl"
