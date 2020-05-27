//
//  SpotLight.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 07/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include "SpotLight.hpp"

namespace pbrlib
{
    SpotLight::SpotLight(
        const Vec3<float>&  position,
        const Vec3<float>&  direction,
        float               inner_radius,
        float               outer_radius,
        const Vec3<float>&  color,
        float               intensity
    ) :
        Component       ("Spot Light"),
        _color          (color),
        _pos            (position),
        _dir            (direction),
        _intensity      (intensity),
        _inner_radius   (inner_radius),
        _outer_radius   (outer_radius)
    {}

    SpotLight::SpotLight(
        const string_view   name,
        const Vec3<float>&  position,
        const Vec3<float>&  direction,
        float               inner_radius,
        float               outer_radius,
        const Vec3<float>&  color,
        float               intensity
    ) :
        Component       (name),
        _color          (color),
        _pos            (position),
        _dir            (direction),
        _intensity      (intensity),
        _inner_radius   (inner_radius),
        _outer_radius   (outer_radius)
    {}

    void SpotLight::setIntensity(float intensity) noexcept
    {
        _intensity = intensity;
    }

    void SpotLight::setColor(const Vec3<float>& color)
    {
        _color = color;
    }

    void SpotLight::setPosition(const Vec3<float>& position)
    {
        _pos = position;
    }

    void SpotLight::setDirection(const Vec3<float>& direction)
    {
        _dir = direction;
    }

    void SpotLight::setInnerRadius(float inner_radius) noexcept
    {
        _inner_radius = inner_radius;
    }

    void SpotLight::setOuterRadius(float outer_radius) noexcept
    {
        _outer_radius = outer_radius;
    }

    float SpotLight::getIntensity() const noexcept
    {
        return _intensity;
    }

    Vec3<float>& SpotLight::getColor() noexcept
    {
        return _color;
    }

    const Vec3<float>& SpotLight::getColor() const noexcept
    {
        return _color;
    }

    Vec3<float>& SpotLight::getPosition() noexcept
    {
        return _pos;
    }

    const Vec3<float>& SpotLight::getPosition() const noexcept
    {
        return _pos;
    }

    Vec3<float>& SpotLight::getDirection() noexcept
    {
        return _dir;
    }

    const Vec3<float>& SpotLight::getDirection() const noexcept
    {
        return _dir;
    }

    float SpotLight::getInnerRadius() const noexcept
    {
        return _inner_radius;
    }

    float SpotLight::getOuterRadius() const noexcept
    {
        return _outer_radius;
    }

    type_index SpotLight::getType() const
    {
        return ComponentUtil::getTypeIndex<SpotLight>();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SpotLight::Builder::Builder(const string_view name) :
        _name(name)
    {}

    void SpotLight::Builder::setIntensity(float intensity) noexcept
    {
        _intensity = intensity;
    }

    void SpotLight::Builder::setColor(const Vec3<float>& color)
    {
        _color = color;
    }

    void SpotLight::Builder::setPosition(const Vec3<float>& position)
    {
        _pos = position;
    }

    void SpotLight::Builder::setDirection(const Vec3<float>& direction)
    {
        _dir = direction;
    }

    void SpotLight::Builder::setInnerRadius(float inner_radius) noexcept
    {
        _inner_radius = inner_radius;
    }

    void SpotLight::Builder::setOuterRadius(float outer_radius) noexcept
    {
        _outer_radius = outer_radius;
    }

    void SpotLight::Builder::setName(const string_view name)
    {
        _name = name;
    }

    SpotLight SpotLight::Builder::build() const
    {
        return SpotLight(
            _name,
            _pos,
            _dir,
            _inner_radius,
            _outer_radius,
            _color,
            _intensity
        );
    }

    PtrSpotLight SpotLight::Builder::buildPtr() const
    {
        return make_shared<SpotLight>(
            _name,
            _pos,
            _dir,
            _inner_radius,
            _outer_radius,
            _color,
            _intensity
        );
    }
}