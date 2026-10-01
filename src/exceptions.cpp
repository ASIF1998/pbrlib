#include <pbrlib/exceptions.hpp>

#include <cpptrace/cpptrace.hpp>

#include <format>

namespace pbrlib::exception
{
    Exception::Exception(std::string_view msg, const std::source_location& location)
    {
        const auto loc_info = std::format(
            "-file:     {}\n"
            "-function: {}\n"
            "-line/col: {}:{}",
            location.file_name(),
            location.function_name(),
            location.line(),
            location.column()
        );

        _msg = std::format("[pbrlib] {}\n\nlocation: \n{}\n\nstacktrace:\n{}", msg, loc_info, cpptrace::generate_trace().to_string(true));
    }

    const char* Exception::what() const noexcept
    {
        return _msg.data();
    }
}

namespace pbrlib::exception
{
    InvalidArgument::InvalidArgument(std::string_view msg, const std::source_location location) :
        Exception(std::format("[invalid-argument] {}", msg), location)
    { }
}

namespace pbrlib::exception
{
    RuntimeError::RuntimeError(std::string_view msg, const std::source_location location) :
        Exception(std::format("[runtime-error] {}", msg), location)
    { }
}

namespace pbrlib::exception
{
    InitializeError::InitializeError(std::string_view msg, const std::source_location location) :
        Exception(std::format("[initialize-error] {}", msg), location)
    { }
}

namespace pbrlib::exception
{
    InvalidState::InvalidState(std::string_view msg, const std::source_location location) :
        Exception(std::format("[invalid-state] {}", msg), location)
    { }
}

namespace pbrlib::exception
{
    FileOpen::FileOpen(std::string_view msg, const std::source_location location) :
        Exception(std::format("[file-open] {}", msg), location)
    { }
}

namespace pbrlib::exception
{
    MathError::MathError(std::string_view msg, const std::source_location location) :
        Exception(std::format("[math-error] {}", msg), location)
    { }
}
