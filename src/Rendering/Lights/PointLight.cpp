//
//  PointLight.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 06/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <pbrlib/Rendering/Lights/PointLight.hpp>

using namespace std;
using namespace pbrlib::math;

namespace pbrlib
{
    PointLight::PointLight(
        const Vec3<float>&  position,
        const Vec3<float>&  color,
        float               intensity
    ) :
        Component   ("Point Light"),
        _color      (color),
        _pos        (position),
        _intensity  (intensity)
    {}
    
    PointLight::PointLight(
        const string_view   name,
        const Vec3<float>&  position,
        const Vec3<float>&  color,
        float               intensity
    ) :
        Component   (name),
        _color      (color),
        _pos        (position),
        _intensity  (intensity)
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
    PointLight::Builder::Builder(const string_view name) :
        _name(name)
    {}

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

    void PointLight::Builder::setName(const string_view name) 
    {
        _name = name;
    }

    PointLight PointLight::Builder::build() const
    {
        return PointLight(_name, _pos, _color, _intensity);
    }

    PtrPointLight PointLight::Builder::buildPtr() const
    {
        return make_shared<PointLight>(_name, _pos, _color, _intensity);
    }
}
