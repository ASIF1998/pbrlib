#pragma once

#include "vec2.hpp"

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

    template<typename Type>
    inline Type dot(const Vec3<Type> v1, const Vec3<Type>& v2);

    template<typename Type>
    inline Vec3<Type> cross(const Vec3<Type> v1, const Vec3<Type>& v2);

    template<typename Type>
    Vec3<Type> normalize(const Vec3<Type>& v);
}

#include "vec3.inl"
