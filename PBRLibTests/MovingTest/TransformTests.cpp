//
//  TransformTests.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 17/04/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <gtest/gtest.h>

#include "../../src/Moving/Transform.hpp"
#include "../../src/math/vec3.hpp"

using namespace testing;
using namespace pbrlib;

TEST(MovingTransform, Constructor)
{
    constexpr Matrix4x4<float> tm1 (
        1.0f, 0.0f, 0.0f, 0.0f, 
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );

    constexpr Matrix4x4<float> tm2 (
        1.0f, 2.0f, 3.0f, 4.0f,
        4.0f, 3.0f, 2.0f, 1.0f,
        5.0f, 6.0f, 7.0f, 8.0f,
        8.0f, 7.0f, 6.0f, 5.0f
    );

    Transform t1;
    Transform t2 (tm2);

    EXPECT_EQ(tm1, t1.getMatrix());
    EXPECT_EQ(tm2, t2.getMatrix());
}

TEST(MovingTransform, EqualAndNotEqual)
{
    constexpr Matrix4x4<float> tm (
        1.0f, 2.0f, 3.0f, 4.0f,
        4.0f, 3.0f, 2.0f, 1.0f,
        5.0f, 6.0f, 7.0f, 8.0f,
        8.0f, 7.0f, 6.0f, 5.0f
    );

    Transform t1;
    Transform t2;

    EXPECT_TRUE(t1 == t2);

    t2.setMatrix(tm);

    EXPECT_TRUE(t1 != t2);
}

TEST(MovingTransform, GetMatrixAndInverseMatrix)
{
    constexpr Matrix4x4<float> tm (
        1.0f, 2.0f, 3.0f, 4.0f,
        4.0f, 3.0f, 2.0f, 1.0f,
        5.0f, 6.0f, 7.0f, 8.0f,
        8.0f, 7.0f, 6.0f, 5.0f
    );

    Transform t (tm);

    EXPECT_EQ(tm, t.getMatrix());
    EXPECT_EQ(inverse(tm), t.getInverseMatrix());
}

TEST(MovingTransform, Translate)
{
    constexpr Vec3<float> t (0.5f, 0.5f, 0.5f);
    
    Transform transform = Transform::translate(t);

    constexpr Matrix4x4<float> r (
        1.0f, 0.0f, 0.0f, 0.5f,
        0.0f, 1.0f, 0.0f, 0.5f,
        0.0f, 0.0f, 1.0f, 0.5f, 
        0.0f, 0.0f, 0.0f, 1.0f
    );

    EXPECT_EQ(r, transform.getMatrix());
}

TEST(MovingTransform, Scale)
{
    constexpr Vec3<float> t (0.5f, 0.5f, 0.5f);
    
    Transform transform = Transform::scale(t);

    constexpr Matrix4x4<float> r (
        0.5f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.5f, 0.0f, 
        0.0f, 0.0f, 0.0f, 1.0f
    );

    EXPECT_EQ(r, transform.getMatrix());
}

TEST(MovingTransform, Rotates)
{
    constexpr Vec3<float> r1 (0.4767313f, -0.291967213f, 0.82914561f);
    constexpr Vec3<float> r2 (0.816371619f, 0.572077572f, -0.079149574f);
    constexpr Vec3<float> r3 (-0.257068098f, 0.698900223f, 0.667423784f);
    
    constexpr float         theta   (60.0f);
    constexpr Vec3<float>   axis1   (1.0f, 0.0f, 0.0f);
    constexpr Vec3<float>   axis2   (0.0f, 1.0f, 0.0f);
    constexpr Vec3<float>   axis3   (0.0f, 0.0f, 1.0f);
    constexpr Vec3<float>   v       (0.4767313f, 0.572077572f, 0.667423784f);

    Transform rx    = Transform::rotateX(theta);
    Transform ry    = Transform::rotateY(theta);
    Transform rz    = Transform::rotateZ(theta);
    Transform ra1   = Transform::rotate(axis1, theta);
    Transform ra2   = Transform::rotate(axis2, theta);
    Transform ra3   = Transform::rotate(axis3, theta);

    Vec3<float> res1 = rx(v);
    Vec3<float> res2 = ry(v);
    Vec3<float> res3 = rz(v);
    Vec3<float> res4 = ra1(v);
    Vec3<float> res5 = ra2(v);
    Vec3<float> res6 = ra3(v);

    EXPECT_EQ(r1, res1);
    EXPECT_EQ(r2, res2);
    EXPECT_EQ(r3, res3);

    EXPECT_EQ(r1, res4);
    EXPECT_EQ(r2, res5);
    EXPECT_EQ(r3, res6);
}

TEST(MovingTransform, LookAt)
{
    constexpr Matrix4x4<float> res (
        -0.0995037183f,     -0.663813591f,  -0.741249323f,      0.0f,
        0.0f,               0.744946361f,   -0.667124391f,      0.0f,
        0.995037198f,       -0.0663813576f, -0.0741249323f,     0.0f,
        0.497518599,        -3.38544941,    -3.78037167,        1.0f
    );

    constexpr Vec3<float> pos   (45.0f, 45.0f, 4.0f);
    constexpr Vec3<float> eye   (-5.0f, 0.0f, -1.0f);
    constexpr Vec3<float> up    (0.0f, 1.0f, 0.0f);

    Transform look_at = Transform::lookAt(pos, eye, up);
    
    EXPECT_EQ(res, look_at.getMatrix());
}

TEST(MovingTransform, PrespectiveProjection)
{
    constexpr Matrix4x4<float> res (
        -0.117089964f,  0.0f,           0.0f,           0.0f,
        0.0f,           -0.156119958f,  0.0f,           0.0f,
        0.0f,           0.0f,           -1.00004995,    -1.0f,
        0.0f,           0.0f,           -0.0100004999, 0.0f
    );

    constexpr float fov       = 60.0f;
    constexpr float aspect    = 4.0f / 3.0f;
    constexpr float z_near    = 0.01f;
    constexpr float z_far     = 200.0f;

    Transform perspective_projection = Transform::perspective(fov, aspect, z_near, z_far);

    EXPECT_EQ(res, perspective_projection.getMatrix());
}
