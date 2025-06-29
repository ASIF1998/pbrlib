#pragma once

#include <pbrlib/math/vec4.hpp>

#include <vulkan/vulkan.hpp>
#include <vma/vk_mem_alloc.h>

#include <string>
#include <string_view>

#include <filesystem>

namespace pbrlib::backend::vk
{
    class Device;
    class CommandBuffer;
}

namespace pbrlib::backend::vk::builders
{
    class Image;
}

namespace pbrlib::backend::vk::decoders
{
    class Image;
}

namespace pbrlib::backend::vk
{
    struct ImageWriteData final
    {
        uint8_t*        ptr_data    = nullptr;
        int             width       = 0;
        int             height      = 0;
        VkFormat        format      = VK_FORMAT_UNDEFINED;
    };

    class Image final
    {
        friend class Surface;
        friend class builders::Image;
        friend class decoders::Image;

        explicit Image(Device& device, bool from_swapchain = false);

    public:
        class Loader;
        class Exporter;

    public:
        Image(Image&& image);
        Image(const Image& image) = delete;

        ~Image();

        Image& operator = (Image&& image);
        Image& operator = (const Image& image) = delete;

        void write(const ImageWriteData& data);

        void changeLayout (
            VkImageLayout           new_layout,
            VkPipelineStageFlags2   src_stage = VK_PIPELINE_STAGE_2_NONE,
            VkPipelineStageFlags2   dst_stage = VK_PIPELINE_STAGE_2_NONE
        );

        void changeLayout (
            CommandBuffer&          command_buffer,
            VkImageLayout           new_layout,
            VkPipelineStageFlags2   src_stage = VK_PIPELINE_STAGE_2_NONE,
            VkPipelineStageFlags2   dst_stage = VK_PIPELINE_STAGE_2_NONE
        );

        VkImage     handle      = VK_NULL_HANDLE;
        VkImageView view_handle = VK_NULL_HANDLE;
        
        uint32_t width  = 0;
        uint32_t height = 0;
        VkFormat format = VK_FORMAT_UNDEFINED;

        uint8_t     level_count = 1;
        uint16_t    layer_count = 1;

        VkImageLayout layout = VK_IMAGE_LAYOUT_UNDEFINED;

    private:
        Device&         _device;
        VmaAllocation   _allocation = VK_NULL_HANDLE;

        bool _from_swapchain;
    };

    class Image::Loader final
    {
        void validate();

    public:
        explicit Loader(Device& device) noexcept;

        Loader(Loader&& loader)         = delete;
        Loader(const Loader& loader)    = delete;

        Loader& operator = (Loader&& loader)        = delete;
        Loader& operator = (const Loader& loader)   = delete;

        Loader& filename(const std::filesystem::path& filename);

        [[nodiscard]] Image load();

    private:
        Device& _device;

        std::filesystem::path _filename;
    };

    class Image::Exporter final
    {
        void validate();

    public:
        explicit Exporter(Device& device) noexcept;

        Exporter(Exporter&& exporter)       = delete;
        Exporter(const Exporter& exporter)  = delete;

        Exporter& operator = (Exporter&& exporter)      = delete;
        Exporter& operator = (const Exporter& exporter) = delete;

        Exporter& image(const Image* ptr_image);
        Exporter& filename(const std::filesystem::path& filename);

        [[nodiscard]] void exoprt();

    private:
        Device& _device;

        const Image* _ptr_image = nullptr;

        std::filesystem::path _filename;
    };
}

namespace pbrlib::backend::vk::builders
{
    class Image final
    {
        friend class Device;

        void validate();
        
        [[nodiscard]] VkSharingMode sharingMode();

    public:
        explicit Image(Device& device) noexcept;

        Image(Image&& builder)      = delete;
        Image(const Image& builder) = delete;

        Image& operator = (Image&& builder)         = delete;
        Image& operator = (const Image& builder)    = delete;

        Image& size(uint32_t width, uint32_t height)    noexcept;
        Image& format(VkFormat format)                  noexcept;
        Image& usage(VkImageUsageFlags usage)           noexcept;
        Image& addQueueFamilyIndex(uint32_t index);

        [[maybe_unused]] Image& filter(VkFilter filter)                             noexcept;
        [[maybe_unused]] Image& sampleCount(VkSampleCountFlagBits sample_count)     noexcept;
        [[maybe_unused]] Image& tiling(VkImageTiling tiling)                        noexcept;
        [[maybe_unused]] Image& fillColor(const pbrlib::math::vec4& fill_color);
        [[maybe_unused]] Image& name(std::string_view image_name);

        [[nodiscard]] vk::Image build();

    private:
        Device& _device;

        uint32_t _width     = 0;
        uint32_t _height    = 0;
        VkFormat _format    = VK_FORMAT_UNDEFINED;

        VkFilter _filter = VK_FILTER_NEAREST;

        pbrlib::math::vec4 _fill_color;

        std::vector<uint32_t> _queues;

        VkSampleCountFlagBits   _sample_count   = VK_SAMPLE_COUNT_1_BIT;
        VkImageTiling           _tiling         = VK_IMAGE_TILING_OPTIMAL;
        VkImageUsageFlags       _usage          = VK_IMAGE_USAGE_FLAG_BITS_MAX_ENUM;

        std::string _name;
    };
}

namespace pbrlib::backend::vk::decoders
{
    class Image final
    {
        void validate();

    public:
        explicit Image(Device& device);

        Image(Image&& decoder)      = delete;
        Image(const Image& decoder) = delete;

        Image& operator = (Image&& decoder)         = delete;
        Image& operator = (const Image& decoder)    = delete;

        Image& name(std::string_view image_name);
        Image& channelsPerPixel(int32_t channels_per_pixel);
        Image& compressedImage(const uint8_t* ptr_data, size_t size);

        [[nodiscard]] vk::Image decode();

    private:
        Device& _device;

        std::string _name;

        int32_t _channels_per_pixel = 4;

        struct
        {
            const uint8_t* ptr_data = nullptr;
            size_t size             = 0;
        } _compressed_image;
    };
}