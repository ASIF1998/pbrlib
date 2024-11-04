#pragma once

#include <pbrlib/logger/private/console.hpp>

#include <format>

namespace pbrlib::log::engine
{
    template<typename ...Args>
    void info(const std::string_view fmt, Args&& ...args)
    {
        priv::EngineLogger::getLogger().info("{}", std::vformat(fmt, std::make_format_args(std::forward<Args>(args)...)));
    }

    template<typename ...Args>
    void warning(const std::string_view fmt, Args&& ...args)
    {
        priv::EngineLogger::getLogger().warn("{}", std::vformat(fmt, std::make_format_args(std::forward<Args>(args)...)));
    }
    
    template<typename ...Args>
    void error(const std::string_view fmt, Args&& ...args)
    {
        priv::EngineLogger::getLogger().error("{}", std::vformat(fmt, std::make_format_args(std::forward<Args>(args)...)));
    }
}