//
//  PointLight.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 06/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef PointLight_hpp
#define PointLight_hpp

#include <pbrlib/math/vec3.hpp>

#include <pbrlib/SceneGraph/Component.hpp>

#include <memory>

namespace pbrlib
{
    class PointLight :
        public Component<PointLight>
    {
    public:
        class Builder
        {
        public:
            Builder(const std::string_view name = "Point Light");

            void setIntensity(float intensity) noexcept;
            void setColor(const math::Vec3<float>& color);
            void setPosition(const math::Vec3<float>& position);
            void setName(const std::string_view name);

            PointLight                  build()     const;
            std::unique_ptr<PointLight> buildPtr()  const;

        private:
            math::Vec3<float>   _color;
            math::Vec3<float>   _pos;
            float               _intensity;
            std::string         _name;
        };

    public:
        PointLight(
            const math::Vec3<float>&    position,
            const math::Vec3<float>&    color,
            float                       intensity
        );

        PointLight(
            const std::string_view      name,
            const math::Vec3<float>&    position,
            const math::Vec3<float>&    color,
            float                       intensity
        );

        void setIntensity(float intensity) noexcept;
        void setColor(const math::Vec3<float>& color);
        void setPosition(const math::Vec3<float>& position);

        float                           getIntensity()  const noexcept;
        math::Vec3<float>&              getColor()      noexcept;
        const math::Vec3<float>&        getColor()      const noexcept;
        math::Vec3<float>&              getPosition()   noexcept;
        const math::Vec3<float>&        getPosition()   const noexcept;
        
    private:
        math::Vec3<float>   _color;
        math::Vec3<float>   _pos;
        float               _intensity;
    };
}

#endif /* PointLight_hpp */
