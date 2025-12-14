#pragma once

#include <gtest/gtest.h>

#include <pbrlib/math/vec2.hpp>
#include <pbrlib/math/vec3.hpp>
#include <pbrlib/math/vec4.hpp>

#include <pbrlib/math/matrix2x2.hpp>
#include <pbrlib/math/matrix3x3.hpp>
#include <pbrlib/math/matrix4x4.hpp>

#include <pbrlib/math/quat.hpp>

#include <backend/logger/logger.hpp>

#include <vulkan/vulkan.h>

#include <string_view>
#include <concepts>

using namespace testing;

namespace pbrlib::testing
{
    template<typename Type>
    inline void notEquality(const Type& v1, const Type& v2)
    {
        EXPECT_NE(v1, v2);
    }

    template<typename Type>
    inline void notEquality(const Type& v1, const Type& v2, const std::string_view err_msg)
    {
        EXPECT_NE(v1, v2) << err_msg << std::endl;
    }

    template<typename T, typename U>
    inline void equality(const T& v1, const U& v2)
    {
        if constexpr (std::is_floating_point<T>::value || std::is_floating_point<U>::value)
            EXPECT_NEAR(v1, v2, 0.0001f);
        else 
            EXPECT_EQ(v1, v2);
    }

    template<typename Type>
    inline void equality(const Type& v1, const Type& v2, const std::string_view err_msg)
    {
        EXPECT_EQ(v1, v2) << err_msg << std::endl;
    }

    template<typename Type>
    inline void greaterEquality(const Type& v1, const Type& v2)
    {
        EXPECT_GE(v1, v2);
    }

    template<typename Type>
    inline void greaterEquality(const Type& v1, const Type& v2, const std::string_view err_msg)
    {
        EXPECT_GE(v1, v2) << err_msg << std::endl;
    }

    template<typename Type>
    inline void equality(const pbrlib::math::Matrix2x2<Type>& m, const Type a[4])
    {
        for (size_t i = 0; i < 2; i++) 
        {
            for (size_t j = 0; j < 2; j++)
                EXPECT_EQ(m.at(i, j), a[i * 2 + j]);
        }
    }

    template<typename Type>
    inline void equality(const pbrlib::math::Matrix2x2<Type>& m, const Type a[4], const std::string_view err_msg)
    {
        for (size_t i = 0; i < 2; i++) 
        {
            for (size_t j = 0; j < 2; j++)
                EXPECT_EQ(m.at(i, j), a[i * 2 + j]) << err_msg << std::endl;
        }
    }

    template<typename Type>
    inline void equality(const pbrlib::math::Matrix3x3<Type>& m, const Type a[9])
    {
        for (size_t i = 0; i < 3; i++) 
        {
            for (size_t j = 0; j < 3; j++)
                EXPECT_EQ(m.at(i, j), a[i * 3 + j]);
        }
    }

    template<typename Type>
    inline void equality(const pbrlib::math::Matrix3x3<Type>& m, const Type a[9], const std::string_view err_msg)
    {
        for (size_t i = 0; i < 3; i++) 
        {
            for (size_t j = 0; j < 3; j++)
                EXPECT_EQ(m.at(i, j), a[i * 3 + j]) << err_msg << std::endl;
        }
    }

    template<typename Type>
    inline void equality(const pbrlib::math::Matrix4x4<Type>& m, const Type a[16])
    {
        for (size_t i = 0; i < 4; i++) 
        {
            for (size_t j = 0; j < 4; j++)
                EXPECT_EQ(m.at(i, j), a[i * 4 + j]);
        }
    }

    template<typename Type>
    inline void equality(const pbrlib::math::Matrix4x4<Type>& m, const Type a[16], const std::string_view err_msg)
    {
        for (size_t i = 0; i < 4; i++) 
        {
            for (size_t j = 0; j < 4; j++)
                EXPECT_EQ(m.at(i, j), a[i * 4 + j]) << err_msg << std::endl;
        }
    }

    inline void equality(float v1, float v2, const std::string_view err_msg)
    {
        EXPECT_NEAR(v1, v2, 0.0001f) << err_msg << std::endl;
    }

    inline void equality(const pbrlib::math::mat2& m, const float a[4], const std::string_view err_msg)
    {
        for (size_t i = 0; i < 2; i++) 
        {
            for (size_t j = 0; j < 2; j++)
                EXPECT_NEAR(m.at(i, j), a[i * 2 + j], 0.0001F) << err_msg << std::endl;
        }
    }

    inline void equality(const pbrlib::math::mat3& m, const float a[9])
    {
        for (size_t i = 0; i < 3; i++) 
        {
            for (size_t j = 0; j < 3; j++)
                EXPECT_NEAR(m.at(i, j), a[i * 3 + j], 0.0001f);
        }
    }

    inline void equality(const pbrlib::math::mat3& m, const float a[9], const std::string_view err_msg)
    {
        for (size_t i = 0; i < 3; i++) 
        {
            for (size_t j = 0; j < 3; j++)
                EXPECT_NEAR(m.at(i, j), a[i * 3 + j], 0.0001f) << err_msg << std::endl;
        }
    }

    inline void equality(const pbrlib::math::mat4& m, const float a[16])
    {
        for (size_t i = 0; i < 4; i++) 
        {
            for (size_t j = 0; j < 4; j++)
                EXPECT_NEAR(m.at(i, j), a[i * 4 + j], 0.0001f);
        }
    }

    inline void equality(const pbrlib::math::mat4& m, const float a[16], const std::string_view err_msg)
    {
        for (size_t i = 0; i < 4; i++) 
        {
            for (size_t j = 0; j < 4; j++)
                EXPECT_NEAR(m.at(i, j), a[i * 4 + j], 0.0001f) << err_msg << std::endl;
        }
    }

    inline void thisTrue(bool t)
    {
        EXPECT_TRUE(t);
    }

    inline void thisTrue(bool t, const std::string_view err_msg)
    {
        EXPECT_TRUE(t) << err_msg << std::endl;
    }

    inline void thisFalse(bool t)
    {
        EXPECT_FALSE(t);
    }

    inline void thisFalse(bool t, const std::string_view err_msg)
    {
        EXPECT_FALSE(t) << err_msg << std::endl;
    }

    template<typename T>
    inline constexpr T roundForInteger(double v)
    {
        if constexpr (std::is_floating_point<T>::value)
            return static_cast<T>(v);
        
        return static_cast<T>(v + 0.5);
    } 
}

namespace pbrlib::testing::vk
{
    constexpr inline bool isSupport()
    {
#ifdef PBRLIB_RUN_FROM_CI
        /// swiftshader apparently doesn't support float16
        return false;
#else 
        return true;
#endif
    }

    inline void isSuccess(VkResult result)
    {
        ASSERT_EQ(result, VK_SUCCESS);
    }
}