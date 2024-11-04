//
//  TransformTests.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 17/04/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include "../utils.hpp"

#include <pbrlib/Moving/Transform.hpp>
#include <pbrlib/math/vec3.hpp>

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

    Matrix4x4<float> mt1 = t1.getMatrix();
    Matrix4x4<float> mt2 = t2.getMatrix();

    pbrlib::testing::utils::equality(tm1, mt1);
    pbrlib::testing::utils::equality(tm2, mt2);
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

    pbrlib::testing::utils::thisTrue(t1 == t2);

    t2.setMatrix(tm);

    pbrlib::testing::utils::thisTrue(t1 != t2);
}

TEST(MovingTransform, GetMatrixAndInverseMatrix)
{
    Matrix4x4<float> tm (
        1.0f, 2.0f, 3.0f, 4.0f,
        4.0f, 3.0f, 2.0f, 1.0f,
        5.0f, 6.0f, 7.0f, 8.0f,
        8.0f, 7.0f, 6.0f, 5.0f
    );

    Transform t (tm);

    Matrix4x4<float> mt = t.getMatrix();

    pbrlib::testing::utils::equality(tm, mt);

    tm = inverse(tm);
    mt = t.getInverseMatrix();

    pbrlib::testing::utils::equality(tm, mt);
}

TEST(MovingTransform, Translate)
{
    constexpr Vec3<float> t (0.5f, 0.5f, 0.5f);
    
    Transform transform = Transform::translate(t);

    constexpr Matrix4x4<float> r (
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f
    );

    pbrlib::testing::utils::equality(r, transform.getMatrix());
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

    pbrlib::testing::utils::equality(r, transform.getMatrix());
}

TEST(MovingTransform, Rotates)
{
    constexpr Vec3<float> r1 (0.4767313000f, -0.291967213f, 0.8291456100f);
    constexpr Vec3<float> r2 (0.8163716190f, 0.5720775720f, -0.079149574f);
    constexpr Vec3<float> r3 (-0.257068098f, 0.6989002230f, 0.6674237840f);
    
    constexpr float         theta   (60.000000f);
    constexpr Vec3<float>   axis1   (1.0000000f, 0.000000000f, 0.000000000f);
    constexpr Vec3<float>   axis2   (0.0000000f, 1.000000000f, 0.000000000f);
    constexpr Vec3<float>   axis3   (0.0000000f, 0.000000000f, 1.000000000f);
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

    pbrlib::testing::utils::equality(r1, res1);
    pbrlib::testing::utils::equality(r2, res2);
    pbrlib::testing::utils::equality(r3, res3);
    pbrlib::testing::utils::equality(r1, res4);
    pbrlib::testing::utils::equality(r2, res5);
    pbrlib::testing::utils::equality(r3, res6);
}

TEST(MovingTransform, LookAt)
{
    constexpr Matrix4x4<float> res (
        -0.0995037183f, -0.6638135910f, -0.7412493230f, 0.0f,
        0.00000000000f, 0.74494636100f, -0.6671243910f, 0.0f,
        0.99503719800f, -0.0663813576f, -0.0741249323f, 0.0f,
        0.49751859900f, -3.3854494100f, -3.7803716700f, 1.0f
    );

    constexpr Vec3<float> pos   (45.0f, 45.0f, 4.00f);
    constexpr Vec3<float> eye   (-5.0f, 0.00f, -1.0f);
    constexpr Vec3<float> up    (0.00f, 1.00f, 0.00f);

    Transform look_at = Transform::lookAt(pos, eye, up);

    pbrlib::testing::utils::equality(res, look_at.getMatrix());
}

TEST(MovingTransform, PrespectiveProjection)
{
    constexpr Matrix4x4<float> res (
        -0.117089964f, 0.00000000f, 0.00000000f, 0.00000000f,
        0.00000000f, -0.156119958f, 0.00000000f, 0.00000000f,
        0.00000000f, 0.00000000f, -1.00004995f, -1.00000000f,
        0.00000000f, 0.00000000f, -0.0100004999f, 0.00000000f
    );

    constexpr float fov       = 60.0f;
    constexpr float aspect    = 4.0f / 3.0f;
    constexpr float z_near    = 0.010f;
    constexpr float z_far     = 200.0f;

    Transform perspective_projection = Transform::perspective(fov, aspect, z_near, z_far);

    pbrlib::testing::utils::equality(res, perspective_projection.getMatrix());
}
