#include <pbrlib/transforms.hpp>
#include <pbrlib/math/matrix4x4.hpp>
#include <pbrlib/math/casts.hpp>

#include <cmath>

namespace pbrlib::transforms
{
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
        const float sin_theta = sin(math::toRadians(theta));
        const float cos_theta = cos(math::toRadians(theta));

        return math::mat4 (
            1.0f, 0.0f,         0.0f,       0.0f,
            0.0f, cos_theta,    -sin_theta, 0.0f,
            0.0f, sin_theta,    cos_theta,  0.0f,
            0.0f, 0.0f,         0.0f,       1.0f
        );
    }

    math::mat4 rotateY(float theta)
    {
        const float sin_theta = sin(math::toRadians(theta));
        const float cos_theta = cos(math::toRadians(theta));

        return math::mat4 (
            cos_theta,  0.0f, sin_theta,    0.0f,
            0.0f,       1.0f, 0.0f,         0.0f,
            -sin_theta, 0.0f, cos_theta,    0.0f,
            0.0f,       0.0f, 0.0f,         1.0f
        );
    }

    math::mat4 rotateZ(float theta)
    {
        const float sin_theta = sin(math::toRadians(theta));
        const float cos_theta = cos(math::toRadians(theta));

        return math::mat4 (
            cos_theta,  -sin_theta, 0.0f, 0.0f,
            sin_theta,  cos_theta,  0.0f, 0.0f,
            0.0f,       0.0f,       1.0f, 0.0f,
            0.0f,       0.0f,       0.0f, 1.0f
        );
    }

    math::mat4 rotate(const math::vec3& axis, float theta)
    {
        const auto r = math::normalize(axis);

        const float theta_in_radians = math::toRadians(theta);

        const float sin_theta           = sin(theta_in_radians);
        const float cos_theta           = cos(theta_in_radians);
        const float one_minus_cos_theta = 1.0f - cos_theta;

        const float rxry = r.x * r.y;
        const float rxrz = r.x * r.z;
        const float ryrz = r.y * r.z;

        const float m11 = r.x * r.x * one_minus_cos_theta + cos_theta;
        const float m12 = rxry * one_minus_cos_theta - r.z * sin_theta;
        const float m13 = rxrz * one_minus_cos_theta + r.y * sin_theta;

        const float m21 = rxry * one_minus_cos_theta + r.z * sin_theta;
        const float m22 = r.y * r.y * one_minus_cos_theta + cos_theta;
        const float m23 = ryrz * one_minus_cos_theta - r.x * sin_theta;

        const float m31 = rxrz * one_minus_cos_theta - r.y * sin_theta;
        const float m32 = ryrz * one_minus_cos_theta + r.x * sin_theta;
        const float m33 = r.z * r.z * one_minus_cos_theta + cos_theta;

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
        const auto f = math::normalize(pos - eye);
        const auto s = math::normalize(math::cross(f, up));
        const auto u = math::cross(s, f);

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
        const auto rad = math::toRadians(fovy);

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
