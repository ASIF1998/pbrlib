#pragma once

#include <cstdint>

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

        inline Type lengthSquared() const;
        inline Type length() const;

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
