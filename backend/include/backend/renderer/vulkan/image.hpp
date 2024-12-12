#pragma once

#include <vulkan/vulkan.hpp>
#include <vma/vk_mem_alloc.h>

#include <pbrlib/math/vec3.hpp>

#include <string>
#include <string_view>

namespace pbrlib::vk
{
    class Device;
}

namespace pbrlib::vk
{
    class Image final
    {
        explicit Image(const Device* ptr_device);

    public:
        class Builder;

    public:
        Image(Image&& image);
        Image(const Image& image) = delete;

        ~Image();

        Image& operator = (Image&& image);
        Image& operator = (const Image& image) = delete;

        VkImage     handle      = VK_NULL_HANDLE;
        VkImageView view_handle = VK_NULL_HANDLE;
        
        uint32_t width  = 0;
        uint32_t height = 0;
        VkFormat format = VK_FORMAT_UNDEFINED;

        uint8_t     level_count = 1;
        uint16_t    layer_count = 1;

    private:
        const Device* _ptr_device;
        VmaAllocation _allocation = VK_NULL_HANDLE;
    };

    class Image::Builder final
    {
        friend class Device;

        void validate();
        VkSharingMode sharingMode();

    public:
        explicit Builder(const Device* ptr_device);

        Builder(Builder&& builder)      = delete;
        Builder(const Builder& builder) = delete;

        Builder& operator = (Builder&& builder)         = delete;
        Builder& operator = (const Builder& builder)    = delete;

        Builder& size(uint32_t width, uint32_t height)  noexcept;
        Builder& format(VkFormat format)                noexcept;
        Builder& usage(VkImageUsageFlags usage)         noexcept;
        Builder& addQueueFamilyIndex(uint32_t index);

        [[maybe_unused]] Builder& filter(VkFilter filter)                           noexcept;
        [[maybe_unused]] Builder& sampleCount(VkSampleCountFlagBits sample_count)   noexcept;
        [[maybe_unused]] Builder& tiling(VkImageTiling tiling)                      noexcept;
        [[maybe_unused]] Builder& fillColor(const pbrlib::math::vec3& fill_color);
        [[maybe_unused]] Builder& name(std::string_view image_name);

        [[nodiscard]] Image build();

    private:
        const Device* _ptr_device;

        uint32_t _width     = 0;
        uint32_t _height    = 0;
        VkFormat _format    = VK_FORMAT_UNDEFINED;

        VkFilter _filter = VK_FILTER_NEAREST;

        pbrlib::math::vec3 _fill_color = pbrlib::math::vec3(0);

        std::vector<uint32_t> _queues;

        VkSampleCountFlagBits   _sample_count   = VK_SAMPLE_COUNT_1_BIT;
        VkImageTiling           _tiling         = VK_IMAGE_TILING_OPTIMAL;
        VkImageUsageFlags       _usage          = VK_IMAGE_USAGE_FLAG_BITS_MAX_ENUM;

        std::string _name;
    };
};