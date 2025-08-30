#pragma once

#include "vec2.hpp"

namespace pbrlib::math
{
    template<typename Type>
    struct Vec3 final
    {
    public:
        inline constexpr Vec3(Type xyz = static_cast<Type>(0))                          noexcept;
        inline constexpr Vec3(Type x, Type y, Type z = static_cast<Type>(0))            noexcept;
        inline constexpr Vec3(const Vec2<Type>& vec2, Type z = static_cast<Type>(0))    noexcept;

        inline bool operator == (const Vec3& v) const noexcept;
        inline bool operator != (const Vec3& v) const noexcept;

        inline Vec3 operator + (const Vec3& v)  const noexcept;
        inline Vec3 operator - (const Vec3& v)  const noexcept;
        inline Vec3 operator * (Type s)         const noexcept;

        inline Vec3& operator += (const Vec3& v)    noexcept;
        inline Vec3& operator -= (const Vec3& v)    noexcept;
        inline Vec3& operator *= (Type s)           noexcept;

        inline Type&    operator [] (size_t i) noexcept;
        inline Type     operator [] (size_t i) const noexcept;

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
    inline Type dot(const Vec3<Type> v1, const Vec3<Type>& v2) noexcept;

    template<typename Type>
    inline Vec3<Type> cross(const Vec3<Type> v1, const Vec3<Type>& v2) noexcept;

    template<typename Type>
    Vec3<Type> normalize(const Vec3<Type>& v);
}

#include "vec3.inl"
