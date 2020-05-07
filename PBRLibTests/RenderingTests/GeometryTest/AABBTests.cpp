//
//  AABBTests.cpp
//  PBRLibTests
//
//  Created by Асиф Мамедов on 19/04/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <numeric>

#include <gtest/gtest.h>

#include "../../../src/Rendering/Geometry/AABB.hpp"
#include "../../../src/math/vec3.hpp"

using namespace testing;
using namespace pbrlib;
using namespace pbrlib::math;
using namespace std;

TEST(RenderingGeometryAABB, Constructor)
{
    constexpr Vec3<float> p (0.5f, 0.5f, 0.5f);

    constexpr Vec3<float> p1 (-1.0f, 1.0f, -1.0f);
    constexpr Vec3<float> p2 (1.0f, -1.0f, 1.0f);

    constexpr float min_value = numeric_limits<float>::lowest();
    constexpr float max_value = numeric_limits<float>::max();

    AABB bbox1;
    AABB bbox2 (p);
    AABB bbox3 (p1, p2);

    EXPECT_EQ(bbox1[0].x, min_value);
    EXPECT_EQ(bbox1[0].y, min_value);
    EXPECT_EQ(bbox1[0].z, min_value);
    EXPECT_EQ(bbox1[1].x, max_value);
    EXPECT_EQ(bbox1[1].y, max_value);
    EXPECT_EQ(bbox1[1].z, max_value);

    EXPECT_EQ(bbox2[0], p);
    EXPECT_EQ(bbox2[1], p);

    EXPECT_EQ(bbox3[0].x, -1.0f);
    EXPECT_EQ(bbox3[0].y, -1.0f);
    EXPECT_EQ(bbox3[0].z, -1.0f);
    EXPECT_EQ(bbox3[1].x, 1.0f);
    EXPECT_EQ(bbox3[1].y, 1.0f);
    EXPECT_EQ(bbox3[1].z, 1.0f);
}

TEST(RenderingGeometryAABB, EqualAndNotEqual)
{
    constexpr Vec3<float> p (0.5f, 0.5f, 0.5f);

    AABB bbox1 (p);
    AABB bbox2 (p);

    EXPECT_TRUE(bbox1 == bbox2);

    bbox2[0].y = -1.0f;

    EXPECT_TRUE(bbox1 != bbox2);
}

TEST(RenderingGeometryAABB, Corner)
{
    constexpr Vec3<float> r0 (-1.0f, -1.0f, -1.0f);
    constexpr Vec3<float> r1 (1.0f, -1.0f, -1.0f);
    constexpr Vec3<float> r2 (-1.0f, 1.0f, -1.0f);
    constexpr Vec3<float> r3 (1.0f, 1.0f, -1.0f);
    constexpr Vec3<float> r4 (-1.0f, -1.0f, 1.0f);
    constexpr Vec3<float> r5 (1.0f, -1.0f, 1.0f);
    constexpr Vec3<float> r6 (-1.0f, 1.0f, 1.0f);
    constexpr Vec3<float> r7 (1.0f, 1.0f, 1.0f);

    constexpr Vec3<float> p1 (-1.0f, 1.0f, -1.0f);
    constexpr Vec3<float> p2 (1.0f, -1.0f, 1.0f);

    AABB bbox (p1, p2);

    EXPECT_TRUE(bbox.corner(0) == r0);
    EXPECT_TRUE(bbox.corner(1) == r1);
    EXPECT_TRUE(bbox.corner(2) == r2);
    EXPECT_TRUE(bbox.corner(3) == r3);
    EXPECT_TRUE(bbox.corner(4) == r4);
    EXPECT_TRUE(bbox.corner(5) == r5);
    EXPECT_TRUE(bbox.corner(6) == r6);
    EXPECT_TRUE(bbox.corner(7) == r7);
}

TEST(RenderingGeometryAABB, Diagonal)
{
    constexpr Vec3<float> r (2.0f, 2.0f, 2.0f);
    
    constexpr Vec3<float> p1 (-1.0f, 1.0f, -1.0f);
    constexpr Vec3<float> p2 (1.0f, -1.0f, 1.0f);

    AABB bbox (p1, p2);

    EXPECT_EQ(r, bbox.diagonal());
}

TEST(RenderingGeometryAABB, SurfaceArea)
{
    constexpr float r = 24.0f;

    constexpr Vec3<float> p1 (-1.0f, 1.0f, -1.0f);
    constexpr Vec3<float> p2 (1.0f, -1.0f, 1.0f);

    AABB bbox (p1, p2);

    EXPECT_EQ(r, bbox.surfaceArea());
}

TEST(RenderingGeometryAABB, Volume)
{
    constexpr float r = 8.0f;

    constexpr Vec3<float> p1 (-1.0f, 1.0f, -1.0f);
    constexpr Vec3<float> p2 (1.0f, -1.0f, 1.0f);

    AABB bbox (p1, p2);

    EXPECT_EQ(r, bbox.volume());
}

TEST(RenderingGeometryAABB, MaximumExtend)
{
    constexpr size_t r = 1;

    constexpr Vec3<float> p1 (0.0f, 1.0f, 0.0f);
    constexpr Vec3<float> p2 (0.0f);

    AABB bbox (p1, p2);

    EXPECT_EQ(r, bbox.maximumExtent());
}

TEST(RenderingGeometryAABB,  Lerp)
{
    constexpr Vec3<float> r (0.5f);

    constexpr Vec3<float> p1 (1.0f);
    constexpr Vec3<float> p2 (0.0f);

    constexpr Vec3<float> t (0.5f);

    AABB bbox (p1, p2);

    EXPECT_EQ(r, bbox.lerp(t));
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

    EXPECT_EQ(r1, res1);
    EXPECT_EQ(r2, res2);
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

    EXPECT_EQ(r, AABB::intersect(bbox1, bbox2));
}

TEST(RenderingGeometryAABB, Overlaps)
{
    constexpr Vec3<float> p1 (0.0f);
    constexpr Vec3<float> p2 (0.6f);
    constexpr Vec3<float> p3 (0.5f);
    constexpr Vec3<float> p4 (1.0f);

    AABB bbox1 (p1, p2);
    AABB bbox2 (p3, p4);

    EXPECT_EQ(true, AABB::overlaps(bbox1, bbox2));
}

TEST(RenderingGeometryAABB, Inside)
{
    constexpr Vec3<float> p1 (0.0f);
    constexpr Vec3<float> p2 (0.6f);
    constexpr Vec3<float> p3 (0.5f);

    AABB bbox (p1, p2);

    EXPECT_EQ(true, AABB::inside(bbox, p3));
}

TEST(RenderingGeometryAABB, Expand)
{
    constexpr float         delta   (0.5f);
    constexpr Vec3<float>   rp1     (delta);
    constexpr Vec3<float>   rp2     (-delta);

    AABB r(rp1, rp2);

    constexpr Vec3<float> p (0.0f);

    AABB bbox (p);

    EXPECT_EQ(r, AABB::expand(bbox, delta));
}
