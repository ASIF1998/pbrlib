#pragma once

#include <backend/logger/console.hpp>

#include <format>

namespace pbrlib::backend::log
{
    template<typename ...Args>
    void info(const std::format_string<Args...> fmt, Args&& ...args)
    {
        priv::EngineLogger::getLogger().info("{}", std::vformat(fmt.get(), std::make_format_args(args...)));
    }

    template<typename ...Args>
    void warning(const std::format_string<Args...> fmt, Args&& ...args)
    {
        priv::EngineLogger::getLogger().warn("{}", std::vformat(fmt.get(), std::make_format_args(args...)));
    }

    template<typename ...Args>
    void error(const std::format_string<Args...> fmt, Args&& ...args)
    {
        priv::EngineLogger::getLogger().error("{}", std::vformat(fmt.get(), std::make_format_args(args...)));
    }
}
