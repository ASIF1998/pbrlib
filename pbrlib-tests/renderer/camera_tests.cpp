#include "../utils/assertions.hpp"

#include <pbrlib/rendering/camera.hpp>

TEST(CameraTests, Ctor)
{
    pbrlib::Camera camera;

    pbrlib::testing::equality(camera.pos(), pbrlib::math::vec3(0, 0, -1));
    pbrlib::testing::equality(camera.eye(), pbrlib::math::vec3(0, 0, 0));
    pbrlib::testing::equality(camera.up(), pbrlib::math::vec3(0, 1, 0));

    EXPECT_NEAR(camera.range().near, 0.0f, 0.0001f);
    EXPECT_TRUE(std::isinf(camera.range().far));
    
    EXPECT_NEAR(camera.fovY(), 0.0f, 0.0001f);
    EXPECT_NEAR(camera.aspect(), 0.0f, 0.0001f);
}

TEST(CameraTests, SettersAndGetters)
{
    constexpr pbrlib::math::vec3 pos    (342.343, 540.123, 932.54);
    constexpr pbrlib::math::vec3 eye    (432.343, 111.123, 532.54);
    constexpr pbrlib::math::vec3 up     (1, 0, 0);

    constexpr float near    = 10.0f;
    constexpr float far     = 1234.43f;

    constexpr float fov_y   = 54.6f;
    constexpr float aspect  = 800.0f / 600.0f;

    pbrlib::Camera camera;
    
    camera.pos(pos);
    camera.eye(eye);
    camera.up(up);
    camera.range(near, far);
    camera.fovY(fov_y);
    camera.aspect(aspect);

    pbrlib::testing::equality(camera.pos(), pos);
    pbrlib::testing::equality(camera.eye(), eye);
    pbrlib::testing::equality(camera.up(), up);

    EXPECT_NEAR(camera.range().near, near, 0.0001f);
    EXPECT_NEAR(camera.range().far, far, 0.0001f);
    
    EXPECT_NEAR(camera.fovY(), fov_y, 0.0001f);
    EXPECT_NEAR(camera.aspect(), aspect, 0.0001f);
}

TEST(CameraTests, ViewMatrix)
{
    pbrlib::Camera camera;
    camera.pos(pbrlib::math::vec3(123, 43, 91));
    camera.eye(pbrlib::math::vec3(43, 65, -45));
    camera.up(pbrlib::math::vec3(0.43, 0.65, 0.12));

    constexpr pbrlib::math::mat4 view_matrix {
        -0.7572f, 0.4176f, -0.5021f, 0.0000f,
        0.4065f, 0.9031f, 0.1380f, 0.0000f,
        0.5111f, -0.0995f, -0.8536f, 0.0000f,
        29.1380f, -81.1443f, -25.7985f, 1.0000f
    };

    pbrlib::testing::equality(camera.view(), view_matrix);
}

TEST(CameraTests, ProjectionMatrix)
{
    pbrlib::Camera camera;
    camera.aspect(800.0f / 600.0f);
    camera.fovY(46.43f);
    camera.range(0.0032f, 132.32f);

    constexpr pbrlib::math::mat4 projection_matrix = {
        0.2711f, 0.0000f, 0.0000f, 0.0000f,
        0.0000f, 0.3615f, 0.0000f, 0.0000f,
        0.0000f, 0.0000f, -1.0000f, -1.0000f,
        0.0000f, 0.0000f, -0.0032f, 0.0000f
    };

    pbrlib::testing::equality(camera.projection(), projection_matrix);
}