#pragma once

#include <string>

#include <cstdint>

namespace pbrlib::settings
{
    struct SSAO final
    {
        uint32_t blur_samples_count = 8;

        float spatial_sigma     = 1.5;
        float luminance_sigma   = 1.5;

        float radius = 0.05f;
    };

    enum class AA : 
        uint8_t
    {
        eNone,
        eFXAA
    };

    struct FXAA final
    {
        float span_max      = 0.5f;
        float reduce_min    = 0.5f;
        float reduce_mul    = 0.5f;
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
        
        settings::AA aa;
    };
}