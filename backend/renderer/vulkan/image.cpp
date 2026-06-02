#include <backend/renderer/vulkan/image.hpp>
#include <backend/renderer/vulkan/device.hpp>
#include <backend/renderer/vulkan/buffer.hpp>
#include <backend/renderer/vulkan/gpu_marker_colors.hpp>
#include <backend/renderer/vulkan/check.hpp>

#include <backend/exceptions.hpp>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#define STBI_NO_GIF
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_WRITE_NO_STDIO
#include <stb_image.h>
#include <stb_image_write.h>

#include <tinyexr/tinyexr.h>

#include <algorithm>
#include <array>
#include <unordered_set>

#include <fstream>

#include <ranges>

namespace pbrlib::backend::utils
{
    struct ScopeExit final
    {
        explicit ScopeExit(std::function<void()>&& callback) :
            callback(std::move(callback))
        { }

        ScopeExit(const ScopeExit& scope_exit) = delete;

        ~ScopeExit()
        {
            if (callback) [[likely]]
                callback();
        }

        ScopeExit& operator = (const ScopeExit& scope_exit) = delete;

        std::function<void()> callback;
    };

    constexpr uint8_t formatSize(VkFormat format)
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
                throw pbrlib::exception::RuntimeError("[vk-image] undefined pixel format");
        }

        std::unreachable();
    }

    constexpr uint8_t channelCount(VkFormat format)
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
                throw pbrlib::exception::RuntimeError("[vk-image] undefined pixel format");
        }

        std::unreachable();
    }

    constexpr uint8_t channelSize(VkFormat format)
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
                throw exception::UndefinedPixelFormat("[vk-image] undefined pixel format");
        }

        std::unreachable();
    }

    constexpr VkFormat pixelType(int32_t exr_pixel_type, int32_t channel_count)
    {
        if (channel_count == 3 || channel_count == 0 || channel_count > 4) [[unlikely]]
            throw exception::UndefinedPixelFormat("[vk-image] undefined pixel format");

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

        throw exception::UndefinedPixelFormat("[vk-image] undefined pixel format");
    }
}

namespace pbrlib::backend::vk
{
    Image::Image(Device& device) :
        _device (device)
    { }

    Image::Image(Image&& image) noexcept :
        _device     (image._device),
        width       (image.width),
        height      (image.height),
        format      (image.format),
        level_count (image.level_count),
        layer_count (image.layer_count),
        layout      (image.layout)
    {
        std::swap(handle, image.handle);
        std::swap(view_handle, image.view_handle);
    }

    Image& Image::operator = (Image&& image) noexcept
    {
        width   = image.width;
        height  = image.height;
        format  = image.format;

        level_count = image.level_count;
        layer_count = image.layer_count;

        layout = image.layout;

        std::swap(handle, image.handle);
        std::swap(view_handle, image.view_handle);

        return *this;
    }

    void Image::write(const ChunkyImageWriteData& data)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        const auto format_size      = utils::formatSize(data.format);
        const auto scanline_size    = data.width * format_size;
        const auto image_size       = scanline_size * data.height;

        auto staging_buffer = builders::Buffer(_device)
            .name("staging-buffer")
            .size(image_size)
            .usage(VK_BUFFER_USAGE_TRANSFER_SRC_BIT)
            .addQueueFamilyIndex(_device.queue().family_index)
            .type(BufferType::eStaging)
            .build();

        std::span<const uint8_t> image_data (data.ptr_data, image_size);

        staging_buffer.write(image_data, 0);

        auto command_buffer = _device.oneTimeSubmitCommandBuffer("command-buffer-for-copy-buffer-to-image");

        changeLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

