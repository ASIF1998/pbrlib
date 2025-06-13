#pragma once

#include <pbrlib/math/vec3.hpp>

namespace pbrlib::math
{
    struct AABB
    {
        explicit constexpr AABB();
        explicit constexpr AABB(const vec3& p);
        explicit constexpr AABB(const vec3& p1, const vec3& p2);

        [[nodiscard]] vec3&         operator [] (size_t i);
        [[nodiscard]] const vec3&   operator [] (size_t i) const;

        [[nodiscard]] vec3 corner(size_t i) const;

        [[nodiscard]] vec3 diagonal() const;

        [[nodiscard]] float surfaceArea()   const;
        [[nodiscard]] float volume()        const;

        [[nodiscard]] bool empty() const noexcept;

        void add(const pbrlib::math::vec3& p);

        vec3 p_min;
        vec3 p_max;
    };
}

#include "aabb.inl"