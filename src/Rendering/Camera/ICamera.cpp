//
//  ICamera.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 06/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include "ICamera.hpp"

namespace pbrlib
{
    ICamera::ICamera(
        const Transform&    view,
        const Transform&    projection,
        const Vec3<float>&  pos,
        const Vec3<float>&  eye,
        float               z_near,
        float               z_far
    ) :
        _view       (view),
        _projection (projection),
        _pos        (pos),
        _dir        (eye),
        _z_near     (z_near),
        _z_far      (z_far)
    {}

    Transform& ICamera::getView() noexcept
    {
        return _view;
    }

    const Transform& ICamera::getView() const noexcept
    {
        return _view;
    }

    Transform& ICamera::getProjection() noexcept
    {
        return _projection;
    }

    const Transform& ICamera::getProjection() const noexcept
    {
        return _projection;
    }

    Transform ICamera::getViewProjection() const noexcept
    {
        return _projection * _view;
    }

    Vec3<float>& ICamera::getPosition() noexcept
    {
        return _pos;
    }

    const Vec3<float>& ICamera::getPosition() const noexcept
    {
        return _pos;
    }

    Vec3<float>& ICamera::getDirection() noexcept
    {
        return _dir;
    }

    const Vec3<float>& ICamera::getDirection() const noexcept
    {
        return _dir;
    }

    float ICamera::getNearClipp() const noexcept
    {
        return _z_near;
    }

    float ICamera::getFarClipp() const noexcept
    {
        return _z_far;
    }

    void ICamera::setLookAt(
        const Vec3<float>& eye,
        const Vec3<float>& pos,
        const Vec3<float>& up
    )
    {
        _pos    = pos;
        _dir    = eye;
        _view   = Transform::lookAt(pos, eye, up);
    }

    void ICamera::setNearClipp(float near)
    {
        _z_near     = near;
        _projection = _calculateProjection();
    }

    void ICamera::setFarClipp(float far)
    {
        _z_far      = far;
        _projection = _calculateProjection();
    }

    void ICamera::setNearAndFarClipp(float near, float far)
    {
        _z_near     = near;
        _z_far      = far;
        _projection = _calculateProjection();
    }
}
