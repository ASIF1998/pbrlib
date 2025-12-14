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

        [[nodiscard]] vec3 diagonal() const noexcept;

        [[nodiscard]] float surfaceArea()   const noexcept;
        [[nodiscard]] float volume()        const noexcept;

        [[nodiscard]] bool empty() const noexcept;

        void add(const pbrlib::math::vec3& p);

        vec3 p_min;
        vec3 p_max;
    };
}

namespace std
{
    template<>
    struct hash<pbrlib::math::AABB> final
    {
        size_t operator () (const pbrlib::math::AABB& aabb) const noexcept;
    };
}

#include "aabb.inl"