        command_buffer.write([&data, &staging_buffer, this] (VkCommandBuffer command_buffer_handle)
        {
            PBRLIB_PROFILING_VK_ZONE_SCOPED(_device, command_buffer_handle, "[vk-image] write-data-in-image");

            const auto aspect = data.format == VK_FORMAT_D32_SFLOAT ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;

            const VkImageSubresourceLayers subresource
            {
                .aspectMask     = static_cast<VkImageAspectFlags>(aspect),
                .mipLevel       = 0,
                .baseArrayLayer = 0,
                .layerCount     = 1
            };

            const VkBufferImageCopy2 region
            {
                .sType              = VK_STRUCTURE_TYPE_BUFFER_IMAGE_COPY_2,
                .bufferOffset       = 0,
                .bufferRowLength    = static_cast<uint32_t>(data.width), // scanline_size
                .bufferImageHeight  = static_cast<uint32_t>(data.height),
                .imageSubresource   = subresource,
                .imageOffset        = { },
                .imageExtent        = {static_cast<uint32_t>(data.width), static_cast<uint32_t>(data.height), 1}
            };

            const VkCopyBufferToImageInfo2 copy_info
            {
                .sType          = VK_STRUCTURE_TYPE_COPY_BUFFER_TO_IMAGE_INFO_2,
                .srcBuffer      = staging_buffer.handle,
                .dstImage       = handle.handle(),
                .dstImageLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                .regionCount    = 1,
                .pRegions       = &region
            };

            vkCmdCopyBufferToImage2(command_buffer_handle, &copy_info);
        }, "[vk-image] write-data-in-image", marker_colors::write_data_in_image);

        _device.submit(command_buffer);
    }

    template<typename PixelChannelTypePrecision>
    void writeToImage(Image& image, const PlanarImageWriteData& src_planar_image)
    {
        const auto pixel_count = src_planar_image.width * src_planar_image.height;

        std::vector<PixelChannelTypePrecision> pixels;
        pixels.reserve(pixel_count * src_planar_image.channel_count);

        const auto src_data = reinterpret_cast<const PixelChannelTypePrecision* const*>(src_planar_image.channels.data());

        for (size_t i = 0; i < pixel_count; ++i)
        {
            for (uint8_t j = 0; j < src_planar_image.channel_count; ++j)
                pixels.push_back(src_data[j][i]);
        }

        const ChunkyImageWriteData write_data
        {
            .ptr_data   = reinterpret_cast<uint8_t*>(pixels.data()),
            .width      = src_planar_image.width,
            .height     = src_planar_image.height,
            .format     = src_planar_image.format
        };

        image.write(write_data);
    }

    void Image::write(const PlanarImageWriteData& data)
    {
        const auto is_fp16 = [] (VkFormat format)
        {
            switch (format)
            {
                case VK_FORMAT_R16_SFLOAT:
                case VK_FORMAT_R16G16_SFLOAT:
                case VK_FORMAT_R16G16B16_SFLOAT:
                case VK_FORMAT_R16G16B16A16_SFLOAT:
                    return true;
                default:
                    return false;
            };

            std::unreachable();
        };

        const auto is_fp32 = [] (VkFormat format)
        {
            switch (format)
            {
                case VK_FORMAT_R32_SFLOAT:
                case VK_FORMAT_R32G32_SFLOAT:
                case VK_FORMAT_R32G32B32_SFLOAT:
                case VK_FORMAT_R32G32B32A32_SFLOAT:
                    return true;
                default:
                    return false;
            };

            std::unreachable();
        };

        if (!is_fp32(data.format) && !is_fp16(data.format)) [[unlikely]]
            throw exception::UndefinedPixelFormat(data.format);

        if (is_fp16(data.format))
            writeToImage<uint16_t>(*this, data);
        else
            writeToImage<uint32_t>(*this, data);
    }

    void Image::changeLayout (
        VkImageLayout           new_layout,
        VkPipelineStageFlags2   src_stage,
        VkPipelineStageFlags2   dst_stage
    )
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        auto command_buffer = _device.oneTimeSubmitCommandBuffer("command-buffer-for-change-image-layout");

        changeLayout(command_buffer, new_layout, src_stage, dst_stage);
        _device.submit(command_buffer);
    }

    void Image::changeLayout (
        CommandBuffer&          command_buffer,
        VkImageLayout           new_layout,
        VkPipelineStageFlags2   src_stage,
        VkPipelineStageFlags2   dst_stage
    )
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        command_buffer.write([this, new_layout, src_stage, dst_stage] (VkCommandBuffer command_buffer_handle)
        {
            PBRLIB_PROFILING_VK_ZONE_SCOPED(_device, command_buffer_handle, "[vk-image] changle-image-layout");

            const auto family_index = _device.queue().family_index;

            const auto aspect_mask =
                    format == VK_FORMAT_D32_SFLOAT
                ?   VK_IMAGE_ASPECT_DEPTH_BIT
                :   VK_IMAGE_ASPECT_COLOR_BIT;

            const VkImageSubresourceRange range
            {
                .aspectMask     = static_cast<VkImageAspectFlags>(aspect_mask),
                .baseMipLevel   = 0,
                .levelCount     = 1,
                .baseArrayLayer = 0,
                .layerCount     = 1
            };

            const VkImageMemoryBarrier2 image_mem_barrier
            {
                .sType                  = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
                .srcStageMask           = src_stage,
                .dstStageMask           = dst_stage,
                .oldLayout              = layout,
                .newLayout              = new_layout,
                .srcQueueFamilyIndex    = family_index,
                .dstQueueFamilyIndex    = family_index,
                .image                  = handle.handle(),
                .subresourceRange       = range
            };

            const VkDependencyInfo dependency_info
            {
                .sType                      = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
                .imageMemoryBarrierCount    = 1,
                .pImageMemoryBarriers       = &image_mem_barrier
            };

            vkCmdPipelineBarrier2(command_buffer_handle, &dependency_info);
        }, "[vk-image] changle-image-layout", vk::marker_colors::change_layout);

        layout = new_layout;
    }
}

