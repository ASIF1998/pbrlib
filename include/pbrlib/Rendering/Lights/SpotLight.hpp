//
//  SpotLight.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 07/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef SpotLight_hpp
#define SpotLight_hpp

#include <pbrlib/math/vec3.hpp>

#include <pbrlib/SceneGraph/Component.hpp>

#include <memory>

namespace pbrlib
{
    class SpotLight;

    using PtrSpotLight = std::shared_ptr<SpotLight>;

    class SpotLight :
        public Component<SpotLight>
    {
    public:
        class Builder
        {
        public:
            Builder(const std::string_view name = "Spot Light");

            void setIntensity(float intensity)                      noexcept;
            void setColor(const math::Vec3<float>& color);
            void setPosition(const math::Vec3<float>& position);
            void setDirection(const math::Vec3<float>& direction);
            void setInnerRadius(float inner_radius)                 noexcept;
            void setOuterRadius(float outer_radius)                 noexcept;
            void setName(const std::string_view name);

            SpotLight       build()     const;
            PtrSpotLight    buildPtr()  const;

        private:
            math::Vec3<float>   _pos;
            math::Vec3<float>   _dir;
            math::Vec3<float>   _color;
            float               _intensity;
            float               _inner_radius;
            float               _outer_radius;
            std::string         _name;
        };

    public:
        SpotLight(
            const math::Vec3<float>&    position,
            const math::Vec3<float>&    direction,
            float                       inner_radius,
            float                       outer_radius,
            const math::Vec3<float>&    color,
            float                       intensity
        );

        SpotLight(
            const std::string_view      name,
            const math::Vec3<float>&    position,
            const math::Vec3<float>&    direction,
            float                       inner_radius,
            float                       outer_radius,
            const math::Vec3<float>&    color,
            float                       intensity
        );

        void setIntensity(float intensity)                      noexcept;
        void setColor(const math::Vec3<float>& color);
        void setPosition(const math::Vec3<float>& position);
        void setDirection(const math::Vec3<float>& direction);
        void setInnerRadius(float inner_radius)                 noexcept;
        void setOuterRadius(float outer_radius)                 noexcept;

        float                           getIntensity()      const noexcept;
        math::Vec3<float>&              getColor()          noexcept;
        const math::Vec3<float>&        getColor()          const noexcept;
        math::Vec3<float>&              getPosition()       noexcept;
        const math::Vec3<float>&        getPosition()       const noexcept;
        math::Vec3<float>&              getDirection()      noexcept;
        const math::Vec3<float>&        getDirection()      const noexcept;
        float                           getInnerRadius()    const noexcept;
        float                           getOuterRadius()    const noexcept;

    private:
        math::Vec3<float>   _pos;
        math::Vec3<float>   _dir;
        math::Vec3<float>   _color;
        float               _intensity;
        float               _inner_radius;
        float               _outer_radius;
    };
}

#endif /* SpotLight_hpp */
