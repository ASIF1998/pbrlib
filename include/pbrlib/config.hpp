#pragma once

#include <string>

#include <cstdint>

namespace pbrlib
{
    struct Config
    {
        std::string title;

        uint32_t width  = 800;
        uint32_t height = 600;

        bool resible = false;
    };
}