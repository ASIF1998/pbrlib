#pragma once

#include <backend/utils/private/info.hpp>

namespace pbrlib::backend::utils
{
    [[nodiscard]]
    constexpr uint32_t engineVersion()
    {
        return backend::priv::EngineVersion;
    }

    [[nodiscard]]
    constexpr uint32_t vulkanVersion()
    {
        return backend::priv::VulkanVersion;
    }
}
