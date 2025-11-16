#pragma once

#include <pbrlib/math/matrix4x4.hpp>
#include <pbrlib/math/vec3.hpp>
#include <pbrlib/math/quat.hpp>

namespace pbrlib::transforms
{
    [[nodiscard]] math::mat4 translate(const math::vec3& t);
    [[nodiscard]] math::mat4 scale(const math::vec3& s);

    [[nodiscard]] math::mat4 rotateX(float theta);
    [[nodiscard]] math::mat4 rotateY(float theta);
    [[nodiscard]] math::mat4 rotateZ(float theta);

    [[nodiscard]] math::mat4 rotate(const math::vec3& axis, float theta);

    [[nodiscard]] math::mat4 lookAt (
        const math::vec3& eye, 
        const math::vec3& pos, 
        const math::vec3& up
    );

    [[nodiscard]] math::mat4 perspective (
        float       fovy, 
        uint32_t    width,
        uint32_t    height,
        float       z_near, 
        float       z_far
    );

    [[nodiscard]] math::quat angleAxis(float angle, const math::vec3& axis);
}
