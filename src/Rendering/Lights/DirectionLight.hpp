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

#include "../../SceneGraph/Component.hpp"

#include <memory>

using namespace std;

namespace pbrlib
{
    using namespace math;

    class DirectionLight;

    using PtrDirectionLight = shared_ptr<DirectionLight>;

    class DirectionLight :
        public Component
    {
    public:
        class Builder
        {
        public:
            Builder(const string_view name = "Direction Light");

            void setIntensity(float intensity) noexcept;
            void setColor(const Vec3<float>& color);
            void setPosition(const Vec3<float>& position);
            void setDirection(const Vec3<float>& direction);
            void setName(const string_view name);

            DirectionLight      build()     const;
            PtrDirectionLight   buildPtr()  const;

        private:
            Vec3<float> _pos;
            Vec3<float> _dir;
            Vec3<float> _color;
            float       _intensity;
            string      _name;
        };

    public:
        DirectionLight(
            const Vec3<float>&  position,
            const Vec3<float>&  direction,
            const Vec3<float>&  color,
            float               intensity
        );

        DirectionLight(
            const string_view   name,
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

        virtual type_index getType() const override;

    private:
        Vec3<float> _pos;
        Vec3<float> _dir;
        Vec3<float> _color;
        float       _intensity;
    };
}

#endif /* DirectionLight_hpp */