namespace pbrlib::backend::vk::builders
{
    Image::Image(Device& device) noexcept :
        _device (device)
    { }

    void Image::validate()
    {
        if (_width == 0 || _height == 0) [[unlikely]]
            throw pbrlib::exception::InvalidState("[vk-image::builder] size is zero");

        if (_format == VK_FORMAT_UNDEFINED) [[unlikely]]
            throw pbrlib::exception::InvalidState("[vk-image::builder] format is undefined");

        if (_queues.empty()) [[unlikely]]
            throw pbrlib::exception::InvalidState("[vk-image::builder] not queues");

        if (_usage == VK_IMAGE_USAGE_FLAG_BITS_MAX_ENUM) [[unlikely]]
            throw pbrlib::exception::InvalidState("[vk-image::builder] invalid usage");
    }

    Image& Image::size(uint32_t width, uint32_t height) noexcept
    {
        _width  = width;
        _height = height;
        return *this;
    }

    Image& Image::format(VkFormat format) noexcept
    {
        _format = format;
        return *this;
    }

    Image& Image::filter(VkFilter filter) noexcept
    {
        _filter = filter;
        return *this;
    }

    Image& Image::fillColor(const pbrlib::math::vec4& fill_color)
    {
        _fill_color = fill_color;
        return *this;
    }

    Image& Image::addQueueFamilyIndex(uint32_t index)
    {
        _queues.push_back(index);
        return *this;
    }

    Image& Image::sampleCount(VkSampleCountFlagBits sample_count) noexcept
    {
        _sample_count = sample_count;
        return *this;
    }

    Image& Image::tiling(VkImageTiling tiling) noexcept
    {
        _tiling = tiling;
        return *this;
    }

    Image& Image::usage(VkImageUsageFlags usage) noexcept
    {
        _usage = usage;
        return *this;
    }

    Image& Image::name(std::string_view image_name)
    {
        _name = image_name;
        return *this;
    }

    VkSharingMode Image::sharingMode()
    {
        std::unordered_set<uint32_t> families (std::begin(_queues), std::end(_queues));
        return families.size() == 1 ? VK_SHARING_MODE_EXCLUSIVE : VK_SHARING_MODE_CONCURRENT;
    }

