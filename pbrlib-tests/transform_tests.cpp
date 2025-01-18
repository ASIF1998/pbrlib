#include "utils.hpp"

#include <pbrlib/transform.hpp>
#include <pbrlib/math/vec3.hpp>

TEST(MovingTransform, Constructor)
{
    constexpr pbrlib::math::mat4 tm1 (
        1.0f, 0.0f, 0.0f, 0.0f, 
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );

    constexpr pbrlib::math::mat4 tm2 (
        1.0f, 2.0f, 3.0f, 4.0f,
        4.0f, 3.0f, 2.0f, 1.0f,
        5.0f, 6.0f, 7.0f, 8.0f,
        8.0f, 7.0f, 6.0f, 5.0f
    );

    pbrlib::Transform t1;
    pbrlib::Transform t2 (tm2);

    pbrlib::math::mat4 mt1 = t1.getMatrix();
    pbrlib::math::mat4 mt2 = t2.getMatrix();

    pbrlib::testing::equality(tm1, mt1);
    pbrlib::testing::equality(tm2, mt2);
}

TEST(MovingTransform, EqualAndNotEqual)
{
    constexpr pbrlib::math::mat4 tm (
        1.0f, 2.0f, 3.0f, 4.0f,
        4.0f, 3.0f, 2.0f, 1.0f,
        5.0f, 6.0f, 7.0f, 8.0f,
        8.0f, 7.0f, 6.0f, 5.0f
    );

    pbrlib::Transform t1;
    pbrlib::Transform t2;

    pbrlib::testing::thisTrue(t1 == t2);

    t2.setMatrix(tm);

    pbrlib::testing::thisTrue(t1 != t2);
}

TEST(MovingTransform, GetMatrixAndInverseMatrix)
{
    pbrlib::math::mat4 tm (
        1.0f, 2.0f, 3.0f, 4.0f,
        4.0f, 3.0f, 2.0f, 1.0f,
        5.0f, 6.0f, 7.0f, 8.0f,
        8.0f, 7.0f, 6.0f, 5.0f
    );

    pbrlib::Transform t (tm);

    pbrlib::math::mat4 mt = t.getMatrix();

    pbrlib::testing::equality(tm, mt);

    tm = inverse(tm);
    mt = t.getInverseMatrix();

    pbrlib::testing::equality(tm, mt);
}

TEST(MovingTransform, Translate)
{
    constexpr pbrlib::math::vec3 t (0.5f, 0.5f, 0.5f);
    
    pbrlib::Transform transform = pbrlib::Transform::translate(t);

    constexpr pbrlib::math::mat4 r (
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f
    );

    pbrlib::testing::equality(r, transform.getMatrix());
}

TEST(MovingTransform, Scale)
{
    constexpr pbrlib::math::vec3 t (0.5f, 0.5f, 0.5f);
    
    auto transform = pbrlib::Transform::scale(t);

    constexpr pbrlib::math::mat4 r (
        0.5f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.5f, 0.0f, 
        0.0f, 0.0f, 0.0f, 1.0f
    );

    pbrlib::testing::equality(r, transform.getMatrix());
}

TEST(MovingTransform, Rotates)
{
    constexpr pbrlib::math::vec3 r1 (0.4767313000f, -0.291967213f, 0.8291456100f);
    constexpr pbrlib::math::vec3 r2 (0.8163716190f, 0.5720775720f, -0.079149574f);
    constexpr pbrlib::math::vec3 r3 (-0.257068098f, 0.6989002230f, 0.6674237840f);
    
    constexpr float theta = 60.0f;

    constexpr pbrlib::math::vec3    axis1   (1.0000000f, 0.000000000f, 0.000000000f);
    constexpr pbrlib::math::vec3    axis2   (0.0000000f, 1.000000000f, 0.000000000f);
    constexpr pbrlib::math::vec3    axis3   (0.0000000f, 0.000000000f, 1.000000000f);
    constexpr pbrlib::math::vec3    v       (0.4767313f, 0.572077572f, 0.667423784f);

    auto rx     = pbrlib::Transform::rotateX(theta);
    auto ry     = pbrlib::Transform::rotateY(theta);
    auto rz     = pbrlib::Transform::rotateZ(theta);
    auto ra1    = pbrlib::Transform::rotate(axis1, theta);
    auto ra2    = pbrlib::Transform::rotate(axis2, theta);
    auto ra3    = pbrlib::Transform::rotate(axis3, theta);

    auto res1 = rx(v);
    auto res2 = ry(v);
    auto res3 = rz(v);
    auto res4 = ra1(v);
    auto res5 = ra2(v);
    auto res6 = ra3(v);

    pbrlib::testing::equality(r1, res1);
    pbrlib::testing::equality(r2, res2);
    pbrlib::testing::equality(r3, res3);
    pbrlib::testing::equality(r1, res4);
    pbrlib::testing::equality(r2, res5);
    pbrlib::testing::equality(r3, res6);
}

TEST(MovingTransform, LookAt)
{
    constexpr pbrlib::math::mat4 res (
        -0.0995037183f, -0.6638135910f, -0.7412493230f, 0.0f,
        0.00000000000f, 0.74494636100f, -0.6671243910f, 0.0f,
        0.99503719800f, -0.0663813576f, -0.0741249323f, 0.0f,
        0.49751859900f, -3.3854494100f, -3.7803716700f, 1.0f
    );

    constexpr pbrlib::math::vec3 pos    (45.0f, 45.0f, 4.00f);
    constexpr pbrlib::math::vec3 eye    (-5.0f, 0.00f, -1.0f);
    constexpr pbrlib::math::vec3 up     (0.00f, 1.00f, 0.00f);

    auto look_at = pbrlib::Transform::lookAt(pos, eye, up);

    pbrlib::testing::equality(res, look_at.getMatrix());
}

TEST(MovingTransform, PrespectiveProjection)
{
    constexpr pbrlib::math::mat4 res (
        -0.117089964f, 0.00000000f, 0.00000000f, 0.00000000f,
        0.00000000f, -0.156119958f, 0.00000000f, 0.00000000f,
        0.00000000f, 0.00000000f, -1.00004995f, -1.00000000f,
        0.00000000f, 0.00000000f, -0.0100004999f, 0.00000000f
    );

    constexpr float fov       = 60.0f;
    constexpr float aspect    = 4.0f / 3.0f;
    constexpr float z_near    = 0.010f;
    constexpr float z_far     = 200.0f;

    auto perspective_projection = pbrlib::Transform::perspective(fov, aspect, z_near, z_far);

    pbrlib::testing::equality(res, perspective_projection.getMatrix());
}
