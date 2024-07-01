#pragma once

#include <spdlog/spdlog.h>

#include <string_view>

#include <memory>

namespace spdlog::sinks
{
#ifdef _WIN32
    using stdout_color_sink_st = wincolor_stdout_sink_st;
#else
    using stdout_color_sink_st = ansicolor_stdout_sink_st;
#endif
}

namespace pbrlib::log::priv
{
    class ConsoleLogger
    {
    public:
        ConsoleLogger() :
            ptr_console_sink (std::make_shared<spdlog::sinks::stdout_color_sink_st>()),
            logger ("", ptr_console_sink)
        {}

        auto& getLog() noexcept
        {
            return logger;
        }

    private:
        std::shared_ptr<spdlog::sinks::stdout_color_sink_st> ptr_console_sink;

        spdlog::logger logger;
    };
}

namespace pbrlib::log::priv
{
    template<typename T>
    class Logger
    {
    public:
        static void init()
        {
            getLogger().set_pattern(T::getPattern());
        }

        static auto& getLogger()
        {
            return logger.getLog();
        }

    private:
        static ConsoleLogger logger;
    };

    class AppLogger : public Logger<AppLogger>
    {
    public:
        static const char* getPattern()
        {
            return "[%H:%M:%S %z] [%^app%$] %v";
        }
    };

    class EngineLogger : public Logger<EngineLogger>
    {
    public:
        static const char* getPattern()
        {
            return "[%H:%M:%S %z] [%^engine%$] %v";
        }
    };

    ConsoleLogger AppLogger::logger;
    ConsoleLogger EngineLogger::logger;
}