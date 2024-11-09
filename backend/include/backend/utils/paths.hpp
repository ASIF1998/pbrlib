#pragma once

#include <backend/utils/private/info.hpp>

namespace pbrlib::utils
{
    [[nodiscard]]
    std::filesystem::path projectRoot()
    {
        return pbrlib::priv::PathToRoot;
    }
}