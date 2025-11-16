#include "utils.hpp"

#include <pbrlib/transforms.hpp>
#include <pbrlib/math/vec3.hpp>

TEST(MovingTransform, Translate)
{
    constexpr pbrlib::math::vec3 t (0.5f, 0.5f, 0.5f);
    
    const auto transform = pbrlib::transforms::translate(t);

    constexpr pbrlib::math::mat4 res (
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f
    );

    pbrlib::testing::equality(transform, res);
}

TEST(MovingTransform, Scale)
{
    constexpr pbrlib::math::vec3 t (0.5f, 0.5f, 0.5f);
    
    auto transform = pbrlib::transforms::scale(t);

    constexpr pbrlib::math::mat4 res (
        0.5f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.5f, 0.0f, 
        0.0f, 0.0f, 0.0f, 1.0f
    );

    pbrlib::testing::equality(transform, res);
}

TEST(MovingTransform, Rotates)
{
    constexpr pbrlib::math::vec3 r1 (0.476731300f, 0.864044726f, -0.161721885f);
    constexpr pbrlib::math::vec3 r2 (-0.339640319f, 0.572077572f, 0.746573329f);
    constexpr pbrlib::math::vec3 r3 (0.733799398f, -0.126822680f, 0.667423785f);
    
    constexpr float theta = 60.0f;

    constexpr pbrlib::math::vec3    axis1   (1.0000000f, 0.000000000f, 0.000000000f);
    constexpr pbrlib::math::vec3    axis2   (0.0000000f, 1.000000000f, 0.000000000f);
    constexpr pbrlib::math::vec3    axis3   (0.0000000f, 0.000000000f, 1.000000000f);
    constexpr pbrlib::math::vec3    v       (0.4767313f, 0.572077572f, 0.667423784f);

    auto res1 = pbrlib::transforms::rotateX(theta) * v;
    auto res2 = pbrlib::transforms::rotateY(theta) * v;
    auto res3 = pbrlib::transforms::rotateZ(theta) * v;
    auto res4 = pbrlib::transforms::rotate(axis1, theta) * v;
    auto res5 = pbrlib::transforms::rotate(axis2, theta) * v;
    auto res6 = pbrlib::transforms::rotate(axis3, theta) * v;

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

    constexpr pbrlib::math::vec3 eye    (45.0f, 45.0f, 4.00f);
    constexpr pbrlib::math::vec3 pos    (-5.0f, 0.00f, -1.0f);
    constexpr pbrlib::math::vec3 up     (0.00f, 1.00f, 0.00f);

    auto look_at = pbrlib::transforms::lookAt(pos, eye, up);

    pbrlib::testing::equality(look_at, res);
}
