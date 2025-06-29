#include <backend/renderer/vulkan/device.hpp>
#include <backend/renderer/vulkan/image.hpp>
#include <backend/renderer/vulkan/buffer.hpp>
#include <backend/renderer/vulkan/gpu_marker_colors.hpp>
#include <backend/utils/vulkan.hpp>

#include <backend/profiling.hpp>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#define STBI_NO_GIF
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_WRITE_NO_STDIO
#include <stb_image.h>
#include <stb_image_write.h>

#include <stdexcept>

#include <algorithm>
#include <unordered_set>

#include <fstream>

namespace pbrlib::backend::utils
{
    constexpr uint8_t formatSize(VkFormat format)
    {
        switch (format)
        {
            case VK_FORMAT_R8_UNORM:
                return 1;
            case VK_FORMAT_R8G8_UNORM:
                return 2;
            case VK_FORMAT_R8G8B8_UNORM:
                return 3;
            case VK_FORMAT_R8G8B8A8_UNORM:
                return 4;
            case VK_FORMAT_R32G32B32A32_SFLOAT:
                return 16;
            default:
                throw exception::RuntimeError("[vk-image] undefined pixel format");
        }

        return std::numeric_limits<uint8_t>::max();
    }
}

namespace pbrlib::backend::vk
{
    Image::Image(Device& device, bool from_swapchain) :
        _device         (device),
        _from_swapchain (from_swapchain)
    { }

    Image::Image(Image&& image) :
        _device     (image._device),
        width       (image.width),
        height      (image.height),
        format      (image.format),
        level_count (image.level_count),
        layer_count (image.layer_count)
    {
        std::swap(handle, image.handle);
        std::swap(view_handle, image.view_handle);
        std::swap(_allocation, image._allocation);
        std::swap(_from_swapchain, image._from_swapchain);
    }

    Image::~Image()
    {
        vkDestroyImageView(_device.device(), view_handle, nullptr);

        if (!_from_swapchain)
            vmaDestroyImage(_device.vmaAllocator(), handle, _allocation);
    }

    Image& Image::operator = (Image&& image)
    {
        width   = image.width;
        height  = image.height;
        format  = image.format;

        level_count = image.level_count;
        layer_count = image.layer_count;

        std::swap(handle, image.handle);
        std::swap(view_handle, image.view_handle);
        std::swap(_allocation, image._allocation);
        std::swap(_from_swapchain, image._from_swapchain);

        return *this;
    }

    void Image::write(const ImageWriteData& data)
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
                .dstImage       = handle,
                .dstImageLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                .regionCount    = 1,
                .pRegions       = &region
            };

            vkCmdCopyBufferToImage2(command_buffer_handle, &copy_info);
        }, "[vk-image] write-data-in-image", marker_colors::write_data_in_image);

        _device.submit(command_buffer);
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
                .image                  = handle,
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
        if (_width == 0 || _height == 0)
            throw exception::InvalidState("[vk-image::builder] size is zero");

        if (_format == VK_FORMAT_UNDEFINED)
            throw exception::InvalidState("[vk-image::builder] format is undefined");

        if (_queues.empty())
            throw exception::InvalidState("[vk-image::builder] not queues");

        if (_usage == VK_IMAGE_USAGE_FLAG_BITS_MAX_ENUM)
            throw exception::InvalidState("[vk-image::builder] invalid usage");
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

        VK_CHECK(vmaCreateImage(
            _device.vmaAllocator(), 
            &image_info, 
            &alloc_info, 
            &image.handle, 
            &image._allocation, 
            nullptr
        ));

        if (_usage & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT)
            image.changeLayout(VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL);
            
        if (_usage & VK_IMAGE_USAGE_TRANSFER_DST_BIT)
            image.changeLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
            
        if (_usage & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT)
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
            .image              = image.handle,
            .viewType           = VK_IMAGE_VIEW_TYPE_2D,
            .format             = _format,
            .components         = components,
            .subresourceRange   = subresource_range
        };

        VK_CHECK(vkCreateImageView(
            _device.device(),
            &image_view_info,
            nullptr,
            &image.view_handle
        ));

        if (!_name.empty())
        {
            VkDebugUtilsObjectNameInfoEXT name_info 
            { 
                .sType         = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT,
                .objectType    = VK_OBJECT_TYPE_IMAGE,
                .objectHandle  = reinterpret_cast<uint64_t>(image.handle),
                .pObjectName   = _name.c_str()
            };

            _device.setName(name_info);

            name_info.objectHandle  = reinterpret_cast<uint64_t>(image.view_handle);
            name_info.objectType    = VK_OBJECT_TYPE_IMAGE_VIEW;

            _device.setName(name_info);
        }

        return image;
    }
}

namespace pbrlib::backend::vk::decoders
{
    Image::Image(Device& device) :
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
        if (_channels_per_pixel < 1 && _channels_per_pixel > 4) 
            throw exception::InvalidState("[vk-image::decoder] invalid count channels per pixel");

        if (!_compressed_image.ptr_data || !_compressed_image.size)
            throw exception::InvalidState("[vk-image::decoder] compressed image data is empty");
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

        ImageWriteData write_data 
        {
            .format = formats[_channels_per_pixel - 1]
        };

        write_data.ptr_data = stbi_load_from_memory(
            _compressed_image.ptr_data, 
            static_cast<int>(_compressed_image.size), 
            &write_data.width, &write_data.height,
            &_channels_per_pixel,
            _channels_per_pixel
        );

        auto image = builders::Image(_device)
            .addQueueFamilyIndex(_device.queue().family_index)
            .format(write_data.format)
            .name(_name)
            .size(static_cast<uint32_t>(write_data.width), static_cast<uint32_t>(write_data.height))
            .usage(VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT)
            .build();

