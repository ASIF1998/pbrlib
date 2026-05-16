#include <pbrlib/exceptions.hpp>

#include <cpptrace/cpptrace.hpp>

#include <format>

namespace pbrlib::exception
{
    Exception::Exception(std::string_view msg)
    {
        _msg = std::format("[pbrlib] {}\n\nstacktrace:\n{}", msg, cpptrace::generate_trace().to_string(true));
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

namespace pbrlib::exception
{
    std::string_view toString(VkFormat format) noexcept
    {
        switch (format)
        {
            case VK_FORMAT_R8_UNORM:
                return "r8_unorm";
            case VK_FORMAT_R8G8_UNORM:
                return "rg8_unorm";
            case VK_FORMAT_R8G8B8_UNORM:
                return "rgb8_unorm";
            case VK_FORMAT_R8G8B8A8_UNORM:
                return "rgba8_unorm";
            case VK_FORMAT_R16_SFLOAT:
                return "r16_sfloat";
            case VK_FORMAT_R16G16B16A16_SFLOAT:
                return "rgba_sfloat";
            case VK_FORMAT_R32G32B32A32_SFLOAT:
                return "rgba32_sfloat";
            default:
                return "unknown_format";
        }

        std::unreachable();
    }

    UndefinedPixelFormat::UndefinedPixelFormat(VkFormat format) :
        Exception(std::format("[undefined-pixel-format] {}", toString(format)))
    { }
}
