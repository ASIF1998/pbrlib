//
//  AABB.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 19/04/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <pbrlib/Rendering/Geometry/AABB.hpp>

#include <algorithm>
#include <numeric>

#include <cassert>

using namespace std;

namespace pbrlib
{
    AABB::AABB() :
        _p_min(numeric_limits<float>::lowest()),
        _p_max(numeric_limits<float>::max())
    {}

    AABB::AABB(const Vec3<float>& p) :
        _p_min(p),
        _p_max(p)
    {}

    AABB::AABB(const Vec3<float>& p1, const Vec3<float>& p2) :
        _p_min(min(p1.x, p2.x), min(p1.y, p2.y), min(p1.z, p2.z)),
        _p_max(max(p1.x, p2.x), max(p1.y, p2.y), max(p1.z, p2.z))
    {}

    Vec3<float>& AABB::operator [] (size_t i)
    {
        assert(i == 0 || i == 1);
        return _min_max[i];
    }

    Vec3<float> AABB::operator [] (size_t i) const
    {
        assert(i == 0 || i == 1);
        return _min_max[i];
    }

    bool AABB::operator == (const AABB& bbox) const
    {
        return _p_min == bbox._p_min && _p_max == bbox._p_max;
    }

    bool AABB::operator != (const AABB& bbox) const
    {
        return _p_min != bbox._p_min || _p_max != bbox._p_max;
    }

    Vec3<float> AABB::corner(size_t c) const
    {
        assert(c >= 0 && c < 8);

        return Vec3<float>(
            _min_max[c & 1].x,
            _min_max[(c & 2) ? 1 : 0].y,
            _min_max[(c & 4) ? 1 : 0].z
        );
    }

    Vec3<float> AABB::diagonal() const
    {
        return _p_max - _p_min;
    }

    float AABB::surfaceArea() const
    {
        auto d = diagonal();
        return 2.0f * (d.x * d.y + d.x * d.z + d.y * d.z);
    }

    float AABB::volume() const
    {
        auto d = diagonal();
        return d.x * d.y * d.z;
    }

    uint32_t AABB::maximumExtent() const
    {
        auto d = diagonal();

        if (d.x > d.y && d.x > d.z) {
            return 0;
        } else if (d.y > d.z) {
            return 1;
        }

        return 2;
    }

    Vec3<float> AABB::lerp(const Vec3<float>& t) const
    {
        return Vec3<float> (
            _p_min.x * (1.0f - t.x) + _p_max.x * t.x,
            _p_min.y * (1.0f - t.y) + _p_max.y * t.y,
            _p_min.z * (1.0f - t.z) + _p_max.z * t.z
        );
    }

    AABB AABB::aabbUnion(const AABB& bbox, const Vec3<float>& p)
    {
        return AABB (
            Vec3<float> (
                min(bbox._p_min.x, p.x),
                min(bbox._p_min.y, p.y),
                min(bbox._p_min.z, p.z)
            ),
            Vec3<float> (
                max(bbox._p_max.x, p.x),
                max(bbox._p_max.y, p.y),
                max(bbox._p_max.z, p.z)
            )
        );
    }

    AABB AABB::aabbUnion(const AABB& bbox1, const AABB& bbox2)
    {
        return AABB (
            Vec3<float> (
                min(bbox1._p_min.x, bbox2._p_min.x),
                min(bbox1._p_min.y, bbox2._p_min.y),
                min(bbox1._p_min.z, bbox2._p_min.z)
            ),
            Vec3<float> (
                max(bbox1._p_max.x, bbox2._p_max.x),
                max(bbox1._p_max.y, bbox2._p_max.y),
                max(bbox1._p_max.z, bbox2._p_max.z)
            )
        );
    }

    AABB AABB::intersect(const AABB& bbox1, const AABB& bbox2)
    {
        return AABB (
            Vec3<float> (
                max(bbox1._p_min.x, bbox2._p_min.x),
                max(bbox1._p_min.y, bbox2._p_min.y),
                max(bbox1._p_min.z, bbox2._p_min.z)
            ),
            Vec3<float> (
                min(bbox1._p_max.x, bbox2._p_max.x),
                min(bbox1._p_max.y, bbox2._p_max.y),
                min(bbox1._p_max.z, bbox2._p_max.z)
            )
        );
    }

    bool AABB::overlaps(const AABB& bbox1, const AABB& bbox2)
    {
        bool x =    bbox1._p_max.x >= bbox2._p_min.x && 
                    bbox1._p_min.x <= bbox2._p_max.x;

        bool y =    bbox1._p_max.y >= bbox2._p_min.y && 
                    bbox1._p_min.y <= bbox2._p_max.y;

        bool z =    bbox1._p_max.z >= bbox2._p_min.z && 
                    bbox1._p_min.z <= bbox2._p_max.z;

        return x && y && z;
    }

    bool AABB::inside(const AABB& bbox, const Vec3<float>& p)
    {
        bool x = p.x >= bbox._p_min.x && p.x <= bbox._p_max.x;
        bool y = p.y >= bbox._p_min.y && p.y <= bbox._p_max.y;
        bool z = p.z >= bbox._p_min.z && p.z <= bbox._p_max.z;

        return x && y && z;
    }

    AABB AABB::expand(const AABB& bbox, float delta)
    {
        Vec3<float> vdelta (delta);
        return AABB (
            bbox._p_min - vdelta,
            bbox._p_max + vdelta
        );
    }
}
