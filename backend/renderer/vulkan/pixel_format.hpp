#pragma once

#include <vulkan/vulkan.h>
#include <variant>

namespace pbrlib::backend
{
    using ChannelType = std::variant<uint8_t, uint16_t, uint32_t, float>;    
}

namespace pbrlib::backend
{
    [[nodiscard]] uint8_t   formatSize(VkFormat format);
    [[nodiscard]] VkFormat  pixelType(int32_t exr_pixel_type, int32_t channel_count);

    [[nodiscard]] uint8_t channelCount(VkFormat format);
    [[nodiscard]] uint8_t channelSize(VkFormat format);

    [[nodiscard]] ChannelType channelMaxValue(VkFormat format);
}
