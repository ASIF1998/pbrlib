//
//  PointLight.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 06/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include "PointLight.hpp"

namespace pbrlib
{
    PointLight::PointLight(
        const Vec3<float>&  position,
        const Vec3<float>&  color,
        float               intensity
    ) :
        _intensity  (intensity),
        _color      (color),
        _pos        (position)
    {}

    void PointLight::setIntensity(float intensity) noexcept
    {
        _intensity = intensity;
    }

    void PointLight::setColor(const Vec3<float>& color)
    {
        _color = color;
    }

    void PointLight::setPosition(const Vec3<float>& position)
    {
        _pos = position;
    }

    float PointLight::getIntensity() const noexcept
    {
        return _intensity;
    }

    Vec3<float>& PointLight::getColor() noexcept
    {
        return _color;
    }

    const Vec3<float>& PointLight::getColor() const noexcept
    {
        return _color;
    }

    Vec3<float>& PointLight::getPosition() noexcept
    {
        return _pos;
    }

    const Vec3<float>& PointLight::getPosition() const noexcept
    {
        return _pos;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void PointLight::Builder::setIntensity(float intensity) noexcept
    {
        _intensity = intensity;
    }

    void PointLight::Builder::setColor(const Vec3<float>& color)
    {
        _color = color;
    }

    void PointLight::Builder::setPosition(const Vec3<float>& position)
    {
        _pos = position;
    }

    PointLight PointLight::Builder::build() const
    {
        return PointLight(_pos, _color, _intensity);
    }

    PtrPointLight PointLight::Builder::buildPtr() const
    {
        return make_shared<PointLight>(_pos, _color, _intensity);
    }
}