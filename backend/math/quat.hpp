#pragma once

#if (defined(__SSE__) || defined(__AVX2__))
#   include <xmmintrin.h>
#endif

#include "vec3.hpp"

#include <pbrlib/transform.hpp>

namespace pbrlib::math
{
    struct Quaternion final
    {
    public:
        inline constexpr Quaternion()                                   noexcept;
        inline constexpr Quaternion(float x, float y, float z, float w) noexcept;
        inline constexpr Quaternion(const vec3& v, float w)             noexcept;

        inline bool operator == (const Quaternion& q) const noexcept;
        inline bool operator != (const Quaternion& q) const noexcept;

        inline Quaternion operator + (const Quaternion& q)  const noexcept;
        inline Quaternion operator - (const Quaternion& q)  const noexcept;
        inline Quaternion operator * (const Quaternion& q)  const noexcept;
        inline Quaternion operator * (float f)              const noexcept;
        inline Quaternion operator / (float f)              const noexcept;

        inline Quaternion& operator += (const Quaternion& q)    noexcept;
        inline Quaternion& operator -= (const Quaternion& q)    noexcept;
        inline Quaternion& operator *= (const Quaternion& q)    noexcept;
        inline Quaternion& operator *= (float f)                noexcept;
        inline Quaternion& operator /= (float f)                noexcept;

        inline float    operator [] (size_t i) const noexcept;
        inline float&   operator [] (size_t i) noexcept;

        inline float lengthSquared()    const noexcept;
        inline float length()           const noexcept;

        inline void normalize();
        inline void inverse();

        inline mat4 toMatrix() const noexcept;

        /**
         * @brief Статический метод, который создаёт следющий кватернион:
         *      (1, 0, 0, 0).
         * 
         * @return Кватернион следующего вида: (1, 0, 0, 0).
        */
        inline static Quaternion i() noexcept;

        /**
         * @brief Статический метод, который создаёт следющий кватернион:
         *      (0, 1, 0, 0).
         * 
         * @return Кватернион следующего вида: (0, 1, 0, 0).
        */
        inline static Quaternion j() noexcept;

        /**
         * @brief Статический метод, который создаёт следющий кватернион:
         *      (0, 0, 1, 0).
         * 
         * @return Кватернион следующего вида: (0, 0, 1, 0).
        */
        inline static Quaternion k() noexcept;

        /**
         * @brief Статический метод, который создаёт следющий кватернион:
         *      (0, 0, 0, 1).
         * 
         * @return Кватернион следующего вида: (0, 0, 0, 1).
        */
        inline static Quaternion identity() noexcept;

        /**
         * @brief Статический метод, который создаёт следющий кватернион:
         *      (0, 0, 0, 0).
         * 
         * @return Кватернион следующего вида: (0, 0, 0, 0).
        */
        inline static Quaternion zerro() noexcept;

#if (defined(__SSE__) || defined(__AVX2__))
    private:
        inline constexpr Quaternion(const __m128& m) noexcept;
#endif

    public:
        union
        {
            struct
            {
                vec3    v;
                float   w;
            };

#if (defined(__SSE__) || defined(__AVX2__))
            __m128  xyzw_simd;
#endif

            float   xyzw[4];
        };
    };

    inline float        dot(const Quaternion& q1, const Quaternion& q2) noexcept;
    inline Quaternion   normalize(const Quaternion& q);
    inline Quaternion   inverse(const Quaternion& q);

    /**
     * @brief Функция возвращающая сопряжённый кватернион.
     * 
     * @param q кватернион.
     * @return Следующий кватернион: (q.v * -1.0f, q.w).
    */
    inline Quaternion conjugate(const Quaternion& q);
}

#include "quat.inl"
