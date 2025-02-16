#pragma once

#include <backend/utils/private/info.hpp>

namespace pbrlib::utils
{
    [[nodiscard]]
    constexpr uint32_t engineVersion()
    {
        return pbrlib::priv::EngineVersion;
    }
    
    [[nodiscard]]
    constexpr uint32_t vulkanVersion()
    {
        return pbrlib::priv::VulkanVersion;
    }
}