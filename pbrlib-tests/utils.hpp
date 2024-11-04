//
//  utils.hpp
//  PBRLibTests
//
//  Created by Асиф Мамедов on 04/12/2021.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef PBRLibTests_utls_hpp
#define PBRLibTests_utls_hpp

#include <gtest/gtest.h>

#include <string_view>

#include <pbrlib/math/vec2.hpp>
#include <pbrlib/math/vec3.hpp>
#include <pbrlib/math/vec4.hpp>

#include <pbrlib/math/matrix2x2.hpp>
#include <pbrlib/math/matrix3x3.hpp>
#include <pbrlib/math/matrix4x4.hpp>

#include <pbrlib/math/quat.hpp>

using namespace pbrlib::math;
using namespace testing;
using namespace std;

namespace pbrlib::testing::utils
{
    template<typename Type>
    inline void notEquality(const Type& v1, const Type& v2)
    {
        EXPECT_NE(v1, v2);
    }

    template<typename Type>
    inline void notEquality(const Type& v1, const Type& v2, const string_view err_msg)
    {
        EXPECT_NE(v1, v2) << err_msg << endl;
    }

    template<typename Type>
    inline void equality(const Type& v1, const Type& v2)
    {
        EXPECT_EQ(v1, v2);
    }

    template<typename Type>
    inline void equality(const Type& v1, const Type& v2, const string_view err_msg)
    {
        EXPECT_EQ(v1, v2) << err_msg << endl;
    }

    template<typename Type>
    inline void greaterEquality(const Type& v1, const Type& v2)
    {
        EXPECT_GE(v1, v2);
    }

    template<typename Type>
    inline void greaterEquality(const Type& v1, const Type& v2, const string_view err_msg)
    {
        EXPECT_GE(v1, v2) << err_msg << endl;
    }

    template<typename Type>
    inline void equality(const Matrix2x2<Type>& m, const Type a[4])
    {
        for (size_t i = 0; i < 2; i++) {
            for (size_t j = 0; j < 2; j++) {
                EXPECT_EQ(m.at(i, j), a[i * 2 + j]);
            }
        }
    }

    template<typename Type>
    inline void equality(const Matrix2x2<Type>& m, const Type a[4], const string_view err_msg)
    {
        for (size_t i = 0; i < 2; i++) {
            for (size_t j = 0; j < 2; j++) {
                EXPECT_EQ(m.at(i, j), a[i * 2 + j]) << err_msg << endl;
            }
        }
    }

    template<typename Type>
    inline void equality(const Matrix3x3<Type>& m, const Type a[9])
    {
        for (size_t i = 0; i < 3; i++) {
            for (size_t j = 0; j < 3; j++) {
                EXPECT_EQ(m.at(i, j), a[i * 3 + j]);
            }
        }
    }

    template<typename Type>
    inline void equality(const Matrix3x3<Type>& m, const Type a[9], const string_view err_msg)
    {
        for (size_t i = 0; i < 3; i++) {
            for (size_t j = 0; j < 3; j++) {
                EXPECT_EQ(m.at(i, j), a[i * 3 + j]) << err_msg << endl;
            }
        }
    }

    template<typename Type>
    inline void equality(const Matrix4x4<Type>& m, const Type a[16])
    {
        for (size_t i = 0; i < 4; i++) {
            for (size_t j = 0; j < 4; j++) {
                EXPECT_EQ(m.at(i, j), a[i * 4 + j]);
            }
        }
    }

    template<typename Type>
    inline void equality(const Matrix4x4<Type>& m, const Type a[16], const string_view err_msg)
    {
        for (size_t i = 0; i < 4; i++) {
            for (size_t j = 0; j < 4; j++) {
                EXPECT_EQ(m.at(i, j), a[i * 4 + j]) << err_msg << endl;
            }
        }
    }

    inline void equality(float v1, float v2)
    {
        EXPECT_NEAR(v1, v2, 0.0001f);
    }

    inline void equality(float v1, float v2, const string_view err_msg)
    {
        EXPECT_NEAR(v1, v2, 0.0001f) << err_msg << endl;
    }

