#pragma once

#include "../utils.hpp"

#include <concepts>

using MathTestingTypes = ::testing::Types <
    float,
    double,
    int32_t,
    uint32_t
>;