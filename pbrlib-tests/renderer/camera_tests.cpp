#include "../utils.hpp"

#include <pbrlib/rendering/camera.hpp>

TEST(CameraTests, Ctor)
{
    pbrlib::Camera camera;

    pbrlib::testing::utils::equality(camera.pos(), pbrlib::math::vec3(0, 0, -1));
    pbrlib::testing::utils::equality(camera.eye(), pbrlib::math::vec3(0, 0, 0));
    pbrlib::testing::utils::equality(camera.up(), pbrlib::math::vec3(0, 1, 0));

    EXPECT_NEAR(camera.range().near, 0.0f, 0.0001f);
    EXPECT_NEAR(camera.range().far, std::numeric_limits<float>::infinity(), 0.0001f);
    
    EXPECT_NEAR(camera.fovY(), 0.0f, 0.0001f);
    EXPECT_NEAR(camera.aspect(), 0.0f, 0.0001f);

    /// @todo add test for projection matrix
    /// @todo add test for view matrix}
}

TEST(CameraTests, SettersAndGetters)
{
    /// @todo
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

    pbrlib::testing::utils::equality(camera.pos(), pos);
    pbrlib::testing::utils::equality(camera.eye(), eye);
    pbrlib::testing::utils::equality(camera.up(), up);

    EXPECT_NEAR(camera.range().near, near, 0.0001f);
    EXPECT_NEAR(camera.range().far, far, 0.0001f);
    
    EXPECT_NEAR(camera.fovY(), fov_y, 0.0001f);
    EXPECT_NEAR(camera.aspect(), aspect, 0.0001f);
}

TEST(CameraTests, ViewMatrix)
{
    /// @todo
}

TEST(CameraTests, ProjectionMatrix)
{
    /// @todo
}