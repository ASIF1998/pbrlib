#include <pbrlib/math/aabb.hpp>

#include <pbrlib/utils/combine_hash.hpp>

namespace pbrlib::math
{
    vec3& AABB::operator [] (size_t i)
    {
        if (i > 1) [[unlikely]]
            throw exception::InvalidArgument("[aabb] invalid index in operator []");
        
        return i == 0 ? p_min : p_max;
    }
    
    const vec3& AABB::operator [] (size_t i) const
    {
        if (i > 1) [[unlikely]]
            throw exception::InvalidArgument("[aabb] invalid index in operator []");
        
        return i == 0 ? p_min : p_max;
    }

    vec3 AABB::corner(size_t i) const
    {
        if (i > 7) [[unlikely]]
            throw exception::InvalidArgument("[aabb] invalid index in method 'corner(index)'");

        const auto x = (*this)[i & 1].x;
        const auto y = (*this)[(i & 2) ? 1 : 0].y;
        const auto z = (*this)[(i & 4) ? 1 : 0].z;

        return vec3(x, y, z);
    }

    vec3 AABB::diagonal() const noexcept
    {
        return p_max - p_min;
    }

    float AABB::surfaceArea() const noexcept
    {
        const auto d = diagonal();
        return 2.0f * (d.x * d.y + d.x * d.z + d.y * d.z);
    }

    float AABB::volume() const noexcept
    {
        const auto d = diagonal();
        return d.x * d.y * d.z;
    }

    bool AABB::empty() const noexcept
    {
        return p_min.x > p_max.x || p_min.y > p_max.y || p_min.z > p_max.z;
    }

    void AABB::add(const pbrlib::math::vec3& p)
    {
        p_min = pbrlib::math::vec3(std::min(p_min.x, p.x), std::min(p_min.y, p.y), std::min(p_min.z, p.z));
        p_max = pbrlib::math::vec3(std::max(p_max.x, p.x), std::max(p_max.y, p.y), std::max(p_max.z, p.z));
    }
}

namespace std
{
    size_t hash<pbrlib::math::AABB>::operator () (const pbrlib::math::AABB& aabb) const noexcept
    {
        size_t hash_value = 0;
        pbrlib::combineHash(hash_value, aabb.p_min);
        pbrlib::combineHash(hash_value, aabb.p_max);
        
        return hash_value;
    }
}