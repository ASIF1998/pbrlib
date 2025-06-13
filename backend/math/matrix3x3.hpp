#pragma once

namespace pbrlib::math
{
    template<typename Type>
    struct Vec3;

    template<typename Type>
    class Matrix3x3
    {
    public:
        inline constexpr Matrix3x3();
        inline constexpr Matrix3x3(Type init_value);

        inline constexpr Matrix3x3(
            Type x0, Type y0, Type z0,
            Type x1, Type y1, Type z1,
            Type x2, Type y2, Type z2 
        );

        inline bool operator == (const Matrix3x3& mat) const;
        inline bool operator != (const Matrix3x3& mat) const;

        inline Matrix3x3    operator + (const Matrix3x3& mat)   const;
        inline Matrix3x3    operator - (const Matrix3x3& mat)   const;
        inline Matrix3x3    operator * (const Matrix3x3& mat)   const;
        inline Matrix3x3    operator * (Type scal)              const;
        inline Vec3<Type>   operator * (const Vec3<Type>& v)    const;

        inline Matrix3x3& operator += (const Matrix3x3& mat);
        inline Matrix3x3& operator -= (const Matrix3x3& mat);
        inline Matrix3x3& operator *= (const Matrix3x3& mat);
        inline Matrix3x3& operator *= (Type scal);

        inline Type*        operator [] (size_t i);
        inline const Type*  operator [] (size_t i) const;

        inline Type&    at(size_t i, size_t j);
        inline Type     at(size_t i, size_t j) const;

        inline Type det() const;

        inline void transpose();
        inline void inverse();

    private:
        union
        {
            Type _array3x3[3][3];
            Type _array9[9];
        };
    };

    template<typename Type>
    inline Matrix3x3<Type> transpose(const Matrix3x3<Type>& mat);

    template<typename Type>
    inline Matrix3x3<Type> inverse(const Matrix3x3<Type>& mat);
}


#include "matrix3x3.inl"