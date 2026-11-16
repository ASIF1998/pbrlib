#pragma once

#include <cstdint>

namespace pbrlib::events
{
    struct ResizeWindow final
    {
        uint32_t width  = 0;
        uint32_t height = 0;
    };
}
