#pragma once

#include <string>

namespace pbrlib::backend::vk::shader
{
    struct Define final
    {
        std::string name;
        std::string value;
    };
}