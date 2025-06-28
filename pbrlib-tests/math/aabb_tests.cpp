#include "../utils.hpp"

#include <pbrlib/math/aabb.hpp>

#include <pbrlib/exceptions.hpp>

#include <array>

TEST(AABBTests, Ctor)
{
    constexpr pbrlib::math::vec3 p_min (0, 0, 0);
    constexpr pbrlib::math::vec3 p_max (1, 1, 1);

    constexpr pbrlib::math::AABB aabb_1;
    constexpr pbrlib::math::AABB aabb_2 (p_min);
    constexpr pbrlib::math::AABB aabb_3 (p_min, p_max);

    pbrlib::testing::thisTrue(aabb_1.empty());

    pbrlib::testing::equality(aabb_2.p_min, aabb_2.p_max);
    pbrlib::testing::thisFalse(aabb_2.empty());

    pbrlib::testing::equality(aabb_3.p_min, p_min);
    pbrlib::testing::equality(aabb_3.p_max, p_max);
    pbrlib::testing::thisFalse(aabb_3.empty());
}

TEST(AABBTests, SubscriptOperator)
{
    constexpr pbrlib::math::vec3 p1 (1, 2, 3);
    constexpr pbrlib::math::vec3 p2 (4, 5, 6);

    constexpr pbrlib::math::AABB aabb (p1, p2);

    pbrlib::testing::equality(aabb[0], aabb.p_min);
    pbrlib::testing::equality(aabb[1], aabb.p_max);
}

TEST(AABBTests, Corner)
{
    constexpr pbrlib::math::vec3 p_min (0, 0, 0);
    constexpr pbrlib::math::vec3 p_max (1, 1, 1);

    constexpr pbrlib::math::AABB aabb (p_min, p_max);

    constexpr std::array points 
    {
        pbrlib::math::vec3(0, 0, 0),
        pbrlib::math::vec3(1, 0, 0),
        pbrlib::math::vec3(0, 1, 0),
        pbrlib::math::vec3(1, 1, 0),
        pbrlib::math::vec3(0, 0, 1),
        pbrlib::math::vec3(1, 0, 1),
        pbrlib::math::vec3(0, 1, 1),
        pbrlib::math::vec3(1, 1, 1),
    };

    for (size_t i = 0; i < 8; ++i)
        pbrlib::testing::equality(aabb.corner(i), points[i]);
}

TEST(AABBTests, Diagonal)
{
    constexpr pbrlib::math::AABB aabb (
        pbrlib::math::vec3(12.21, 43.23, 54.76),
        pbrlib::math::vec3(65.11, 98.73, 83.12)
    );

    constexpr pbrlib::math::vec3 d (52.9, 55.5, 28.36);

    pbrlib::testing::equality(aabb.diagonal(), d);
}

TEST(AABBTests, SurfaceArea)
{
    constexpr pbrlib::math::AABB aabb (
        pbrlib::math::vec3(12.21, 43.23, 54.76),
        pbrlib::math::vec3(65.11, 98.73, 83.12)
    );

    constexpr auto surface_area = 12020.3496f;

    pbrlib::testing::equality(aabb.surfaceArea(), surface_area);
}

TEST(AABBTests, Volume)
{
    constexpr pbrlib::math::AABB aabb (
        pbrlib::math::vec3(12.21, 43.23, 54.76),
        pbrlib::math::vec3(65.11, 98.73, 83.12)
    );

    constexpr auto volume = 83263.5625f;

    pbrlib::testing::equality(aabb.volume(), volume);
}

TEST(AABBTests, SubscriptOperatorEdgeCase)
{
    constexpr pbrlib::math::vec3 p1 (1, 2, 3);
    constexpr pbrlib::math::vec3 p2 (4, 5, 6);

    constexpr pbrlib::math::AABB aabb (p1, p2);
    
    EXPECT_THROW({
        [[maybe_unused]] const auto p = aabb[2];
    }, pbrlib::exception::InvalidArgument);
}

TEST(AABBTests, CornerEdgeCase)
{
    constexpr pbrlib::math::vec3 p1 (1, 2, 3);
    constexpr pbrlib::math::vec3 p2 (4, 5, 6);

    constexpr pbrlib::math::AABB aabb (p1, p2);

    EXPECT_THROW({
        [[maybe_unused]] const auto p = aabb.corner(8);
    }, pbrlib::exception::InvalidArgument);
}