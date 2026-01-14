#pragma once

#include <string>
#include <string_view>

#include <exception>

#include <stacktrace>

namespace pbrlib::exception
{
    class Exception :
        public std::exception
    {
    protected:
        explicit Exception(std::string_view msg, std::stacktrace trace = std::stacktrace::current());

        const char* what() const noexcept override;

        std::string _msg;
    };

    class InvalidArgument final :
        public Exception
    {
    public:
        explicit InvalidArgument(std::string_view msg);
    };

    class RuntimeError final :
        public Exception
    {
    public:
        explicit RuntimeError(std::string_view msg);
    };

    class InitializeError final :
        public Exception
    {
    public:
        explicit InitializeError(std::string_view msg);
    };

    class InvalidState final :
        public Exception
    {
    public:
        explicit InvalidState(std::string_view msg);
    };

    class FileOpen final :
        public Exception
    {
    public:
        explicit FileOpen(std::string_view msg);
    };

    class MathError final :
        public Exception
    {
    public:
        explicit MathError(std::string_view msg);
    };
}
