#pragma once

#include <string>
#include <string_view>

#include <exception>
#include <source_location>

namespace pbrlib::exception
{
    class Exception :
        public std::exception
    {
    protected:
        explicit Exception(std::string_view msg, const std::source_location& location);

        const char* what() const noexcept override;

        std::string _msg;
    };

    class InvalidArgument final :
        public Exception
    {
    public:
        explicit InvalidArgument(std::string_view msg, const std::source_location location = std::source_location::current());
    };

    class RuntimeError final :
        public Exception
    {
    public:
        explicit RuntimeError(std::string_view msg, const std::source_location location = std::source_location::current());
    };

    class InitializeError final :
        public Exception
    {
    public:
        explicit InitializeError(std::string_view msg, const std::source_location location = std::source_location::current());
    };

    class InvalidState final :
        public Exception
    {
    public:
        explicit InvalidState(std::string_view msg, const std::source_location location = std::source_location::current());
    };

    class FileOpen final :
        public Exception
    {
    public:
        explicit FileOpen(std::string_view msg, const std::source_location location = std::source_location::current());
    };

    class MathError final :
        public Exception
    {
    public:
        explicit MathError(std::string_view msg, const std::source_location location = std::source_location::current());
    };
}
