#pragma once

#include <backend/utils/private/info.hpp>

namespace pbrlib::utils
{
    [[nodiscard]]
    inline std::filesystem::path projectRoot()
    {
        return pbrlib::priv::PathToRoot;
    }
}