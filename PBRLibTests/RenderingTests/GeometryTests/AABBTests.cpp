//
//  AABBTests.cpp
//  PBRLibTests
//
//  Created by Асиф Мамедов on 19/04/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <numeric>

#include "../../utils.hpp"

#include "../../../src/Rendering/Geometry/AABB.hpp"
#include "../../../src/math/vec3.hpp"

using namespace pbrlib;
using namespace pbrlib::math;
using namespace std;

TEST(RenderingGeometryAABB, Constructor)
{
    constexpr Vec3<float> p (0.5f, 0.5f, 0.5f);

    constexpr Vec3<float> p1 (-1.0f, 1.00f, -1.0f);
    constexpr Vec3<float> p2 (1.00f, -1.0f, 1.00f);

    constexpr float min_value = numeric_limits<float>::lowest();
    constexpr float max_value = numeric_limits<float>::max();

    AABB bbox1;
    AABB bbox2 (p);
    AABB bbox3 (p1, p2);

    pbrlib::testing::utils::equality(bbox1[0], Vec3<float>(min_value));
    pbrlib::testing::utils::equality(bbox1[1], Vec3<float>(max_value));
    pbrlib::testing::utils::equality(bbox2[0], p);
    pbrlib::testing::utils::equality(bbox2[1], p);
    pbrlib::testing::utils::equality(bbox3[0], Vec3<float>(-1.0f));
    pbrlib::testing::utils::equality(bbox3[1], Vec3<float>(1.0f));
}

TEST(RenderingGeometryAABB, EqualAndNotEqual)
{
    constexpr Vec3<float> p (0.5f, 0.5f, 0.5f);

    AABB bbox1 (p);
    AABB bbox2 (p);

    pbrlib::testing::utils::thisTrue(bbox1 == bbox2);

    bbox2[0].y = -1.0f;

    pbrlib::testing::utils::thisTrue(bbox1 != bbox2);
}

TEST(RenderingGeometryAABB, Corner)
{
    constexpr Vec3<float> r0 (-1.0f, -1.0f, -1.0f);
    constexpr Vec3<float> r1 (1.00f, -1.0f, -1.0f);
    constexpr Vec3<float> r2 (-1.0f, 1.00f, -1.0f);
    constexpr Vec3<float> r3 (1.00f, 1.00f, -1.0f);
    constexpr Vec3<float> r4 (-1.0f, -1.0f, 1.00f);
    constexpr Vec3<float> r5 (1.00f, -1.0f, 1.00f);
    constexpr Vec3<float> r6 (-1.0f, 1.00f, 1.00f);
    constexpr Vec3<float> r7 (1.00f, 1.00f, 1.00f);

    constexpr Vec3<float> p1 (-1.0f, 1.00f, -1.0f);
    constexpr Vec3<float> p2 (1.00f, -1.0f, 1.00f);

    AABB bbox (p1, p2);

    pbrlib::testing::utils::equality(bbox.corner(0), r0);
    pbrlib::testing::utils::equality(bbox.corner(1), r1);
    pbrlib::testing::utils::equality(bbox.corner(2), r2);
    pbrlib::testing::utils::equality(bbox.corner(3), r3);
    pbrlib::testing::utils::equality(bbox.corner(4), r4);
    pbrlib::testing::utils::equality(bbox.corner(5), r5);
    pbrlib::testing::utils::equality(bbox.corner(6), r6);
    pbrlib::testing::utils::equality(bbox.corner(7), r7);
}

TEST(RenderingGeometryAABB, Diagonal)
{
    constexpr Vec3<float> r (2.0f, 2.0f, 2.0f);
    
    constexpr Vec3<float> p1 (-1.0f, 1.00f, -1.0f);
    constexpr Vec3<float> p2 (1.00f, -1.0f, 1.00f);

    AABB bbox (p1, p2);

    pbrlib::testing::utils::equality(r, bbox.diagonal());
}

TEST(RenderingGeometryAABB, SurfaceArea)
{
    constexpr float r = 24.0f;

    constexpr Vec3<float> p1 (-1.0f, 1.00f, -1.0f);
    constexpr Vec3<float> p2 (1.00f, -1.0f, 1.00f);

    AABB bbox (p1, p2);

    pbrlib::testing::utils::equality(r, bbox.surfaceArea());
}

