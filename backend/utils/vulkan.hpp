#pragma once

#include <pbrlib/exceptions.hpp>

#define VK_CHECK(fn)                                        \
    do {                                                    \
        if (auto res = fn; res != VK_SUCCESS)               \
            throw exception::RuntimeError("[vulkan] "#fn);  \
    } while (false)
