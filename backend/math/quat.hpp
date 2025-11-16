#pragma once

#include "vec3.hpp"

#include <pbrlib/math/matrix4x4.hpp>

namespace pbrlib::math
{
    struct Quaternion final
    {
        inline constexpr Quaternion()                                   noexcept;
        inline constexpr Quaternion(float x, float y, float z, float w) noexcept;
        inline constexpr Quaternion(const vec3& v, float w)             noexcept;

        inline constexpr bool operator == (const Quaternion& q) const noexcept;
        inline constexpr bool operator != (const Quaternion& q) const noexcept;

        inline constexpr Quaternion operator + (const Quaternion& q)    const noexcept;
        inline constexpr Quaternion operator - (const Quaternion& q)    const noexcept;
        inline constexpr Quaternion operator * (const Quaternion& q)    const noexcept;
        inline constexpr Quaternion operator / (float f)                const noexcept;

        inline constexpr Quaternion& operator += (const Quaternion& q)  noexcept;
        inline constexpr Quaternion& operator -= (const Quaternion& q)  noexcept;
        inline constexpr Quaternion& operator *= (const Quaternion& q)  noexcept;
        inline constexpr Quaternion& operator *= (float f)              noexcept;
        inline constexpr Quaternion& operator /= (float f)              noexcept;

        inline float    operator [] (size_t i) const noexcept;
        inline float&   operator [] (size_t i) noexcept;

        inline constexpr float  lengthSquared() const noexcept;
        inline float            length()        const noexcept;

        inline void             normalize();
        inline constexpr void   inverse();

        inline constexpr mat4 toMatrix() const noexcept;

        /**
         * @brief a static method that creates the following quaternion:
         *      (1, 0, 0, 0).
         * 
         * @return a quaternion of the following form: (1, 0, 0, 0).
        */
        inline constexpr static Quaternion i() noexcept;

        /**
         * @brief a static method that creates the following quaternion:
         *      (0, 1, 0, 0).
         * 
         * @return a quaternion of the following form: (0, 1, 0, 0).
        */
        inline constexpr static Quaternion j() noexcept;

        /**
         * @brief a static method that creates the following quaternion:
         *      (0, 0, 1, 0).
         * 
         * @return a quaternion of the following form: (0, 0, 1, 0).
        */
        inline constexpr static Quaternion k() noexcept;

        /**
         * @brief a static method that creates the following quaternion:
         *      (0, 0, 0, 1).
         * 
         * @return a quaternion of the following form: (0, 0, 0, 1).
        */
        inline constexpr static Quaternion identity() noexcept;

        /**
         * @brief a static method that creates the following quaternion:
         *      (0, 0, 0, 0).
         * 
         * @return a quaternion of the following form: (0, 0, 0, 0).
        */
        inline constexpr static Quaternion zerro() noexcept;

        union
        {
            struct
            {
                vec3    v;
                float   w;
            };

            float   xyzw[4];
        };
    };

    inline constexpr float      dot(const Quaternion& q1, const Quaternion& q2) noexcept;
    inline Quaternion           normalize(const Quaternion& q);
    inline constexpr Quaternion inverse(const Quaternion& q);

    inline constexpr Quaternion operator * (const Quaternion& q, float s);
    inline constexpr Quaternion operator * (float s, const Quaternion& q);

    /**
     * @return (q.v * -1.0f, q.w).
    */
    inline constexpr Quaternion conjugate(const Quaternion& q);
}

#include <backend/math/quat.inl>
