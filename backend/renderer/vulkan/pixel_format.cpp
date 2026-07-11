#include <backend/renderer/vulkan/pixel_format.hpp>

#include <pbrlib/exceptions.hpp>
#include <backend/exceptions.hpp>

#include <tinyexr/tinyexr.h>

#include <utility>

namespace pbrlib::backend
{
    uint8_t formatSize(VkFormat format)
    {
        switch (format)
        {
            case VK_FORMAT_R8_UNORM:
                return 1;
            case VK_FORMAT_R8G8_UNORM:
            case VK_FORMAT_R16_SFLOAT:
                return 2;
            case VK_FORMAT_R8G8B8_UNORM:
                return 3;
            case VK_FORMAT_R8G8B8A8_UNORM:
                return 4;
            case VK_FORMAT_R16G16B16A16_SFLOAT:
                return 8;
            case VK_FORMAT_R32G32B32A32_SFLOAT:
                return 16;
            default:
                throw pbrlib::exception::RuntimeError("[pixel-format] undefined pixel format");
        }
    }

    uint8_t channelCount(VkFormat format)
    {
        switch (format)
        {
            case VK_FORMAT_R8_UNORM:
            case VK_FORMAT_R16_SFLOAT:
                return 1;
            case VK_FORMAT_R8G8_UNORM:
                return 2;
            case VK_FORMAT_R8G8B8_UNORM:
                return 3;
            case VK_FORMAT_R8G8B8A8_UNORM:
            case VK_FORMAT_R16G16B16A16_SFLOAT:
            case VK_FORMAT_R32G32B32A32_SFLOAT:
                return 4;
            default:
                throw pbrlib::exception::RuntimeError("[pixel-format] undefined pixel format");
        }
    }

    uint8_t channelSize(VkFormat format)
    {
        switch (format)
        {
            case VK_FORMAT_R8_UNORM:
            case VK_FORMAT_R8G8_UNORM:
            case VK_FORMAT_R8G8B8_UNORM:
            case VK_FORMAT_R8G8B8A8_UNORM:
                return 1;
            case VK_FORMAT_R16_SFLOAT:
            case VK_FORMAT_R16G16B16A16_SFLOAT:
                return 2;
            case VK_FORMAT_R32G32B32A32_SFLOAT:
                return 4;
            default:
                throw exception::UndefinedPixelFormat("[pixel-format] undefined pixel format");
        }
    }

    VkFormat pixelType(int32_t exr_pixel_type, int32_t channel_count)
    {
        if (channel_count == 3 || channel_count == 0 || channel_count > 4) [[unlikely]]
            throw exception::UndefinedPixelFormat("[pixel-format] undefined pixel format");

        if (exr_pixel_type == TINYEXR_PIXELTYPE_FLOAT)
        {
            switch (channel_count)
            {
                case 1:
                    return VK_FORMAT_R32_SFLOAT;
                case 2:
                    return VK_FORMAT_R32G32_SFLOAT;
                case 4:
                    return VK_FORMAT_R32G32B32A32_SFLOAT;
            }
        }
        else if (exr_pixel_type == TINYEXR_PIXELTYPE_HALF)
        {
            switch (channel_count)
            {
                case 1:
                    return VK_FORMAT_R16_SFLOAT;
                case 2:
                    return VK_FORMAT_R16G16_SFLOAT;
                case 4:
                    return VK_FORMAT_R16G16B16A16_SFLOAT;
            }
        }

        throw exception::UndefinedPixelFormat("[pixel-format] undefined pixel format");
    }

    ChannelType channelMaxValue(VkFormat format)
    {
        switch(format)
        {
            case VK_FORMAT_R8_UNORM:
            case VK_FORMAT_R8G8_UNORM:
            case VK_FORMAT_R8G8B8_UNORM:
            case VK_FORMAT_R8G8B8A8_UNORM:
                return 1.0f;
            case VK_FORMAT_R16_SFLOAT:
            case VK_FORMAT_R16G16B16A16_SFLOAT:
                return 65504.0f;
            case VK_FORMAT_R32G32B32A32_SFLOAT:
                return std::numeric_limits<float>::max();
            default:
                throw pbrlib::exception::RuntimeError("[pixel-format] undefined pixel format");
        }
    }
}