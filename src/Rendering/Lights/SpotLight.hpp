//
//  SpotLight.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 07/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef SpotLight_hpp
#define SpotLight_hpp

#include "../../math/vec3.hpp"

#include <memory>

using namespace std;

namespace pbrlib
{
    using namespace math;

    class SpotLight;

    using PtrSpotLight = shared_ptr<SpotLight>;

    class SpotLight
    {
    public:
        class Builder
        {
        public:
            void setIntensity(float intensity)                  noexcept;
            void setColor(const Vec3<float>& color);
            void setPosition(const Vec3<float>& position);
            void setDirection(const Vec3<float>& direction);
            void setInnerRadius(float inner_radius)             noexcept;
            void setOuterRadius(float outer_radius)             noexcept;

            SpotLight       build()     const;
            PtrSpotLight    buildPtr()  const;

        private:
            Vec3<float> _pos;
            Vec3<float> _dir;
            Vec3<float> _color;
            float       _intensity;
            float       _inner_radius;
            float       _outer_radius;
        };

    public:
        SpotLight(
            const Vec3<float>&  position,
            const Vec3<float>&  direction,
            float               inner_radius,
            float               outer_radius,
            const Vec3<float>&  color,
            float               intensity
        );

        void setIntensity(float intensity)                  noexcept;
        void setColor(const Vec3<float>& color);
        void setPosition(const Vec3<float>& position);
        void setDirection(const Vec3<float>& direction);
        void setInnerRadius(float inner_radius)             noexcept;
        void setOuterRadius(float outer_radius)             noexcept;

        float                   getIntensity()      const noexcept;
        Vec3<float>&            getColor()          noexcept;
        const Vec3<float>&      getColor()          const noexcept;
        Vec3<float>&            getPosition()       noexcept;
        const Vec3<float>&      getPosition()       const noexcept;
        Vec3<float>&            getDirection()      noexcept;
        const Vec3<float>&      getDirection()      const noexcept;
        float                   getInnerRadius()    const noexcept;
        float                   getOuterRadius()    const noexcept;

    private:
        Vec3<float> _pos;
        Vec3<float> _dir;
        Vec3<float> _color;
        float       _intensity;
        float       _inner_radius;
        float       _outer_radius;
    };
}

#endif /* SpotLight_hpp */