    inline void equality(const Vec2<float>& v1, const Vec2<float>& v2)
    {
        EXPECT_NEAR(v1.x, v2.x, 0.0001f);
        EXPECT_NEAR(v1.y, v2.y, 0.0001f);
    }

    inline void equality(const Vec2<float>& v1, const Vec2<float>& v2, const string_view err_msg)
    {
        EXPECT_NEAR(v1.x, v2.x, 0.0001f) << err_msg << endl;
        EXPECT_NEAR(v1.y, v2.y, 0.0001f) << err_msg << endl;
    }

    inline void equality(const Vec3<float>& v1, const Vec3<float>& v2)
    {
        EXPECT_NEAR(v1.x, v2.x, 0.0001f);
        EXPECT_NEAR(v1.y, v2.y, 0.0001f);
        EXPECT_NEAR(v1.z, v2.z, 0.0001f);
    }

    inline void equality(const Vec3<float>& v1, const Vec3<float>& v2, const string_view err_msg)
    {
        EXPECT_NEAR(v1.x, v2.x, 0.0001f) << err_msg << endl;
        EXPECT_NEAR(v1.y, v2.y, 0.0001f) << err_msg << endl;
        EXPECT_NEAR(v1.z, v2.z, 0.0001f) << err_msg << endl;
    }

    inline void equality(const Vec4<float>& v1, const Vec4<float>& v2)
    {
        EXPECT_NEAR(v1.x, v2.x, 0.0001f);
        EXPECT_NEAR(v1.y, v2.y, 0.0001f);
        EXPECT_NEAR(v1.z, v2.z, 0.0001f);
        EXPECT_NEAR(v1.w, v2.w, 0.0001f);
    }

    inline void equality(const Vec4<float>& v1, const Vec4<float>& v2, const string_view err_msg)
    {
        EXPECT_NEAR(v1.x, v2.x, 0.0001f) << err_msg << endl;
        EXPECT_NEAR(v1.y, v2.y, 0.0001f) << err_msg << endl;
        EXPECT_NEAR(v1.z, v2.z, 0.0001f) << err_msg << endl;
        EXPECT_NEAR(v1.w, v2.w, 0.0001f) << err_msg << endl;
    }

    inline void equality(const Matrix2x2<float>& m1, const Matrix2x2<float>& m2)
    {
        for (size_t i = 0; i < 2; ++i) {
            for (size_t j = 0; j < 2; ++j) {
                EXPECT_NEAR(m1[i][j], m2[i][j], 0.0001f);
            }
        }
    }

    inline void equality(const Matrix2x2<float>& m1, const Matrix2x2<float>& m2, const string_view err_msg)
    {
        for (size_t i = 0; i < 2; ++i) {
            for (size_t j = 0; j < 2; ++j) {
                EXPECT_NEAR(m1[i][j], m2[i][j], 0.0001f) << err_msg << endl;
            }
        }
    }

    inline void equality(const Matrix2x2<float>& m, const float a[4])
    {
        for (size_t i = 0; i < 2; i++) {
            for (size_t j = 0; j < 2; j++) {
                EXPECT_NEAR(m.at(i, j), a[i * 2 + j], 0.0001f);
            }
        }
    }

    inline void equality(const Matrix2x2<float>& m, const float a[4], const string_view err_msg)
    {
        for (size_t i = 0; i < 2; i++) {
            for (size_t j = 0; j < 2; j++) {
                EXPECT_NEAR(m.at(i, j), a[i * 2 + j], 0.0001F) << err_msg << endl;
            }
        }
    }

#if (defined(__SSE__) || defined(__AVX2__))
    inline void equality(const Matrix2x2<float>& m, const __m128& a)
    {
        float v[4];
        _mm_store_ps(v, a);

        for (size_t i = 0; i < 2; i++) {
            for (size_t j = 0; j < 2; j++) {
                EXPECT_NEAR(m.at(i, j), v[i * 2 + j], 0.0001f);
            }
        }

    }

    inline void equality(const Matrix2x2<float>& m, const __m128& a, const string_view err_msg)
    {
        float v[4];
        _mm_store_ps(v, a);

        for (size_t i = 0; i < 2; i++) {
            for (size_t j = 0; j < 2; j++) {
                EXPECT_NEAR(m.at(i, j), v[i * 2 + j], 0.0001f) << err_msg << endl;
            }
        }
    }
#endif

