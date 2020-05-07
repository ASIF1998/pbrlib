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
        float               z_far,
        const Viewport&     viewport
    ) :
        _view       (view),
        _projection (projection),
        _pos        (pos),
        _dir        (eye),
        _z_near     (z_near),
        _z_far      (z_far),
        _viewport   (viewport)
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

    Viewport& ICamera::getViewport() noexcept
    {
        return _viewport;
    }

    const Viewport& ICamera::getViewport() const noexcept
    {
        return _viewport;
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

    void ICamera::setViewport(const Viewport& viewport) noexcept
    {
        _viewport = viewport;
    }

    void ICamera::setViewport(
        float x,
        float y,
        float width,
        float height,
        float min_depth,
        float max_depth
    ) noexcept
    {
        _viewport.x         = x;
        _viewport.y         = y;
        _viewport.width     = width;
        _viewport.height    = height;
        _viewport.minDepth  = min_depth;
        _viewport.maxDepth  = max_depth;
    }
}
