#pragma once

#include <pbrlib/exceptions.hpp>

#include <string>
#include <format>

#include <fstream>
#include <sstream>

#include <filesystem>

namespace pbrlib::backend::vk::shader::utils
{
    inline std::string getSource(const std::filesystem::path& filename)
    {
        if (!std::filesystem::exists(filename)) [[unlikely]]
            throw exception::InvalidState(std::format("[shader-compiler] not find file: {}", filename.string()));

        std::ifstream file(filename);

        if (!file) [[unlikely]]
            throw exception::FileOpen(std::format("[shader-compiler] {}", filename.string()));

        std::ostringstream contents;
        contents << file.rdbuf();

        return contents.str();
    }
}