    inline void equality(const Matrix3x3<float>& m1, const Matrix3x3<float>& m2)
    {
        for (size_t i = 0; i < 3; ++i) {
            for (size_t j = 0; j < 3; ++j) {
                EXPECT_NEAR(m1[i][j], m2[i][j], 0.0001f);
            }
        }
    }

    inline void equality(const Matrix3x3<float>& m1, const Matrix3x3<float>& m2, const string_view err_msg)
    {
        for (size_t i = 0; i < 3; ++i) {
            for (size_t j = 0; j < 3; ++j) {
                EXPECT_NEAR(m1[i][j], m2[i][j], 0.0001f) << err_msg << endl;
            }
        }
    }

    inline void equality(const Matrix3x3<float>& m, const float a[9])
    {
        for (size_t i = 0; i < 3; i++) {
            for (size_t j = 0; j < 3; j++) {
                EXPECT_NEAR(m.at(i, j), a[i * 3 + j], 0.0001f);
            }
        }
    }

    inline void equality(const Matrix3x3<float>& m, const float a[9], const string_view err_msg)
    {
        for (size_t i = 0; i < 3; i++) {
            for (size_t j = 0; j < 3; j++) {
                EXPECT_NEAR(m.at(i, j), a[i * 3 + j], 0.0001f) << err_msg << endl;
            }
        }
    }

    inline void equality(const Matrix4x4<float>& m1, const Matrix4x4<float>& m2)
    {
        for (size_t i = 0; i < 4; ++i) {
            for (size_t j = 0; j < 4; ++j) {
                EXPECT_NEAR(m1[i][j], m2[i][j], 0.0001f);
            }
        }
    }

    inline void equality(const Matrix4x4<float>& m1, const Matrix4x4<float>& m2, const string_view err_msg)
    {
        for (size_t i = 0; i < 4; ++i) {
            for (size_t j = 0; j < 4; ++j) {
                EXPECT_NEAR(m1[i][j], m2[i][j], 0.0001f) << err_msg << endl;
            }
        }
    }

    inline void equality(const Matrix4x4<float>& m, const float a[16])
    {
        for (size_t i = 0; i < 4; i++) {
            for (size_t j = 0; j < 4; j++) {
                EXPECT_NEAR(m.at(i, j), a[i * 4 + j], 0.0001f);
            }
        }
    }

    inline void equality(const Matrix4x4<float>& m, const float a[16], const string_view err_msg)
    {
        for (size_t i = 0; i < 4; i++) {
            for (size_t j = 0; j < 4; j++) {
                EXPECT_NEAR(m.at(i, j), a[i * 4 + j], 0.0001f) << err_msg << endl;
            }
        }
    }

    inline void equality(const Quaternion& q1, const Quaternion& q2)
    {
        EXPECT_NEAR(q1.v.x, q2.v.x, 0.0001f);
        EXPECT_NEAR(q1.v.y, q2.v.y, 0.0001f);
        EXPECT_NEAR(q1.v.z, q2.v.z, 0.0001f);
        EXPECT_NEAR(q1.w, q2.w, 0.0001f);
    }

    inline void equality(const Quaternion& q1, const Quaternion& q2, const string_view err_msg)
    {
        EXPECT_NEAR(q1.v.x, q2.v.x, 0.0001f) << err_msg << endl;
        EXPECT_NEAR(q1.v.y, q2.v.y, 0.0001f) << err_msg << endl;
        EXPECT_NEAR(q1.v.z, q2.v.z, 0.0001f) << err_msg << endl;
        EXPECT_NEAR(q1.w, q2.w, 0.0001f) << err_msg << endl;
    }

    inline void thisTrue(bool t)
    {
        EXPECT_TRUE(t);
    }

    inline void thisTrue(bool t, const string_view err_msg)
    {
        EXPECT_TRUE(t) << err_msg << endl;
    }

    inline void thisFalse(bool t)
    {
        EXPECT_FALSE(t);
    }

    inline void thisFalse(bool t, const string_view err_msg)
    {
        EXPECT_FALSE(t) << err_msg << endl;
    }
}

#endif