#pragma once

#include <backend/utils/generate_color.hpp>

namespace pbrlib
{
    struct GpuMarkerColors final
    {
        constexpr static auto write_data_in_buffer  = utils::generateColor(1);
        constexpr static auto write_data_in_image   = utils::generateColor(2);
    };
}