    vk::Image Image::build()
    {
        validate();

        vk::Image image (_device);

        image.width     = _width;
        image.height    = _height;
        image.format    = _format;

        const VkImageCreateInfo image_info
        {
            .sType                  = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
            .imageType              = VK_IMAGE_TYPE_2D,
            .format                 = _format,
            .extent                 = {_width, _height, 1},
            .mipLevels              = 1,
            .arrayLayers            = 1,
            .samples                = _sample_count,
            .tiling                 = _tiling,
            .usage                  = _usage,
            .sharingMode            = sharingMode(),
            .queueFamilyIndexCount  = static_cast<uint32_t>(_queues.size()),
            .pQueueFamilyIndices    = _queues.data(),
            .initialLayout          = VK_IMAGE_LAYOUT_UNDEFINED
        };

        VmaAllocationCreateInfo alloc_info
        {
            .flags      = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT,
            .usage      = VMA_MEMORY_USAGE_AUTO,
            .priority   = 1.0f
        };

        if (_tiling == VK_IMAGE_TILING_LINEAR)
            alloc_info.flags |= VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |  VMA_ALLOCATION_CREATE_MAPPED_BIT;

        VkImage         image_handle        = VK_NULL_HANDLE;
        VmaAllocation   allocation_handle   = VK_NULL_HANDLE;
        VK_CHECK(vmaCreateImage(
            _device.vmaAllocator(),
            &image_info,
            &alloc_info,
            &image_handle,
            &allocation_handle,
            nullptr
        ));

        image.handle = ImageHandle(image_handle, allocation_handle, true);

        if (_usage & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT)
            image.changeLayout(VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL);
        else if (_usage & VK_IMAGE_USAGE_SAMPLED_BIT)
            image.changeLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        else if (_usage & VK_IMAGE_USAGE_TRANSFER_DST_BIT)
            image.changeLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        else if (_usage & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT)
            image.changeLayout(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);

        const auto aspect = _format == VK_FORMAT_D32_SFLOAT ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;

        const VkImageSubresourceRange subresource_range
        {
            .aspectMask        = static_cast<VkImageAspectFlags>(aspect),
            .baseMipLevel      = 0,
            .levelCount        = image.level_count,
            .baseArrayLayer    = 0,
            .layerCount        = image.layer_count
        };

        constexpr VkComponentMapping components
        {
            .r = VK_COMPONENT_SWIZZLE_IDENTITY,
            .g = VK_COMPONENT_SWIZZLE_IDENTITY,
            .b = VK_COMPONENT_SWIZZLE_IDENTITY,
            .a = VK_COMPONENT_SWIZZLE_IDENTITY
        };

        const VkImageViewCreateInfo image_view_info
        {
            .sType              = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .image              = image.handle.handle(),
            .viewType           = VK_IMAGE_VIEW_TYPE_2D,
            .format             = _format,
            .components         = components,
            .subresourceRange   = subresource_range
        };

        VK_CHECK(vkCreateImageView(
            _device.device(),
            &image_view_info,
            nullptr,
            &image.view_handle.handle()
        ));

        if (!_name.empty()) [[likely]]
        {
            VkDebugUtilsObjectNameInfoEXT name_info
            {
                .sType         = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT,
                .objectType    = VK_OBJECT_TYPE_IMAGE,
                .objectHandle  = reinterpret_cast<uint64_t>(image.handle.handle()),
                .pObjectName   = _name.c_str()
            };

            _device.setName(name_info);

            name_info.objectHandle  = reinterpret_cast<uint64_t>(image.view_handle.handle());
            name_info.objectType    = VK_OBJECT_TYPE_IMAGE_VIEW;

            _device.setName(name_info);
        }

        return image;
    }
}

namespace pbrlib::backend::vk::decoders
{
    Image::Image(Device& device) noexcept :
        _device (device)
    { }

    Image& Image::name(std::string_view image_name)
    {
        _name = image_name;
        return *this;
    }

    Image& Image::channelsPerPixel(int32_t channels_per_pixel)
    {
        _channels_per_pixel = channels_per_pixel;
        return *this;
    }

    Image& Image::compressedImage(const uint8_t* ptr_data, size_t size)
    {
        _compressed_image.ptr_data  = ptr_data;
        _compressed_image.size      = size;

        return *this;
    }

