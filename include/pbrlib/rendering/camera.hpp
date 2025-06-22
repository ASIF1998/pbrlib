#pragma once

#include <pbrlib/math/vec3.hpp>
#include <pbrlib/math/matrix4x4.hpp>

#include <numeric>

namespace pbrlib
{
    class Camera final
    {
    public:
        struct Range
        {
            float near_dist = 0.0f;
            float far_dist  = std::numeric_limits<float>::infinity();
        };

    public:
        void pos(const math::vec3& pos);
        void eye(const math::vec3& eye);
        void up(const math::vec3& up);

        void range(float near_dist, float far_dist);

        void fovY(float fov_y);

        void width(uint32_t w);
        void height(uint32_t h);

        [[nodiscard]] const math::vec3& pos()   const noexcept;
        [[nodiscard]] const math::vec3& eye()   const noexcept;
        [[nodiscard]] const math::vec3& up()    const noexcept;

        [[nodiscard]] const Range& range() const noexcept;

        [[nodiscard]] float fovY() const noexcept;

        [[nodiscard]] uint32_t width()  const noexcept;
        [[nodiscard]] uint32_t height() const noexcept;

        [[nodiscard]] math::mat4 view()         const;
        [[nodiscard]] math::mat4 projection()   const;

        void translate(const math::vec3& t);
        void moveEye(const math::vec3& t);

    private:
        math::vec3 _pos = math::vec3(0, 0, -1);
        math::vec3 _eye = math::vec3(0, 0, 0);
        math::vec3 _up  = math::vec3(0, 1, 0);
        
        Range _range;

        float _fov_y = 0.0f;

        uint32_t _width     = 0;
        uint32_t _height    = 0;
    };
}