#include <cassert>
#include <cmath>

namespace pbrlib::math
{
    template<typename Type>
    inline constexpr Vec3<Type>::Vec3(Type xyz) :
        x{xyz},
        y{xyz},
        z{xyz}
    {}

    template<typename Type>
    inline constexpr Vec3<Type>::Vec3(Type x, Type y, Type z) :
        x{x},
        y{y},
        z{z}
    {}

    template<typename Type>
    inline constexpr Vec3<Type>::Vec3(const Vec2<Type>& vec2, Type z) :
        x{vec2.x},
        y{vec2.y},
        z{z}
    {}

    template<typename Type>
    inline bool Vec3<Type>::operator == (const Vec3<Type>& v) const
    {
        return x == v.x && y == v.y && z == v.z;
    }

    template<typename Type>
    inline bool Vec3<Type>::operator != (const Vec3<Type>& v) const
    {
        return x != v.x || y != v.y || z != v.z;
    }

    template<typename Type>
    inline Vec3<Type> Vec3<Type>::operator + (const Vec3<Type>& v) const
    {
        return Vec3<Type>(x + v.x, y + v.y, z + v.z);
    }

    template<typename Type>
    inline Vec3<Type> Vec3<Type>::operator - (const Vec3<Type>& v) const
    {
        return Vec3<Type>(x - v.x, y - v.y, z - v.z);
    }

    template<typename Type>
    inline Vec3<Type> Vec3<Type>::operator * (const Type s) const
    {
        return Vec3<Type>(x * s, y * s, z * s);
    }

    template<typename Type>
    inline Vec3<Type>& Vec3<Type>::operator += (const Vec3<Type>& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;

        return *this;
    }

    template<typename Type>
    inline Vec3<Type>& Vec3<Type>::operator -= (const Vec3<Type>& v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;

        return *this;
    }

    template<typename Type>
    inline Vec3<Type>& Vec3<Type>::operator *= (Type s)
    {
        x *= s;
        y *= s;
        z *= s;

        return *this;
    }

    template<typename Type>
    Type& Vec3<Type>::operator [] (size_t i) noexcept
    {
        return xyz[i];
    }

    template<typename Type>
    Type Vec3<Type>::operator [] (size_t i) const noexcept
    {
        return xyz[i];
    }

    template<typename Type>
    inline Type Vec3<Type>::lengthSquared() const
    {
        return x * x + y * y + z * z;
    }

    template<typename Type>
    inline Type Vec3<Type>::length() const
    {
        return std::sqrt(x * x + y * y + z * z);
    }

    template<typename Type>
    void Vec3<Type>::normalize()
    {
        auto l = length();
        
        assert(l);

        x /= l;
        y /= l;
        z /= l;
    }

    template<typename Type>
    inline Type dot(const Vec3<Type> v1, const Vec3<Type>& v2)
    {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }

    template<typename Type>
    inline Vec3<Type> cross(const Vec3<Type> v1, const Vec3<Type>& v2)
    {
        return Vec3<Type>(
            v1.y * v2.z - v1.z * v2.y,
            v1.z * v2.x - v1.x * v2.z,
            v1.x * v2.y - v1.y * v2.x
        );
    }

    template<typename Type>
    Vec3<Type> normalize(const Vec3<Type>& v)
    {
        auto l = v.length();
        assert(l != static_cast<Type>(0));

        return Vec3<Type>(v.x / l, v.y / l, v.z / l);
    }
}
