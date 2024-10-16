//
//  DirectionLight.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 07/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef DirectionLight_hpp
#define DirectionLight_hpp

#include <pbrlib/math/vec3.hpp>

#include <pbrlib/scene/Component.hpp>

#include <memory>

namespace pbrlib
{
    class DirectionLight :
        public Component<DirectionLight>
    {
    public:
        class Builder
        {
        public:
            Builder(const std::string_view name = "Direction Light");

            void setIntensity(float intensity) noexcept;
            void setColor(const math::Vec3<float>& color);
            void setDirectionToLight(const math::Vec3<float>& direction);
            void setName(const std::string_view name);

            DirectionLight                  build()     const;
            std::unique_ptr<DirectionLight> buildPtr()  const;

        private:
            math::Vec3<float>   _dir_to_light;
            math::Vec3<float>   _color;
            float               _intensity;
            std::string         _name;
        };

    public:
        DirectionLight(
            const math::Vec3<float>&    direction_to_light,
            const math::Vec3<float>&    color,
            float                       intensity
        );

        DirectionLight(
            const std::string_view      name,
            const math::Vec3<float>&    direction_to_light,
            const math::Vec3<float>&    color,
            float                       intensity
        );

        void setIntensity(float intensity) noexcept;
        void setColor(const math::Vec3<float>& color);
        void setDirectionToLight(const math::Vec3<float>& direction);

        float                       getIntensity()          const noexcept;
        math::Vec3<float>&          getColor()              noexcept;
        const math::Vec3<float>&    getColor()              const noexcept;
        math::Vec3<float>&          getDirectionToLight()   noexcept;
        const math::Vec3<float>&    getDirectionToLight()   const noexcept;

    private:
        math::Vec3<float>   _dir_to_lihght;
        math::Vec3<float>   _color;
        float               _intensity;
    };
}

#endif /* DirectionLight_hpp */
