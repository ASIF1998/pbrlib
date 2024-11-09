#pragma once

#include <backend/utils/private/info.hpp>

namespace pbrlib::utils
{
    [[nodiscard]]
    uint32_t engineVersion()
    {
        return pbrlib::priv::EngineVersion;
    }
    
    [[nodiscard]]
    uint32_t vulkanVersion()
    {
        return pbrlib::priv::VulkanVersion;
    }
}