    void Image::validate()
    {
        if (_channels_per_pixel < 1 && _channels_per_pixel > 4) [[unlikely]]
            throw pbrlib::exception::InvalidState("[vk-image::decoder] invalid count channels per pixel");

        if (!_compressed_image.ptr_data || !_compressed_image.size) [[unlikely]]
            throw pbrlib::exception::InvalidState("[vk-image::decoder] compressed image data is empty");
    }

    vk::Image Image::decode()
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        validate();

        constexpr std::array formats
        {
            VK_FORMAT_R8_UNORM,
            VK_FORMAT_R8G8_UNORM,
            VK_FORMAT_R8G8B8_UNORM,
            VK_FORMAT_R8G8B8A8_UNORM
        };

        ChunkyImageWriteData write_data
        {
            .format = formats[_channels_per_pixel - 1]
        };

        stbi_set_flip_vertically_on_load(true);

        write_data.ptr_data = stbi_load_from_memory(
            _compressed_image.ptr_data,
            static_cast<int>(_compressed_image.size),
            &write_data.width, &write_data.height,
            &_channels_per_pixel,
            _channels_per_pixel
        );

        const utils::ScopeExit scope_exit ([ptr_data = write_data.ptr_data]
        {
           stbi_image_free(ptr_data);
        });

        auto image = builders::Image(_device)
            .addQueueFamilyIndex(_device.queue().family_index)
            .format(write_data.format)
            .name(_name)
            .size(static_cast<uint32_t>(write_data.width), static_cast<uint32_t>(write_data.height))
            .usage(VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT)
            .build();

        image.write(write_data);
        image.changeLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        return image;
    }
}

namespace pbrlib::backend::vk::loaders
{
    Image::Image(Device& device) noexcept :
        _device (device)
    { }

    Image& Image::filename(const std::filesystem::path& filename)
    {
        _filename = filename;
        return *this;
    }

    vk::Image stbReader(Device& device, const std::filesystem::path& filename)
    {
        int num_channels = 0;

        ChunkyImageWriteData data
        {
            .format = VK_FORMAT_R8G8B8A8_UNORM
        };

        data.ptr_data = stbi_load(
            filename.string().c_str(),
            &data.width, &data.height,
            &num_channels,
            STBI_rgb_alpha
        );

        utils::ScopeExit scoped_exit ([ptr_data = data.ptr_data]
        {
            stbi_image_free(ptr_data);
        });

        auto image = builders::Image(device)
            .name(filename.filename().string())
            .addQueueFamilyIndex(device.queue().family_index)
            .format(data.format)
            .size(static_cast<uint32_t>(data.width), static_cast<uint32_t>(data.height))
            .usage(VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT)
            .build();

        image.write(data);

        return image;
    }

    vk::Image exrReader(Device& device, const std::filesystem::path& filename)
    {
        const auto path_to_file = filename.string();

        EXRVersion exr_version = { };
        if (ParseEXRVersionFromFile(&exr_version, path_to_file.c_str()) || exr_version.multipart) [[unlikely]]
            throw pbrlib::exception::RuntimeError(std::format("[vk-image::loader] invalid exr file: '{}'", path_to_file));

        EXRHeader exr_header = { };
        InitEXRHeader(&exr_header);
        const utils::ScopeExit exr_header_scope ([&exr_header]
        {
            FreeEXRHeader(&exr_header);
        });

        const char* err = nullptr;

        if (ParseEXRHeaderFromFile(&exr_header, &exr_version, path_to_file.c_str(), &err)) [[unlikely]]
        {
            const auto msg = std::format("[vk-image::loader] failed parse '{}' header: {}", path_to_file, err);
            FreeEXRErrorMessage(err);
            throw pbrlib::exception::RuntimeError(msg);
        }

        EXRImage exr_image = { };
        InitEXRImage(&exr_image);
        const utils::ScopeExit exr_image_scope ([&exr_image]
        {
            FreeEXRImage(&exr_image);
        });

        if (LoadEXRImageFromFile(&exr_image, &exr_header, path_to_file.c_str(), &err)) [[unlikely]]
        {
            const auto msg = std::format("[vk-image::loader] failed load '{}' image: {}", path_to_file, err);
            FreeEXRErrorMessage(err);
            throw pbrlib::exception::RuntimeError(msg);
        }

        std::array<const uint8_t*, 4> channels = { };
        for (size_t i = 0; i < exr_image.num_channels; ++i)
            channels[i] = exr_image.images[exr_image.num_channels - i - 1];

        const auto exr_pixel_type = exr_header.pixel_types[0];
        for (size_t i = 0; i < exr_image.num_channels; ++i)
        {
            if (exr_header.pixel_types[i] != exr_pixel_type)
                throw exception::UndefinedPixelFormat("[vk-image::loader] incorrect pixel type");
        }

        const PlanarImageWriteData data
        {
            .channels       = channels,
            .width          = exr_image.width,
            .height         = exr_image.height,
            .channel_count  = static_cast<uint8_t>(exr_image.num_channels),
            .format         = utils::pixelType(exr_pixel_type, exr_image.num_channels)
        };

        auto image = builders::Image(device)
            .name(filename.filename().string())
            .addQueueFamilyIndex(device.queue().family_index)
            .format(data.format)
            .size(static_cast<uint32_t>(data.width), static_cast<uint32_t>(data.height))
            .usage(VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT)
            .build();

        image.write(data);

        return image;
    }