TEST(RenderingGeometryAABB, Volume)
{
    constexpr float r = 8.0f;

    constexpr Vec3<float> p1 (-1.0f, 1.00f, -1.0f);
    constexpr Vec3<float> p2 (1.00f, -1.0f, 1.00f);

    AABB bbox (p1, p2);

    pbrlib::testing::utils::equality(r, bbox.volume());
}

TEST(RenderingGeometryAABB, MaximumExtend)
{
    constexpr size_t r = 1;

    constexpr Vec3<float> p1 (0.0f, 1.0f, 0.0f);
    constexpr Vec3<float> p2 (0.0f);

    AABB bbox (p1, p2);

    pbrlib::testing::utils::equality(r, bbox.maximumExtent());
}

TEST(RenderingGeometryAABB,  Lerp)
{
    constexpr Vec3<float> r (0.5f);

    constexpr Vec3<float> p1 (1.0f);
    constexpr Vec3<float> p2 (0.0f);

    constexpr Vec3<float> t (0.5f);

    AABB bbox (p1, p2);

    pbrlib::testing::utils::equality(r, bbox.lerp(t));
}

TEST(RenderingGeometryAABB, Union)
{
    constexpr Vec3<float> p1 (0.0f);
    constexpr Vec3<float> p2 (0.5f);
    constexpr Vec3<float> p3 (1.0f);
    constexpr Vec3<float> p4 (2.0f);

    AABB r1 (p1, p3);
    AABB r2 (p1, p4);

    AABB bbox1 (p1, p2);
    AABB bbox2 (p2, p3);

    AABB res1 = AABB::aabbUnion(bbox1, bbox2);
    AABB res2 = AABB::aabbUnion(res1, p4);

    pbrlib::testing::utils::equality(r1[0], res1[0]);
    pbrlib::testing::utils::equality(r1[1], res1[1]);
    pbrlib::testing::utils::equality(r2[0], res2[0]);
    pbrlib::testing::utils::equality(r2[1], res2[1]);
}

TEST(RenderingGeometryAABB, Intersect)
{
    constexpr Vec3<float> p1 (0.0f);
    constexpr Vec3<float> p2 (0.6f);
    constexpr Vec3<float> p3 (0.5f);
    constexpr Vec3<float> p4 (1.0f);

    AABB r (p3, p2);

    AABB bbox1 (p1, p2);
    AABB bbox2 (p3, p4);

    AABB res = AABB::intersect(bbox1, bbox2);

    pbrlib::testing::utils::equality(r[0], res[0]);
    pbrlib::testing::utils::equality(r[1], res[1]);
}

TEST(RenderingGeometryAABB, Overlaps)
{
    constexpr Vec3<float> p1 (0.0f);
    constexpr Vec3<float> p2 (0.6f);
    constexpr Vec3<float> p3 (0.5f);
    constexpr Vec3<float> p4 (1.0f);

    AABB bbox1 (p1, p2);
    AABB bbox2 (p3, p4);

    pbrlib::testing::utils::equality(true, AABB::overlaps(bbox1, bbox2));
}

TEST(RenderingGeometryAABB, Inside)
{
    constexpr Vec3<float> p1 (0.0f);
    constexpr Vec3<float> p2 (0.6f);
    constexpr Vec3<float> p3 (0.5f);

    AABB bbox (p1, p2);

    pbrlib::testing::utils::equality(true, AABB::inside(bbox, p3));
}

TEST(RenderingGeometryAABB, Expand)
{
    constexpr float         delta   (0.5f);
    constexpr Vec3<float>   rp1     (delta);
    constexpr Vec3<float>   rp2     (-delta);

    AABB r(rp1, rp2);

    constexpr Vec3<float> p (0.0f);

    AABB bbox (p);

    AABB res = AABB::expand(bbox, delta);

    pbrlib::testing::utils::equality(r[0], res[0]);
    pbrlib::testing::utils::equality(r[1], res[1]);
}
