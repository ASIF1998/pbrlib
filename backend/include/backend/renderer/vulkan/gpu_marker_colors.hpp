#pragma once

#include <backend/utils/generate_color.hpp>

namespace pbrlib::vk::marker_colors
{
    constexpr auto write_data_in_buffer = generateColor(1);
    constexpr auto write_data_in_image  = generateColor(2);
    
    constexpr auto present_image = generateColor(3);

    constexpr auto graphics_pipeline = generateColor(4);
}