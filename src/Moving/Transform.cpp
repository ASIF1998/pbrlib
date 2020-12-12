//
//  Transform.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 17/04/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include "Transform.hpp"

#include "../math/matrix4x4.hpp"

#include "../Rendering/Geometry/AABB.hpp"

#include <cmath>

namespace pbrlib
{
    float getRadians(float angle)
    {
        constexpr float p = M_PI / 180.0f;
        return angle * p;
    }

    /*  -------------------------   */
    constexpr Matrix4x4<float> Transform::IDENTITY_MATRIX;

    Transform::Transform() :
        _m(IDENTITY_MATRIX)
    {}

    Transform::Transform(const Matrix4x4<float>& m) :
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

    Vec3<float> Transform::operator () (const Vec3<float>& v) const
    {
        return {
            _m[0][0] * v.x + _m[0][1] * v.y + _m[0][2] * v.z,
            _m[1][0] * v.x + _m[1][1] * v.y + _m[1][2] * v.z,
            _m[2][0] * v.x + _m[2][1] * v.y + _m[2][2] * v.z
        };
    }

    AABB Transform::operator () (const AABB& bbox) const
    {
        AABB new_bbox ((*this)(bbox.corner(0)));
        
        new_bbox = AABB::aabbUnion(new_bbox, (*this)(bbox.corner(1)));
        new_bbox = AABB::aabbUnion(new_bbox, (*this)(bbox.corner(2)));
        new_bbox = AABB::aabbUnion(new_bbox, (*this)(bbox.corner(3)));
        new_bbox = AABB::aabbUnion(new_bbox, (*this)(bbox.corner(4)));
        new_bbox = AABB::aabbUnion(new_bbox, (*this)(bbox.corner(5)));
        new_bbox = AABB::aabbUnion(new_bbox, (*this)(bbox.corner(6)));
        new_bbox = AABB::aabbUnion(new_bbox, (*this)(bbox.corner(7)));

        return new_bbox;
    }

    Transform Transform::operator * (const Transform& t) const
    {
        return Transform(_m * t._m);
    }

    bool Transform::identity() const
    {
        return _m == IDENTITY_MATRIX;
    }

    Matrix4x4<float>& Transform::getMatrix() noexcept
    {
        return _m;
    }

    const Matrix4x4<float>& Transform::getMatrix() const noexcept
    {
        return _m;
    }

    Matrix4x4<float> Transform::getInverseMatrix() const noexcept
    {
        return inverse(_m);
    }

    void Transform::setMatrix(const Matrix4x4<float>& m)
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

    Transform Transform::translate(const Vec3<float>& t)
    {
       return Transform(Matrix4x4<float>(
           1.0f, 0.0f, 0.0f, 0.0f,
           0.0f, 1.0f, 0.0f, 0.0f,
           0.0f, 0.0f, 1.0f, 0.0f,
           t.x,  t.y,  t.z,  1.0f
       ));
    }

    Transform Transform::scale(const Vec3<float>& s)
    {
        return Transform(Matrix4x4<float>(
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

        return Transform(Matrix4x4<float>(
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

        return Transform(Matrix4x4<float>(
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

        return Transform(Matrix4x4<float>(
            cos_theta,  -sin_theta, 0.0f, 0.0f,
            sin_theta,  cos_theta,  0.0f, 0.0f,
            0.0f,       0.0f,       1.0f, 0.0f,
            0.0f,       0.0f,       0.0f, 1.0f
        ));
    }

    Transform Transform::rotate(const Vec3<float>& axis, float theta)
    {
        Vec3<float> r = normalize(axis);

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

        return Transform(Matrix4x4<float>(
            m11,    m12,    m13,    0.0f,
            m21,    m22,    m23,    0.0f,
            m31,    m32,    m33,    0.0f,
            0.0f,   0.0f,   0.0f,   1.0f
        ));
    }

    Transform Transform::lookAt(
        const Vec3<float>& pos, 
        const Vec3<float>& eye, 
        const Vec3<float>& up
    )
    {
        Vec3<float> d = normalize(pos - eye);
        Vec3<float> r = normalize(cross(d, up));
        Vec3<float> u = cross(r, d);

        return Transform(Matrix4x4<float>(
           r.x,            u.x,            -d.x,            0.0f,
           r.y,            u.y,            -d.y,            0.0f,
           r.z,            u.z,            -d.z,            0.0f,
           -dot(r, eye),   -dot(u, eye),   dot(d, eye),     1.0f
       ));
    }

    Transform Transform::perspective(
        float fovy, 
        float aspect, 
        float z_near, 
        float z_far
    )
    {
        float a = tan(fovy / 2.0f);

        return Transform(Matrix4x4<float>(
           1.0f / (aspect * a),    0.0f,       0.0f,                                            0.0f,
           0.0f,                   -1.0f / a,  0.0f,                                            0.0f,
           0.0f,                   0.0f,       z_far / (z_near - z_far),                        -1.0f,
           0.0f,                   0.0f,       -(z_far * z_near) / (z_far - z_near),            0.0f
       ));
    }

    ostream& operator << (ostream& print, const Transform& t)
    {
        print << t._m;
        return print;
    }
}
