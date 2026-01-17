#pragma once

#include <backend/utils/private/info.hpp>

namespace pbrlib::backend::utils
{
    [[nodiscard]]
    inline std::filesystem::path projectRoot()
    {
        return priv::PathToRoot;
    }
}
