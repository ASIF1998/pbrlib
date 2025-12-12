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

    template<typename T>
    inline void equality(const pbrlib::math::Vec2<T>& v1, const pbrlib::math::Vec2<T>& v2)
    {
        if constexpr (std::is_floating_point<T>::value)
        {
            EXPECT_NEAR(v1.x, v2.x, 0.0001f);
            EXPECT_NEAR(v1.y, v2.y, 0.0001f);
        }
        else
        {
            EXPECT_EQ(v1.x, v2.x);
            EXPECT_EQ(v1.y, v2.y);
        }
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

    inline void equality(const pbrlib::math::vec2& v1, const pbrlib::math::vec2& v2)
    {
        EXPECT_NEAR(v1.x, v2.x, 0.0001f);
        EXPECT_NEAR(v1.y, v2.y, 0.0001f);
    }

    inline void equality(const pbrlib::math::vec2& v1, const pbrlib::math::vec2& v2, const std::string_view err_msg)
    {
        EXPECT_NEAR(v1.x, v2.x, 0.0001f) << err_msg << std::endl;
        EXPECT_NEAR(v1.y, v2.y, 0.0001f) << err_msg << std::endl;
    }

    inline void equality(const pbrlib::math::vec3& v1, const pbrlib::math::vec3& v2)
    {
        EXPECT_NEAR(v1.x, v2.x, 0.0001f);
        EXPECT_NEAR(v1.y, v2.y, 0.0001f);
        EXPECT_NEAR(v1.z, v2.z, 0.0001f);
    }

    inline void equality(const pbrlib::math::vec3& v1, const pbrlib::math::vec3& v2, const std::string_view err_msg)
    {
        EXPECT_NEAR(v1.x, v2.x, 0.0001f) << err_msg << std::endl;
        EXPECT_NEAR(v1.y, v2.y, 0.0001f) << err_msg << std::endl;
        EXPECT_NEAR(v1.z, v2.z, 0.0001f) << err_msg << std::endl;
    }

    inline void equality(const pbrlib::math::vec4& v1, const pbrlib::math::vec4& v2)
    {
        EXPECT_NEAR(v1.x, v2.x, 0.0001f);
        EXPECT_NEAR(v1.y, v2.y, 0.0001f);
        EXPECT_NEAR(v1.z, v2.z, 0.0001f);
        EXPECT_NEAR(v1.w, v2.w, 0.0001f);
    }

    inline void equality(const pbrlib::math::vec4& v1, const pbrlib::math::vec4& v2, const std::string_view err_msg)
    {
        EXPECT_NEAR(v1.x, v2.x, 0.0001f) << err_msg << std::endl;
        EXPECT_NEAR(v1.y, v2.y, 0.0001f) << err_msg << std::endl;
        EXPECT_NEAR(v1.z, v2.z, 0.0001f) << err_msg << std::endl;
        EXPECT_NEAR(v1.w, v2.w, 0.0001f) << err_msg << std::endl;
    }

    inline void equality(const pbrlib::math::mat2& m1, const pbrlib::math::mat2& m2)
    {
        for (size_t i = 0; i < 2; ++i) 
        {
            for (size_t j = 0; j < 2; ++j)
                EXPECT_NEAR(m1[i][j], m2[i][j], 0.0001f);
        }
    }

    inline void equality(const pbrlib::math::mat2& m1, const pbrlib::math::mat2& m2, const std::string_view err_msg)
    {
        for (size_t i = 0; i < 2; ++i) 
        {
            for (size_t j = 0; j < 2; ++j)
                EXPECT_NEAR(m1[i][j], m2[i][j], 0.0001f) << err_msg << std::endl;
        }
    }

    inline void equality(const pbrlib::math::mat2& m, const float a[4])
    {
        for (size_t i = 0; i < 2; i++) 
        {
            for (size_t j = 0; j < 2; j++)
                EXPECT_NEAR(m.at(i, j), a[i * 2 + j], 0.0001f);
        }
    }

    inline void equality(const pbrlib::math::mat2& m, const float a[4], const std::string_view err_msg)
    {
        for (size_t i = 0; i < 2; i++) 
        {
            for (size_t j = 0; j < 2; j++)
                EXPECT_NEAR(m.at(i, j), a[i * 2 + j], 0.0001F) << err_msg << std::endl;
        }
    }

#if (defined(__SSE__) || defined(__AVX2__))
    inline void equality(const pbrlib::math::mat2& m, const __m128& a)
    {
        float v[4];
        _mm_store_ps(v, a);

        for (size_t i = 0; i < 2; i++) 
        {
            for (size_t j = 0; j < 2; j++)
                EXPECT_NEAR(m.at(i, j), v[i * 2 + j], 0.0001f);
        }

    }

    inline void equality(const pbrlib::math::mat2& m, const __m128& a, const std::string_view err_msg)
    {
        float v[4];
        _mm_store_ps(v, a);

        for (size_t i = 0; i < 2; i++) 
        {
            for (size_t j = 0; j < 2; j++)
                EXPECT_NEAR(m.at(i, j), v[i * 2 + j], 0.0001f) << err_msg << std::endl;
        }
    }
#endif

    inline void equality(const pbrlib::math::mat3& m1, const pbrlib::math::mat3& m2)
    {
        for (size_t i = 0; i < 3; ++i) 
        {
            for (size_t j = 0; j < 3; ++j)
                EXPECT_NEAR(m1[i][j], m2[i][j], 0.0001f);
        }
    }

    inline void equality(const pbrlib::math::mat3& m1, const pbrlib::math::mat3& m2, const std::string_view err_msg)
    {
        for (size_t i = 0; i < 3; ++i) 
        {
            for (size_t j = 0; j < 3; ++j)
                EXPECT_NEAR(m1[i][j], m2[i][j], 0.0001f) << err_msg << std::endl;
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

    inline void equality(const pbrlib::math::mat4& m1, const pbrlib::math::mat4& m2)
    {
        for (size_t i = 0; i < 4; ++i) {
            for (size_t j = 0; j < 4; ++j) {
                EXPECT_NEAR(m1[i][j], m2[i][j], 0.0001f);
            }
        }
    }

    inline void equality(const pbrlib::math::mat4& m1, const pbrlib::math::mat4& m2, const std::string_view err_msg)
    {
        for (size_t i = 0; i < 4; ++i) 
        {
            for (size_t j = 0; j < 4; ++j)
                EXPECT_NEAR(m1[i][j], m2[i][j], 0.0001f) << err_msg << std::endl;
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

    inline void equality(const pbrlib::math::quat& q1, const pbrlib::math::quat& q2)
    {
        EXPECT_NEAR(q1.v.x, q2.v.x, 0.0001f);
        EXPECT_NEAR(q1.v.y, q2.v.y, 0.0001f);
        EXPECT_NEAR(q1.v.z, q2.v.z, 0.0001f);
        EXPECT_NEAR(q1.w, q2.w, 0.0001f);
    }

    inline void equality(const pbrlib::math::quat& q1, const pbrlib::math::quat& q2, const std::string_view err_msg)
    {
        EXPECT_NEAR(q1.v.x, q2.v.x, 0.0001f) << err_msg << std::endl;
        EXPECT_NEAR(q1.v.y, q2.v.y, 0.0001f) << err_msg << std::endl;
        EXPECT_NEAR(q1.v.z, q2.v.z, 0.0001f) << err_msg << std::endl;
        EXPECT_NEAR(q1.w, q2.w, 0.0001f) << err_msg << std::endl;
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