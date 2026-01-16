#pragma once

#include <backend/utils/generate_color.hpp>

namespace pbrlib::backend::vk::marker_colors
{
    constexpr auto write_data_in_buffer = generateColor(1);
    constexpr auto write_data_in_image  = generateColor(2);

    constexpr auto present_image = generateColor(3);

    constexpr auto graphics_pipeline    = generateColor(4);
    constexpr auto compute_pipeline     = generateColor(5);

    constexpr auto change_layout = generateColor(6);

    constexpr auto blit_image = generateColor(7);

    constexpr auto bilateral_blur   = generateColor(8);
    constexpr auto fxaa             = generateColor(9);

    constexpr auto clear = generateColor(10);
}