    vk::Image Image::load()
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        if (!std::filesystem::exists(_filename)) [[unlikely]]
            throw pbrlib::exception::InvalidState(std::format("[vk-image::loader] image not found: {}.", _filename.string()));

        if (_filename.extension() == ".exr")
            return exrReader(_device, _filename);

        return stbReader(_device, _filename);
    }
}

namespace pbrlib::backend::vk::exporters
{
    struct WriteToImageParams final
    {
        void validate() const
        {
            if (data.empty()) [[unlikely]]
                throw pbrlib::exception::InvalidArgument("[vk-image::exporter] image data is empty");

            if (pixel_format == VK_FORMAT_UNDEFINED) [[unlikely]]
                throw pbrlib::exception::InvalidArgument("[vk-image::exporter] undefined pixel format");

            const auto expected_ext = utils::channelSize(pixel_format) > 1 ? ".exr" : ".png";
            if (const auto ext = filename.extension(); ext != expected_ext) [[unlikely]]
                throw pbrlib::exception::InvalidArgument(std::format("[vk-image::exporter] invalid file extension: {}", ext.string()));

            const auto channel_count = utils::channelCount(pixel_format);
            if (!channel_count || channel_count > 4) [[unlikely]]
                throw pbrlib::exception::InvalidArgument(std::format("[vk-image::exporter] channel count: {}", channel_count));

            const auto total_size = channel_count * width * height * utils::channelSize(pixel_format);
            if (total_size != data.size()) [[unlikely]]
                throw pbrlib::exception::InvalidArgument(std::format("[vk-image::exporter] image size:{}x{}, channel count:{}, channel size: {}", width, height, channel_count, utils::channelSize(pixel_format)));
        }

        const std::filesystem::path&    filename;
        std::span<const uint8_t>        data;
        uint32_t                        width           = 0u;
        uint32_t                        height          = 0u;
        VkFormat                        pixel_format    = VK_FORMAT_UNDEFINED;
    };

    using Fp16 = uint16_t;
    using Fp32 = uint32_t;

    Image::Image(Device& device) noexcept :
        _device (device)
    { }

    Image& Image::image(const vk::Image* ptr_image)
    {
        _ptr_image = ptr_image;
        return *this;
    }

    Image& Image::filename(const std::filesystem::path& filename)
    {
        _filename = filename;
        return *this;
    }

