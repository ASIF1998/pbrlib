#pragma once

#include <string>

#include <cstdint>

namespace pbrlib::settings
{
    struct SSAO
    {
        uint32_t blur_samples_count = 8;

        float spatial_sigma     = 1.5;
        float luminance_sigma   = 1.5;
    };
}

namespace pbrlib
{
    struct Config final
    {
        std::string title;

        uint32_t width  = 800;
        uint32_t height = 600;

        bool resible        = false;
        bool draw_in_window = true;

        settings::SSAO ssao;
    };
}