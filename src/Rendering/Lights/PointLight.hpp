//
//  PointLight.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 06/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef PointLight_hpp
#define PointLight_hpp

#include "../../math/vec3.hpp"

#include <memory>

using namespace std;

namespace pbrlib
{
    using namespace math;

    class PointLight;

    using PtrPointLight = shared_ptr<PointLight>;

    class PointLight
    {
    public:
        class Builder
        {
        public:
            void setIntensity(float intensity) noexcept;
            void setColor(const Vec3<float>& color);
            void setPosition(const Vec3<float>& position);

            PointLight      build()     const;
            PtrPointLight   buildPtr()  const;

        private:
            Vec3<float> _color;
            Vec3<float> _pos;
            float       _intensity;
        };

    public:
        PointLight(
            const Vec3<float>&  position,
            const Vec3<float>&  color,
            float               intensity
        );

        void setIntensity(float intensity) noexcept;
        void setColor(const Vec3<float>& color);
        void setPosition(const Vec3<float>& position);

        float                   getIntensity()  const noexcept;
        Vec3<float>&            getColor()      noexcept;
        const Vec3<float>&      getColor()      const noexcept;
        Vec3<float>&            getPosition()   noexcept;
        const Vec3<float>&      getPosition()   const noexcept;
        
    private:
        Vec3<float> _color;
        Vec3<float> _pos;
        float       _intensity;
    };
}

#endif /* PointLight_hpp */
