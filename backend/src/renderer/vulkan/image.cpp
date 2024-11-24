#include <backend/renderer/vulkan/device.hpp>
#include <backend/renderer/vulkan/image.hpp>
#include <backend/utils/vulkan.hpp>

#include <stdexcept>

#include <algorithm>
#include <unordered_set>

namespace pbrlib::vk
{
    Image::Image(const Device* ptr_device) :
        _ptr_device(ptr_device)
    { }

    Image::Image(Image&& image) :
        _ptr_device (image._ptr_device),
        width       (image.width),
        height      (image.height),
        format      (image.format),
        level_count (image.level_count),
        layer_count (image.layer_count)
    {
        std::swap(handle, image.handle);
        std::swap(view_handle, image.view_handle);
        std::swap(_allocation, image._allocation);
    }

    Image::~Image()
    {
        if (view_handle != VK_NULL_HANDLE)
            vkDestroyImageView(_ptr_device->device(), view_handle, nullptr);

        if (handle != VK_NULL_HANDLE)
            vmaDestroyImage(_ptr_device->vmaAllocator(), handle, _allocation);
    }

    Image& Image::operator = (Image&& image)
    {
        _ptr_device = image._ptr_device;

        width   = image.width;
        height  = image.height;
        format  = image.format;

        level_count = image.level_count;
        layer_count = image.layer_count;

        std::swap(handle, image.handle);
        std::swap(view_handle, image.view_handle);
        std::swap(_allocation, image._allocation);

        return *this;
    }
}

namespace pbrlib::vk
{
    Image::Builder::Builder(const Device* ptr_device) :
        _ptr_device (ptr_device)
    { }

    void Image::Builder::validate()
    {
        if (!_ptr_device)
            throw std::runtime_error("[Vulkan::Image::Builder] Not device");

        if (_width == 0 || _height == 0)
            throw std::runtime_error("[Vulkan::Image::Builder] Image size is zero");

        if (_format == VK_FORMAT_UNDEFINED)
            throw std::runtime_error("[Vulkan::Image::Builder] Image format is undefined");

        if (_queues.empty())
            throw std::runtime_error("[Vulkan::Image::Builder] Not queues");

        if (_usage == VK_IMAGE_USAGE_FLAG_BITS_MAX_ENUM)
            throw std::runtime_error("[Vulkan::Image::Builder] Invalid usage");
    }

    Image::Builder& Image::Builder::size(uint32_t width, uint32_t height) noexcept
    {
        _width  = width;
        _height = height;
        return *this;
    }

    Image::Builder& Image::Builder::format(VkFormat format) noexcept
    {
        _format = format;
        return *this;
    }

    Image::Builder& Image::Builder::filter(VkFilter filter) noexcept
    {
        _filter = filter;
        return *this;
    }

    Image::Builder& Image::Builder::fillColor(const pbrlib::math::vec3& fill_color)
    {
        _fill_color = fill_color;
        return *this;
    }

    Image::Builder& Image::Builder::addQueueFamilyIndex(uint32_t index)
    {
        _queues.push_back(index);
        return *this;
    }

    Image::Builder& Image::Builder::sampleCount(VkSampleCountFlagBits sample_count) noexcept
    {
        _sample_count = sample_count;
        return *this;
    }

    Image::Builder& Image::Builder::tiling(VkImageTiling tiling) noexcept
    {
        _tiling = tiling;
        return *this;
    }
    
    Image::Builder& Image::Builder::usage(VkImageUsageFlags usage) noexcept
    {
        _usage = usage;
        return *this;
    }

    Image::Builder& Image::Builder::name(std::string_view image_name)
    {
        _name = image_name;
        return *this;
    }

    VkSharingMode Image::Builder::sharingMode()
    {
        std::unordered_set<uint32_t> families (std::begin(_queues), std::end(_queues));
        return families.size() == 1 ? VK_SHARING_MODE_EXCLUSIVE : VK_SHARING_MODE_CONCURRENT;
    }

    Image Image::Builder::build()
    {
        validate();

        Image image (_ptr_device);

        image.width     = _width;
        image.height    = _height;
        image.format    = _format;

        VkImageCreateInfo image_info = { };
        image_info.sType                    = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        image_info.arrayLayers              = 1;
        image_info.mipLevels                = 1;
        image_info.extent                   = {_width, _height, 1};
        image_info.format                   = _format;
        image_info.imageType                = VK_IMAGE_TYPE_2D;
        image_info.initialLayout            = VK_IMAGE_LAYOUT_UNDEFINED;
        image_info.pQueueFamilyIndices      = _queues.data();
        image_info.queueFamilyIndexCount    = static_cast<uint32_t>(_queues.size());
        image_info.samples                  = _sample_count;
        image_info.tiling                   = _tiling;
        image_info.usage                    = _usage;
        image_info.sharingMode              = sharingMode();

        VmaAllocationCreateInfo alloc_info = { };
        alloc_info.usage    = VMA_MEMORY_USAGE_AUTO;
        alloc_info.flags    = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;
        alloc_info.priority = 1.0f;

        VK_CHECK(vmaCreateImage(
            _ptr_device->vmaAllocator(), 
            &image_info, 
            &alloc_info, 
            &image.handle, 
            &image._allocation, 
            nullptr
        ));

        VkImageSubresourceRange subresource_range = { };
        subresource_range.aspectMask        = VK_IMAGE_ASPECT_COLOR_BIT;
        subresource_range.baseArrayLayer    = 0;
        subresource_range.baseMipLevel      = 0;
        subresource_range.layerCount        = image.layer_count;
        subresource_range.levelCount        = image.level_count;

        VkComponentMapping components = { };
        components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        VkImageViewCreateInfo image_view_info = { };
        image_view_info.sType               = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        image_view_info.components          = components;
        image_view_info.format              = _format;
        image_view_info.image               = image.handle;
        image_view_info.subresourceRange    = subresource_range;
        image_view_info.viewType            = VK_IMAGE_VIEW_TYPE_2D;

        VK_CHECK(vkCreateImageView(
            _ptr_device->device(),
            &image_view_info,
            nullptr,
            &image.view_handle
        ));

        if (!_name.empty())
        {
            VkDebugUtilsObjectNameInfoEXT name_info = { };
            name_info.sType         = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
            name_info.objectHandle  = reinterpret_cast<uint64_t>(image.handle);
            name_info.pObjectName   = _name.c_str();
            name_info.objectType    = VK_OBJECT_TYPE_IMAGE;

            _ptr_device->setName(name_info);

            name_info.objectHandle  = reinterpret_cast<uint64_t>(image.view_handle);
            name_info.objectType    = VK_OBJECT_TYPE_IMAGE_VIEW;

            _ptr_device->setName(name_info);
        }

        return image;
    }
}