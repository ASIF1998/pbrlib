    #pragma once

    #include <vulkan/vulkan.h>

    namespace pbrlib::backend
    {
        [[nodiscard]] uint8_t formatSize(VkFormat format);
        [[nodiscard]] uint8_t channelCount(VkFormat format);
        [[nodiscard]] uint8_t channelSize(VkFormat format);
        [[nodiscard]] VkFormat pixelType(int32_t exr_pixel_type, int32_t channel_count);
    }