    void Image::validate()
    {
        if (!_ptr_image) [[unlikely]]
            throw pbrlib::exception::InvalidState("[vk-image::exporter] image is null");

        if (_ptr_image->layout != VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL) [[unlikely]]
            throw pbrlib::exception::InvalidState("[vk-image::exporter] invalid image layout");

        if (_filename.empty()) [[unlikely]]
            throw pbrlib::exception::InvalidState("[vk-image::exporter] didn't set filename");

        if (std::filesystem::is_directory(_filename)) [[unlikely]]
            throw pbrlib::exception::InvalidState(std::format("[vk-image::exporter] filename is directory: {}", _filename.string()));

        if (const auto save_directory = _filename.parent_path(); !std::filesystem::exists(save_directory)) [[unlikely]]
            std::filesystem::create_directory(save_directory);
    }

    Buffer fetch(Device& device, const vk::Image& image)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        if (image.layout != VK_IMAGE_LAYOUT_GENERAL && image.layout != VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL) [[unlikely]]
            throw pbrlib::exception::InvalidState("[vk-image::exporter] image must be in TRANSFER_SRC_OPTIMAL or GENERAL layout before fetch");

        const auto size = image.width * image.height * utils::formatSize(image.format);
        auto buffer = builders::Buffer(device)
            .addQueueFamilyIndex(device.queue().family_index)
            .name("[vk-image::exporter] fetch buffer")
            .size(size)
            .type(BufferType::eReadback)
            .usage(VK_BUFFER_USAGE_TRANSFER_DST_BIT)
            .build();

        auto command_buffer = device.oneTimeSubmitCommandBuffer("[vk-image::exporter] copy from image to buffer");
        command_buffer.write([&buffer, &image] (const auto command_buffer_handle)
        {
            constexpr VkImageSubresourceLayers color_image_subresource
            {
                .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
                .mipLevel       = 0,
                .baseArrayLayer = 0,
                .layerCount     = 1
            };

            const VkBufferImageCopy2 copy_region
            {
                .sType              = VK_STRUCTURE_TYPE_BUFFER_IMAGE_COPY_2,
                .imageSubresource   = color_image_subresource,
                .imageExtent        = {image.width, image.height, 1u}
            };

            const VkCopyImageToBufferInfo2 copy_image_to_buffer_info
            {
                .sType          = VK_STRUCTURE_TYPE_COPY_IMAGE_TO_BUFFER_INFO_2,
                .srcImage       = image.handle.handle(),
                .srcImageLayout = image.layout,
                .dstBuffer      = buffer.handle.handle(),
                .regionCount    = 1,
                .pRegions       = &copy_region
            };

            vkCmdCopyImageToBuffer2(command_buffer_handle, &copy_image_to_buffer_info);
        });

        device.submit(command_buffer);

