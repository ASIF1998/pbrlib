#include <pbrlib/transforms.hpp>
#include <pbrlib/math/matrix4x4.hpp>

#include <cmath>
#include <numbers>

namespace pbrlib::transforms
{
    float radians(float angle)
    {
        constexpr float p = static_cast<float>(std::numbers::pi_v<float>) / 180.0f;
        return angle * p;
    }

    math::mat4 translate(const math::vec3& t)
    {
        return math::mat4 (
           1.0f, 0.0f, 0.0f, 0.0f,
           0.0f, 1.0f, 0.0f, 0.0f,
           0.0f, 0.0f, 1.0f, 0.0f,
           t.x,  t.y,  t.z,  1.0f
       );
    }

    math::mat4 scale(const math::vec3& s)
    {
        return math::mat4 (
            s.x,  0.0f, 0.0f, 0.0f,
            0.0f, s.y,  0.0f, 0.0f,
            0.0f, 0.0f, s.z,  0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
    }

    math::mat4 rotateX(float theta)
    {
        float sin_theta = sin(radians(theta));
        float cos_theta = cos(radians(theta));

        return math::mat4 (
            1.0f, 0.0f,         0.0f,       0.0f,
            0.0f, cos_theta,    -sin_theta, 0.0f,
            0.0f, sin_theta,    cos_theta,  0.0f,
            0.0f, 0.0f,         0.0f,       1.0f
        );
    }

    math::mat4 rotateY(float theta)
    {
        float sin_theta = sin(radians(theta));
        float cos_theta = cos(radians(theta));

        return math::mat4 (
            cos_theta,  0.0f, sin_theta,    0.0f,
            0.0f,       1.0f, 0.0f,         0.0f,
            -sin_theta, 0.0f, cos_theta,    0.0f,
            0.0f,       0.0f, 0.0f,         1.0f
        );
    }

    math::mat4 rotateZ(float theta)
    {
        float sin_theta = sin(radians(theta));
        float cos_theta = cos(radians(theta));

        return math::mat4 (
            cos_theta,  -sin_theta, 0.0f, 0.0f,
            sin_theta,  cos_theta,  0.0f, 0.0f,
            0.0f,       0.0f,       1.0f, 0.0f,
            0.0f,       0.0f,       0.0f, 1.0f
        );
    }

    math::mat4 rotate(const math::vec3& axis, float theta)
    {
        math::vec3 r = math::normalize(axis);

        float theta_in_radians = radians(theta);

        float sin_theta             = sin(theta_in_radians);
        float cos_theta             = cos(theta_in_radians);
        float one_minus_cos_theta   = 1.0f - cos_theta;

        float rxry = r.x * r.y;
        float rxrz = r.x * r.z;
        float ryrz = r.y * r.z;

        float m11 = r.x * r.x * one_minus_cos_theta + cos_theta;
        float m12 = rxry * one_minus_cos_theta - r.z * sin_theta;
        float m13 = rxrz * one_minus_cos_theta + r.y * sin_theta;

        float m21 = rxry * one_minus_cos_theta + r.z * sin_theta;
        float m22 = r.y * r.y * one_minus_cos_theta + cos_theta;
        float m23 = ryrz * one_minus_cos_theta - r.x * sin_theta;

        float m31 = rxrz * one_minus_cos_theta - r.y * sin_theta;
        float m32 = ryrz * one_minus_cos_theta + r.x * sin_theta;
        float m33 = r.z * r.z * one_minus_cos_theta + cos_theta;

        return math::mat4 (
            m11,    m12,    m13,    0.0f,
            m21,    m22,    m23,    0.0f,
            m31,    m32,    m33,    0.0f,
            0.0f,   0.0f,   0.0f,   1.0f
        );
    }

    math::mat4 lookAt (
        const math::vec3& eye, 
        const math::vec3& pos, 
        const math::vec3& up
    )
    {
        auto f = math::normalize(pos - eye);
        auto s = math::normalize(math::cross(f, up));
        auto u = math::cross(s, f);

        math::mat4 mat;

        mat[0][0] = s.x;
		mat[1][0] = s.y;
		mat[2][0] = s.z;
		mat[0][1] = u.x;
		mat[1][1] = u.y;
		mat[2][1] = u.z;
		mat[0][2] =-f.x;
		mat[1][2] =-f.y;
		mat[2][2] =-f.z;
        mat[3][0] =-math::dot(s, eye);
		mat[3][1] =-math::dot(u, eye);
		mat[3][2] = math::dot(f, eye);

        return mat;
    }

    math::mat4 perspective (
        float       fovy,
        uint32_t    width,
        uint32_t    height,
        float       z_near, 
        float       z_far
    )
    {
        const auto rad = radians(fovy);

        math::mat4 mat;

		mat[1][1] = std::cos(0.5f * rad) / std::sin(0.5f * rad);
		mat[0][0] = mat[1][1] * static_cast<float>(height) / width;
		mat[2][2] = z_far / (z_near - z_far);
		mat[2][3] = -1.0f;
		mat[3][2] = -(z_far * z_near) / (z_far - z_near);
        mat[3][3] = 0.0f;

        return mat;
    }

    math::quat angleAxis(float angle, const math::vec3& axis)
    {
        return math::quat(math::normalize(axis) * std::sinf(angle * 0.5f), std::cosf(angle * 0.5f));
    }
}
