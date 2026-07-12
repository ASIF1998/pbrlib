#include <backend/exceptions.hpp>

#include <format>
#include <utility>

namespace pbrlib::backend::exception
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

    UndefinedPixelFormat::UndefinedPixelFormat(std::string_view msg) :
        Exception(msg)
    { }
}
