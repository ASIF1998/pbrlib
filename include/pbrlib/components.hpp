#pragma once

#include <pbrlib/math/matrix4x4.hpp>

#include <string>
#include <string_view>

namespace pbrlib::component
{
    struct Tag final
    {
        explicit Tag(std::string_view name);
    
        std::string name;
    };

    struct Transform final
    {
        math::mat4 transform;
    };
}