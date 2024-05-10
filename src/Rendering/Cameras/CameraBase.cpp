//
//  CameraBase.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 06/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <pbrlib/Rendering/Cameras/CameraBase.hpp>

namespace pbrlib
{
    CameraBase::CameraBase(
        const string_view   component_name,
        const Transform&    view,
        const Transform&    projection,
        const Vec3<float>&  pos,
        const Vec3<float>&  eye,
        float               z_near,
        float               z_far,
        const Viewport&     viewport
    ) :
        Component   (component_name),
        _view       (view),
        _projection (projection),
        _pos        (pos),
        _dir        (eye),
        _z_near     (z_near),
        _z_far      (z_far),
        _viewport   (viewport)
    {}

    Transform& CameraBase::getView() noexcept
    {
        return _view;
    }

    const Transform& CameraBase::getView() const noexcept
    {
        return _view;
    }

    Transform& CameraBase::getProjection() noexcept
    {
        return _projection;
    }

    const Transform& CameraBase::getProjection() const noexcept
    {
        return _projection;
    }

    Transform CameraBase::getViewProjection() const noexcept
    {
        return _projection * _view;
    }

    Vec3<float>& CameraBase::getPosition() noexcept
    {
        return _pos;
    }

    const Vec3<float>& CameraBase::getPosition() const noexcept
    {
        return _pos;
    }

    Vec3<float>& CameraBase::getDirection() noexcept
    {
        return _dir;
    }

    const Vec3<float>& CameraBase::getDirection() const noexcept
    {
        return _dir;
    }

    float CameraBase::getNearClipp() const noexcept
    {
        return _z_near;
    }

    float CameraBase::getFarClipp() const noexcept
    {
        return _z_far;
    }

    Viewport& CameraBase::getViewport() noexcept
    {
        return _viewport;
    }

    const Viewport& CameraBase::getViewport() const noexcept
    {
        return _viewport;
    }

    void CameraBase::setLookAt(
        const Vec3<float>& eye,
        const Vec3<float>& pos,
        const Vec3<float>& up
    )
    {
        _pos    = pos;
        _dir    = eye;
        _view   = Transform::lookAt(pos, eye, up);
    }

    void CameraBase::setNearClipp(float near)
    {
        _z_near     = near;
        _projection = _calculateProjection();
    }

    void CameraBase::setFarClipp(float far)
    {
        _z_far      = far;
        _projection = _calculateProjection();
    }

    void CameraBase::setNearAndFarClipp(float near, float far)
    {
        _z_near     = near;
        _z_far      = far;
        _projection = _calculateProjection();
    }

    void CameraBase::setViewport(const Viewport& viewport) noexcept
    {
        _viewport = viewport;
    }

    void CameraBase::setViewport(
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

    type_index CameraBase::getType() const
    {
        return ComponentUtil::getTypeIndex<CameraBase>();
    }
}
