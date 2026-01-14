#include <limits>

namespace pbrlib::math::utils
{
    static constexpr auto v_max = std::numeric_limits<float>::max();
    static constexpr auto v_min = std::numeric_limits<float>::min();
}

namespace pbrlib::math
{
    inline constexpr AABB::AABB() :
        p_min(v_max, v_max, v_max),
        p_max(v_min, v_min, v_min)
    { }

    inline constexpr AABB::AABB(const vec3& p) :
        p_min(p),
        p_max(p)
    { }

    inline constexpr AABB::AABB(const vec3& p1, const vec3& p2) :
        p_min(std::min(p1.x, p2.x), std::min(p1.y, p2.y), std::min(p1.z, p2.z)),
        p_max(std::max(p1.x, p2.x), std::max(p1.y, p2.y), std::max(p1.z, p2.z))
    { }
}
