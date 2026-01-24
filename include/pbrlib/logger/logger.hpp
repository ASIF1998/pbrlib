#pragma once

#include <backend/logger/console.hpp>

#include <format>

namespace pbrlib::log
{
    template<typename ...Args>
    void info(const std::string_view fmt, Args&& ...args)
    {
        backend::log::priv::AppLogger::getLogger().info("{}", std::vformat(fmt, std::make_format_args(std::forward<Args>(args)...)));
    }

    template<typename ...Args>
    void warning(const std::string_view fmt, Args&& ...args)
    {
        backend::log::priv::AppLogger::getLogger().warn("{}", std::vformat(fmt, std::make_format_args(std::forward<Args>(args)...)));
    }
    
    template<typename ...Args>
    void error(const std::string_view fmt, Args&& ...args)
    {
        backend::log::priv::AppLogger::getLogger().error("{}", std::vformat(fmt, std::make_format_args(std::forward<Args>(args)...)));
    }
}