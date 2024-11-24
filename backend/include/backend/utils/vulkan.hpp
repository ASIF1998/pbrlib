#pragma once

#include <stdexcept>

#define VK_CHECK(fn)                                    \
    do {                                                \
        if (auto res = fn; res != VK_SUCCESS)           \
            throw std::runtime_error("[Vulkan] "#fn);   \
    } while (false)
