//
//  DirectionLight.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 07/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include "DirectionLight.hpp"

namespace pbrlib
{
    DirectionLight::DirectionLight(
        const Vec3<float>&  position,
        const Vec3<float>&  direction,
        const Vec3<float>&  color,
        float               intensity
    ) :
        Component   ("Direction Light"),
        _pos        (position),
        _dir        (direction),
        _color      (color),
        _intensity  (intensity)
    {}

    DirectionLight::DirectionLight(
        const string_view   name,
        const Vec3<float>&  position,
        const Vec3<float>&  direction,
        const Vec3<float>&  color,
        float               intensity
    ) :
        Component   (name),
        _pos        (position),
        _dir        (direction),
        _color      (color),
        _intensity  (intensity)
    {}

    void DirectionLight::setIntensity(float intensity) noexcept
    {
        _intensity = intensity;
    }

    void DirectionLight::setColor(const Vec3<float>& color)
    {
        _color = color;
    }

    void DirectionLight::setPosition(const Vec3<float>& position)
    {
        _pos = position;
    }

    void DirectionLight::setDirection(const Vec3<float>& direction)
    {
        _dir = direction;
    }

    float DirectionLight::getIntensity() const noexcept
    {
        return _intensity;
    }

    Vec3<float>& DirectionLight::getColor() noexcept
    {
        return _color;
    }

    const Vec3<float>& DirectionLight::getColor() const noexcept
    {
        return _color;
    }

    Vec3<float>& DirectionLight::getPosition() noexcept
    {
        return _pos;
    }

    const Vec3<float>& DirectionLight::getPosition() const noexcept
    {
        return _pos;
    }

    Vec3<float>& DirectionLight::getDirection() noexcept
    {
        return _dir;
    }

    const Vec3<float>& DirectionLight::getDirection() const noexcept
    {
        return _dir;
    }

    type_index DirectionLight::getType() const
    {
        return ComponentUtil::getTypeIndex<DirectionLight>();
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
    DirectionLight::Builder::Builder(const string_view name) :
        _name(name)
    {}

    void DirectionLight::Builder::setIntensity(float intensity) noexcept
    {
        _intensity = intensity;
    }

    void DirectionLight::Builder::setColor(const Vec3<float>& color)
    {
        _color = color;
    }

    void DirectionLight::Builder::setPosition(const Vec3<float>& position)
    {
        _pos = position;
    }

    void DirectionLight::Builder::setDirection(const Vec3<float>& direction)
    {
        _dir = direction;
    }

    void DirectionLight::Builder::setName(const string_view name)
    {
        _name = name;
    }

    DirectionLight DirectionLight::Builder::build() const
    {
        return DirectionLight(_name, _pos, _dir, _color, _intensity);
    }

    PtrDirectionLight DirectionLight::Builder::buildPtr() const
    {
        return make_shared<DirectionLight>(_name, _pos, _dir, _color, _intensity);
    }
}