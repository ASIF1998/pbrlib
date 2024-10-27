#pragma once

#include <pbrlib/math/vec3.hpp>
#include <pbrlib/math/matrix4x4.hpp>

#include <pbrlib/scene/component.hpp>

#include <numeric>

namespace pbrlib
{
    class Camera final :
        public Component<Camera>
    {
    public:
        struct Range
        {
            float near = 0.0f;
            float far = std::numeric_limits<float>::infinity();
        };

    public:
        void pos(const pbrlib::math::vec3& pos);
        void eye(const pbrlib::math::vec3& eye);
        void up(const pbrlib::math::vec3& up);

        void range(float near, float far);

        void fovY(float fov_y);
        void aspect(float aspect);

        [[nodiscard]] const pbrlib::math::vec3& pos()   const noexcept;
        [[nodiscard]] const pbrlib::math::vec3& eye()   const noexcept;
        [[nodiscard]] const pbrlib::math::vec3& up()    const noexcept;

        [[nodiscard]] const Range& range() const noexcept;

        [[nodiscard]] float fovY()      const noexcept;
        [[nodiscard]] float aspect()    const noexcept;

        [[nodiscard]] pbrlib::math::mat4 view()         const;
        [[nodiscard]] pbrlib::math::mat4 projection()   const;

    private:
        pbrlib::math::vec3 _pos = pbrlib::math::vec3(0, 0, -1);
        pbrlib::math::vec3 _eye = pbrlib::math::vec3(0, 0, 0);
        pbrlib::math::vec3 _up  = pbrlib::math::vec3(0, 1, 0);
        
        Range _range;

        float _fov_y    = 0.0f;
        float _aspect   = 0.0f;
    };
}