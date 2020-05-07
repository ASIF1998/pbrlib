//
//  DirectionLight.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 07/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef DirectionLight_hpp
#define DirectionLight_hpp

#include "../../math/vec3.hpp"

#include <memory>

using namespace std;

namespace pbrlib
{
    using namespace math;

    class DirectionLight;

    using PtrDirectionLight = shared_ptr<DirectionLight>;

    class DirectionLight
    {
    public:
        class Builder
        {
        public:
            void setIntensity(float intensity) noexcept;
            void setColor(const Vec3<float>& color);
            void setPosition(const Vec3<float>& position);
            void setDirection(const Vec3<float>& direction);

            DirectionLight      build()     const;
            PtrDirectionLight   buildPtr()  const;

        private:
            Vec3<float> _pos;
            Vec3<float> _dir;
            Vec3<float> _color;
            float       _intensity;
        };

    public:
        DirectionLight(
            const Vec3<float>&  position,
            const Vec3<float>&  direction,
            const Vec3<float>&  color,
            float               intensity
        );

        void setIntensity(float intensity) noexcept;
        void setColor(const Vec3<float>& color);
        void setPosition(const Vec3<float>& position);
        void setDirection(const Vec3<float>& direction);

        float               getIntensity()  const noexcept;
        Vec3<float>&        getColor()      noexcept;
        const Vec3<float>&  getColor()      const noexcept;
        Vec3<float>&        getPosition()   noexcept;
        const Vec3<float>&  getPosition()   const noexcept;
        Vec3<float>&        getDirection()  noexcept;
        const Vec3<float>&  getDirection()  const noexcept;

    private:
        Vec3<float> _pos;
        Vec3<float> _dir;
        Vec3<float> _color;
        float       _intensity;
    };
}

#endif /* DirectionLight_hpp */
