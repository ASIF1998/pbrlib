#include <pbrlib/transform.hpp>
#include <pbrlib/math/matrix4x4.hpp>

#include <cmath>
#include <numbers>

using namespace pbrlib::math;

namespace pbrlib
{
    float getRadians(float angle)
    {
        constexpr float p = static_cast<float>(std::numbers::pi_v<float>) / 180.0f;
        return angle * p;
    }

    /*  -------------------------   */

    Transform::Transform() :
        _m(1.0)
    {}

    Transform::Transform(const mat4& m) :
        _m(m)
    {}

    bool Transform::operator == (const Transform& t) const
    {
        return _m == t._m;
    }

    bool Transform::operator != (const Transform& t) const
    {
        return _m != t._m;
    }

    vec3 Transform::operator () (const vec3& v) const
    {
        return {
            _m[0][0] * v.x + _m[0][1] * v.y + _m[0][2] * v.z,
            _m[1][0] * v.x + _m[1][1] * v.y + _m[1][2] * v.z,
            _m[2][0] * v.x + _m[2][1] * v.y + _m[2][2] * v.z
        };
    }

    Transform Transform::operator * (const Transform& t) const
    {
        return Transform(_m * t._m);
    }

    bool Transform::identity() const
    {
        constexpr mat4 identity_mat (1.0f);
        return _m == identity_mat;
    }

    mat4& Transform::getMatrix() noexcept
    {
        return _m;
    }

    const mat4& Transform::getMatrix() const noexcept
    {
        return _m;
    }

    mat4 Transform::getInverseMatrix() const noexcept
    {
        return inverse(_m);
    }

    void Transform::setMatrix(const mat4& m)
    {
        _m = m;
    }

    Transform inverse(const Transform& t)
    {
        return Transform(inverse(t._m));
    }

    Transform transpose(const Transform& t)
    {
        return Transform(transpose(t._m));
    }

    Transform Transform::translate(const vec3& t)
    {
        return Transform(mat4(
           1.0f, 0.0f, 0.0f, 0.0f,
           0.0f, 1.0f, 0.0f, 0.0f,
           0.0f, 0.0f, 1.0f, 0.0f,
           t.x,  t.y,  t.z,  1.0f
       ));
    }

    Transform Transform::scale(const vec3& s)
    {
        return Transform(mat4(
            s.x,  0.0f, 0.0f, 0.0f,
            0.0f, s.y,  0.0f, 0.0f,
            0.0f, 0.0f, s.z,  0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        ));
    }

    Transform Transform::rotateX(float theta)
    {
        float sin_theta = sin(getRadians(theta));
        float cos_theta = cos(getRadians(theta));

        return Transform(mat4(
            1.0f, 0.0f,         0.0f,       0.0f,
            0.0f, cos_theta,    -sin_theta, 0.0f,
            0.0f, sin_theta,    cos_theta,  0.0f,
            0.0f, 0.0f,         0.0f,       1.0f
        ));
    }

    Transform Transform::rotateY(float theta)
    {
        float sin_theta = sin(getRadians(theta));
        float cos_theta = cos(getRadians(theta));

        return Transform(mat4(
            cos_theta,  0.0f, sin_theta,    0.0f,
            0.0f,       1.0f, 0.0f,         0.0f,
            -sin_theta, 0.0f, cos_theta,    0.0f,
            0.0f,       0.0f, 0.0f,         1.0f
        ));
    }

    Transform Transform::rotateZ(float theta)
    {
        float sin_theta = sin(getRadians(theta));
        float cos_theta = cos(getRadians(theta));

        return Transform(mat4(
            cos_theta,  -sin_theta, 0.0f, 0.0f,
            sin_theta,  cos_theta,  0.0f, 0.0f,
            0.0f,       0.0f,       1.0f, 0.0f,
            0.0f,       0.0f,       0.0f, 1.0f
        ));
    }

    Transform Transform::rotate(const vec3& axis, float theta)
    {
        vec3 r = normalize(axis);

        float theta_in_radians = getRadians(theta);

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

        return Transform(mat4(
            m11,    m12,    m13,    0.0f,
            m21,    m22,    m23,    0.0f,
            m31,    m32,    m33,    0.0f,
            0.0f,   0.0f,   0.0f,   1.0f
        ));
    }

    Transform Transform::lookAt(
        const vec3& pos, 
        const vec3& eye, 
        const vec3& up
    )
    {
        auto f = normalize(pos - eye);
        auto s = normalize(cross(f, up));
        auto u = cross(s, f);

        mat4 mat;

        mat[0][0] = s.x;
		mat[1][0] = s.y;
		mat[2][0] = s.z;
		mat[0][1] = u.x;
		mat[1][1] = u.y;
		mat[2][1] = u.z;
		mat[0][2] =-f.x;
		mat[1][2] =-f.y;
		mat[2][2] =-f.z;
        mat[3][0] =-dot(s, eye);
		mat[3][1] =-dot(u, eye);
		mat[3][2] = dot(f, eye);

        return Transform(mat);
    }

    Transform Transform::perspective(
        float fovy, 
        float aspect, 
        float z_near, 
        float z_far
    )
    {
        auto tan_half_fovy = std::tan(fovy * 0.5f);

        mat4 mat;
        mat[0][0] = 1.0f / (aspect * tan_half_fovy);
		mat[1][1] = 1.0f / (tan_half_fovy);
		mat[2][2] = z_far / (z_near - z_far);
		mat[2][3] = -1.0f;
		mat[3][2] = -(z_far * z_near) / (z_far - z_near);
		mat[3][3] = 0.0f;

        return Transform(mat);
    }
}
