#include <pbrlib/exceptions.hpp>

#include <format>

namespace pbrlib::exception
{
    Exception::Exception(std::string_view msg /*, std::stacktrace trace*/)
    {
        // _msg = std::format("[pbrlib] {}\n\nstacktrace:\n{}", msg, std::to_string(trace));
    }

    const char* Exception::what() const noexcept
    {
        return _msg.data();
    }
}

namespace pbrlib::exception
{
    InvalidArgument::InvalidArgument(std::string_view msg) :
        Exception(std::format("[invalid-argument] {}", msg))
    { }
}

namespace pbrlib::exception
{
    RuntimeError::RuntimeError(std::string_view msg) :
        Exception(std::format("[runtime-error] {}", msg))
    { }
}

namespace pbrlib::exception
{
    InitializeError::InitializeError(std::string_view msg) :
        Exception(std::format("[initialize-error] {}", msg))
    { }
}

namespace pbrlib::exception
{
    InvalidState::InvalidState(std::string_view msg) :
        Exception(std::format("[invalid-state] {}", msg))
    { }
}

namespace pbrlib::exception
{
    FileOpen::FileOpen(std::string_view msg) :
        Exception(std::format("[file-open] {}", msg))
    { }
}

namespace pbrlib::exception
{
    MathError::MathError(std::string_view msg) :
        Exception(std::format("[math-error] {}", msg))
    { }
}