        return buffer;
    }

    void writeToPng(const WriteToImageParams& params)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        params.validate();

        std::ofstream file (params.filename, std::ios::out | std::ios::binary);

        if (!file) [[unlikely]]
            throw pbrlib::exception::FileOpen("[vk-image::exporter] failed create writer");

        const auto width            = static_cast<int32_t>(params.width);
        const auto height           = static_cast<int32_t>(params.height);
        const auto channel_count    = static_cast<int>(utils::channelCount(params.pixel_format));
        const auto stride           = channel_count * width;

        stbi_write_png_to_func([](void *context, void *data, int size)
        {
            auto ptr_file = reinterpret_cast<std::ofstream*>(context);
            ptr_file->write(reinterpret_cast<char*>(data), static_cast<std::streamsize>(size));
        }, &file, width, height, channel_count, params.data.data(), stride);
    }

    template<typename FloatPrecision>
    void writeToExr(const WriteToImageParams& params)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        params.validate();

        EXRHeader exr_header = { };
        InitEXRHeader(&exr_header);

        const utils::ScopeExit exr_header_scope ([&exr_header]
        {
            exr_header.channels                 = nullptr;
            exr_header.num_channels             = 0;
            exr_header.pixel_types              = nullptr;
            exr_header.requested_pixel_types    = nullptr;

            FreeEXRHeader(&exr_header);
        });

        exr_header.compression_type = TINYEXR_COMPRESSIONTYPE_ZIP;

        const auto channel_count = utils::channelCount(params.pixel_format);

        EXRImage exr_image = { };
        InitEXRImage(&exr_image);

        const utils::ScopeExit exr_image_scope ([&exr_image]
        {
            exr_image.images = nullptr;

            FreeEXRImage(&exr_image);
        });

        exr_image.num_channels  = static_cast<int32_t>(channel_count);
        exr_image.width         = static_cast<int>(params.width);
        exr_image.height        = static_cast<int>(params.height);

        using Channel = std::vector<FloatPrecision>;
        std::vector<Channel> channels(channel_count, Channel(params.width * params.height));

        const auto src_data = reinterpret_cast<const FloatPrecision*>(params.data.data());
        if (channel_count == 1)
            memcpy(channels[0].data(), src_data, params.width * params.height * sizeof(FloatPrecision));
        else
        {
            for (size_t j = 0; j < channel_count; ++j)
            {
                auto& channel = channels[channel_count - j - 1];
                for (size_t i = 0; i < params.width * params.height; ++i)
                    channel[i] = src_data[i * channel_count + j];
            }
        }

        std::vector<FloatPrecision*> channel_ptrs(channel_count);
        for (auto&& [pointer, channel]: std::views::zip(channel_ptrs, channels))
            pointer = channel.data();

        exr_image.images = reinterpret_cast<uint8_t**>(channel_ptrs.data());

        std::vector<EXRChannelInfo> channel_info (channel_count, EXRChannelInfo { });
        exr_header.num_channels = static_cast<int32_t>(channel_count);
        exr_header.channels     = channel_info.data();

        if (channel_count == 1)
            exr_header.channels[0].name[0] = 'Y';
        else if (channel_count == 2)
        {
            exr_header.channels[0].name[0] = 'A';
            exr_header.channels[1].name[0] = 'Y';
        }
        else if (channel_count == 3)
        {
            exr_header.channels[0].name[0] = 'B';
            exr_header.channels[1].name[0] = 'G';
            exr_header.channels[2].name[0] = 'R';
        }
        else if (channel_count == 4)
        {
            exr_header.channels[0].name[0] = 'A';
            exr_header.channels[1].name[0] = 'B';
            exr_header.channels[2].name[0] = 'G';
            exr_header.channels[3].name[0] = 'R';
        }

        constexpr auto type = std::is_same_v<FloatPrecision, Fp32>
            ?   TINYEXR_PIXELTYPE_FLOAT
            :   TINYEXR_PIXELTYPE_HALF;

        std::vector<int> pixels_buffer (channel_count * 2, type);

        exr_header.pixel_types              = pixels_buffer.data();
        exr_header.requested_pixel_types    = pixels_buffer.data() + channel_count;

        const char* err = nullptr;
        if (SaveEXRImageToFile(&exr_image, &exr_header, params.filename.string().c_str(), &err) != TINYEXR_SUCCESS) [[unlikely]]
        {
            const auto msg = std::format("[vk-image::exporter] failed save '{}': {}", params.filename.string(), err);
            FreeEXRErrorMessage(err);
            throw pbrlib::exception::RuntimeError(msg);
        }
    }

    void Image::save()
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        validate();

        auto readback_buffer = fetch(_device, *_ptr_image);
        readback_buffer.map([this] (std::span<const uint8_t> data)
        {
            const WriteToImageParams write_params
            {
                .filename       = _filename,
                .data           = data,
                .width          = _ptr_image->width,
                .height         = _ptr_image->height,
                .pixel_format   = _ptr_image->format

            };

            switch (_ptr_image->format)
            {
                case VK_FORMAT_R8_UNORM:
                case VK_FORMAT_R8G8_UNORM:
                case VK_FORMAT_R8G8B8_UNORM:
                case VK_FORMAT_R8G8B8A8_UNORM:
                    writeToPng(write_params);
                    break;
                case VK_FORMAT_R16_SFLOAT:
                case VK_FORMAT_R16G16B16A16_SFLOAT:
                    writeToExr<Fp16>(write_params);
                    break;
                case VK_FORMAT_R32G32B32A32_SFLOAT:
                    writeToExr<Fp32>(write_params);
                    break;
                default:
                    throw exception::UndefinedPixelFormat(_ptr_image->format);
            }
        });
    }
}