        image.write(write_data);

        stbi_image_free(write_data.ptr_data);

        image.changeLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        return image;
    }
}

namespace pbrlib::backend::vk
{
    Image::Loader::Loader(Device& device) noexcept :
        _device (device)
    { }

    Image::Loader& Image::Loader::filename(const std::filesystem::path& filename)
    {
        _filename = filename;
        return *this;
    }

    void Image::Loader::validate()
    {
        if (!std::filesystem::exists(_filename))
            throw exception::InvalidState(std::format("[vk-image::loader] image not found: {}.", _filename.string()));
    }

    Image Image::Loader::load()
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        validate();

        int num_channels = 0;
        
        ImageWriteData data
        {
            .format = VK_FORMAT_R8G8B8A8_UNORM
        };
        
        data.ptr_data = stbi_load(
            _filename.string().c_str(), 
            &data.width, &data.height, 
            &num_channels, 
            STBI_rgb_alpha
        );

        if (data.width < 1 || data.height < 1 || !data.ptr_data)
            throw exception::RuntimeError(std::format("[image-loader] failed load image '{}'", _filename.string()));

        auto image = builders::Image(_device)
            .name(_filename.filename().string())
            .addQueueFamilyIndex(_device.queue().family_index)
            .format(data.format)
            .size(static_cast<uint32_t>(data.width), static_cast<uint32_t>(data.height))
            .usage(VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT)
            .build();

        image.write(data);

        stbi_image_free(data.ptr_data);

        return image;
    }
}

namespace pbrlib::backend::vk
{
    Image::Exporter::Exporter(Device& device) noexcept :
        _device (device)
    { }

    Image::Exporter& Image::Exporter::image(const Image* ptr_image)
    {
        _ptr_image = ptr_image;
        return *this;
    }

    Image::Exporter& Image::Exporter::filename(const std::filesystem::path& filename)
    {
        _filename = filename;
        return *this;
    }

    void Image::Exporter::validate()
    {
        if (!_ptr_image)
            throw exception::InvalidState("[vk-image::exporter] image is null");

        if (_ptr_image->layout != VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL)
            throw exception::InvalidState("[vk-image::exporter] invalid image layout");

        if (_filename.empty())
            throw exception::InvalidState("[vk-image::exporter] didn't set filename");
    }

    void Image::Exporter::exoprt()
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        validate();

        auto width  = static_cast<int>(_ptr_image->width);
        auto height = static_cast<int>(_ptr_image->height);
        
        auto image = builders::Image(_device)
            .size(_ptr_image->width, _ptr_image->height)
            .format(VK_FORMAT_R8G8B8A8_UNORM)
            .usage(VK_IMAGE_USAGE_TRANSFER_DST_BIT)
            .addQueueFamilyIndex(_device.queue().family_index)
            .tiling(VK_IMAGE_TILING_LINEAR)
            .fillColor(pbrlib::math::vec3(std::numeric_limits<float>::max()))
            .name("[vk-image::exporter] src-image")
            .build();
        
        image.changeLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

        auto command_buffer = _device.oneTimeSubmitCommandBuffer();
        command_buffer.write([&image, this] (auto command_buffer_handle)
        {
            PBRLIB_PROFILING_VK_ZONE_SCOPED(_device, command_buffer_handle, "[vk-image::exoprter] blit-src-image-to-save");

            const VkOffset3D blit_size
            {
                .x = static_cast<int32_t>(image.width),
                .y = static_cast<int32_t>(image.height),
                .z = 1
            };

            const VkImageBlit2 region
            {
                .sType = VK_STRUCTURE_TYPE_IMAGE_BLIT_2,
                .srcSubresource = 
                {
                    .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                    .layerCount = 1
                },
                .srcOffsets = {{}, blit_size},
                .dstSubresource = 
                {
                    .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                    .layerCount = 1
                },
                .dstOffsets = {{}, blit_size}
            };

            const VkBlitImageInfo2 blit_info 
            { 
                .sType          = VK_STRUCTURE_TYPE_BLIT_IMAGE_INFO_2,
                .srcImage       = _ptr_image->handle,
                .srcImageLayout = _ptr_image->layout,
                .dstImage       = image.handle,
                .dstImageLayout = image.layout,
                .regionCount    = 1,
                .pRegions       = &region,
                .filter         = VK_FILTER_NEAREST
            };

            vkCmdBlitImage2(command_buffer_handle, &blit_info);
        }, "[vk-image::exoprter] blit-src-image-to-save", marker_colors::blit_image);

        _device.submit(command_buffer);

        const VkImageSubresource sub_resource 
        {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .mipLevel   = 0,
            .arrayLayer = 0
        };

        VkSubresourceLayout sub_resource_layout = { };

        vkGetImageSubresourceLayout(
            _device.device(),
            image.handle,
            &sub_resource, 
            &sub_resource_layout
        );

        uint8_t* ptr_data = nullptr;
        VK_CHECK(vmaMapMemory(
            _device.vmaAllocator(),
            image._allocation,
            reinterpret_cast<void**>(&ptr_data)
        ));

        std::ofstream file (_filename, std::ios::out | std::ios::binary);

        if (!file)
            throw exception::FileOpen("[vk-image::exporter] failed create writer");

        std::function<void (void*, int)> writer = [this, &file] (void* data, int size)
        {
            file.write(reinterpret_cast<char*>(data), size);
        };

        stbi_write_png_to_func([](void *context, void *data, int size)
        { 
            (*reinterpret_cast<std::function<void (void*, int)>*>(context))(data, size);
        }, &writer, width, height, 4, ptr_data + sub_resource_layout.offset, static_cast<int>(sub_resource_layout.rowPitch));

        vmaUnmapMemory(_device.vmaAllocator(), image._allocation);
    }
}