#include <pbrlib/window.hpp>

#include <backend/renderer/vulkan/device.hpp>
#include <backend/renderer/vulkan/surface.hpp>
#include <backend/utils/vulkan.hpp>

#include <backend/logger/logger.hpp>

#include <SDL3/SDL_vulkan.h>

#include <ranges>

namespace pbrlib::backend::vk
{
    bool formatIsRgb(VkFormat format)
    {
        switch(format)
        {
        case VK_FORMAT_R8G8B8_UNORM:
        case VK_FORMAT_R8G8B8_SNORM:
        case VK_FORMAT_R8G8B8_USCALED:
        case VK_FORMAT_R8G8B8_SSCALED:
        case VK_FORMAT_R8G8B8_UINT:
        case VK_FORMAT_R8G8B8_SINT:
        case VK_FORMAT_R8G8B8_SRGB:
        case VK_FORMAT_R16G16B16_UNORM:
        case VK_FORMAT_R16G16B16_SNORM:
        case VK_FORMAT_R16G16B16_USCALED:
        case VK_FORMAT_R16G16B16_SSCALED:
        case VK_FORMAT_R16G16B16_UINT:
        case VK_FORMAT_R16G16B16_SINT:
        case VK_FORMAT_R16G16B16_SFLOAT:
        case VK_FORMAT_R32G32B32_UINT:
        case VK_FORMAT_R32G32B32_SINT:
        case VK_FORMAT_R32G32B32_SFLOAT:
        case VK_FORMAT_R64G64B64_UINT:
        case VK_FORMAT_R64G64B64_SINT:
        case VK_FORMAT_R64G64B64_SFLOAT:
            return true;

        case VK_FORMAT_R8G8B8A8_UNORM:
        case VK_FORMAT_R8G8B8A8_SNORM:
        case VK_FORMAT_R8G8B8A8_USCALED:
        case VK_FORMAT_R8G8B8A8_SSCALED:
        case VK_FORMAT_R8G8B8A8_UINT:
        case VK_FORMAT_R8G8B8A8_SINT:
        case VK_FORMAT_R8G8B8A8_SRGB:
        case VK_FORMAT_R16G16B16A16_UNORM:
        case VK_FORMAT_R16G16B16A16_SNORM:
        case VK_FORMAT_R16G16B16A16_USCALED:
        case VK_FORMAT_R16G16B16A16_SSCALED:
        case VK_FORMAT_R16G16B16A16_UINT:
        case VK_FORMAT_R16G16B16A16_SINT:
        case VK_FORMAT_R16G16B16A16_SFLOAT:
        case VK_FORMAT_R32G32B32A32_UINT:
        case VK_FORMAT_R32G32B32A32_SINT:
        case VK_FORMAT_R32G32B32A32_SFLOAT:
        case VK_FORMAT_R64G64B64A64_UINT:
        case VK_FORMAT_R64G64B64A64_SINT:
        case VK_FORMAT_R64G64B64A64_SFLOAT:
            return true;    

        default:
            return false;
        }
    }

    bool formatIsBgr(VkFormat format)
    {
        switch(format)
        {
            case VK_FORMAT_B8G8R8_UNORM:
            case VK_FORMAT_B8G8R8_SNORM:
            case VK_FORMAT_B8G8R8_USCALED:
            case VK_FORMAT_B8G8R8_SSCALED:
            case VK_FORMAT_B8G8R8_UINT:
            case VK_FORMAT_B8G8R8_SINT:
            case VK_FORMAT_B8G8R8_SRGB:
                return true;

            case VK_FORMAT_B8G8R8A8_UNORM:
            case VK_FORMAT_B8G8R8A8_SNORM:
            case VK_FORMAT_B8G8R8A8_USCALED:
            case VK_FORMAT_B8G8R8A8_SSCALED:
            case VK_FORMAT_B8G8R8A8_UINT:
            case VK_FORMAT_B8G8R8A8_SINT:
            case VK_FORMAT_B8G8R8A8_SRGB:
                return true;
            
            default:
                return false;
        }
    }

    VkSurfaceFormatKHR getFormat(const std::vector<VkSurfaceFormatKHR>& formats)
    {
        auto is_valid_format = [] (const auto& format)
        {
            return formatIsRgb(format.format) || formatIsBgr(format.format);
        };
        
        for(const auto& format: formats | std::views::filter(is_valid_format))
            return format;

        throw exception::InvalidState("[vk-surface] failed find surface format");
    }
}

namespace pbrlib::backend::vk
{
    Surface::Surface(Device& device, const pbrlib::Window* ptr_window) :
        _device (device)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        if (!ptr_window) [[unlikely]]
            throw exception::InvalidArgument("[vk-surface] pointer to window is null");

        createSurface(ptr_window);
        
        _surface_format = getFormat(getSurfaceFormats());

        createSwapchain(ptr_window);

        const auto [width, height] = ptr_window->size();
        getImages(width, height);
        createImageViews(static_cast<uint32_t>(width), static_cast<uint32_t>(height));
    }

    Surface::Surface(Surface&& surface) :
        _surface_format (surface._surface_format),
        _device         (surface._device),
        _images         (std::move(surface._images))
    {
        std::swap(_surface_handle, surface._surface_handle);
        std::swap(_swapchain_handle, surface._swapchain_handle);
        std::swap(_next_image_fence_handle, surface._next_image_fence_handle);
    }

    Surface& Surface::operator = (Surface&& surface)
    {
        std::swap(_surface_handle, surface._surface_handle);
        std::swap(_swapchain_handle, surface._swapchain_handle);

        std::swap(_images, surface._images);

        std::swap(_next_image_fence_handle, surface._next_image_fence_handle);

        _surface_format = surface._surface_format;

        return *this;
    }

    void Surface::createSurface(const pbrlib::Window* ptr_window)
    {
        auto ptr_sdl_window = static_cast<SDL_Window*>(ptr_window->_ptr_window);

        if (SDL_Vulkan_CreateSurface(ptr_sdl_window, _device.instance(), nullptr, &_surface_handle.handle()) == SDL_FALSE) [[unlikely]]
            throw exception::InitializeError("[vk-surface] failed create");
    }

    void Surface::createSwapchain(const pbrlib::Window* ptr_window)
    {
        VkSurfaceCapabilitiesKHR capabilities = { };

        VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
            _device.physicalDevice(), 
            _surface_handle, 
            &capabilities
        ));

        constexpr uint32_t image_count = 2;

        const auto [width, height] = ptr_window->size();
        
        const auto family_index = _device.queue().family_index;

        const VkSwapchainCreateInfoKHR swapchain_info 
        { 
            .sType                  = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
            .surface                = _surface_handle,
            .minImageCount          = image_count,
            .imageFormat            = _surface_format.format,
            .imageColorSpace        = _surface_format.colorSpace,
            .imageExtent            = {static_cast<uint32_t>(width), static_cast<uint32_t>(height)},
            .imageArrayLayers       = 1,
            .imageUsage             = capabilities.supportedUsageFlags,
            .imageSharingMode       = VK_SHARING_MODE_EXCLUSIVE,
            .queueFamilyIndexCount  = 1,
            .pQueueFamilyIndices    = &family_index,
            .preTransform           = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
            .compositeAlpha         = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
            .presentMode            = VK_PRESENT_MODE_IMMEDIATE_KHR,
            .clipped                = VK_TRUE
        };

        VK_CHECK(vkCreateSwapchainKHR(_device.device(), &swapchain_info, nullptr, &_swapchain_handle.handle()));
    }

    std::vector<VkSurfaceFormatKHR> Surface::getSurfaceFormats()
    {
        uint32_t num_formats = 0;

        VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(
            _device.physicalDevice(), 
            _surface_handle, 
            &num_formats, 
            nullptr
        ));

        std::vector<VkSurfaceFormatKHR> formats(num_formats);

        VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(
            _device.physicalDevice(), 
            _surface_handle, 
            &num_formats, 
            formats.data()
        ));

        return formats;
    }

    void Surface::getImages(uint32_t width, uint32_t height)
    {
        uint32_t count = 0;

        VK_CHECK(vkGetSwapchainImagesKHR(
            _device.device(), 
            _swapchain_handle, 
            &count, nullptr
        ));

        std::vector<VkImage> handles (count);
        
        VK_CHECK(vkGetSwapchainImagesKHR(
            _device.device(), 
            _swapchain_handle, 
            &count, handles.data()
        ));

        for (auto handle: handles)
        {
            vk::Image image (_device);
            image.handle    = ImageHandle(handle, VK_NULL_HANDLE, false);
            image.width     = width; 
            image.height    = height; 
            image.format    = _surface_format.format;

            _images.push_back(std::move(image));
        }
    }

    void Surface::createImageViews(uint32_t width, uint32_t height)
    {
        const VkImageSubresourceRange subresource 
        { 
            .aspectMask      = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel    = 0,
            .levelCount      = 1,
            .baseArrayLayer  = 0,
            .layerCount      = 1
        };

        constexpr VkComponentMapping component_mapping 
        { 
            .r = VK_COMPONENT_SWIZZLE_IDENTITY,
            .g = VK_COMPONENT_SWIZZLE_IDENTITY,
            .b = VK_COMPONENT_SWIZZLE_IDENTITY,
            .a = VK_COMPONENT_SWIZZLE_IDENTITY
        };

        VkImageViewCreateInfo image_view_info 
        { 
            .sType               = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .viewType            = VK_IMAGE_VIEW_TYPE_2D,
            .format              = _surface_format.format,
            .components          = component_mapping,
            .subresourceRange    = subresource
        };

        for (auto& image: _images)
        {
            image_view_info.image = image.handle.handle();
            VK_CHECK(vkCreateImageView(_device.device(), &image_view_info, nullptr, &image.view_handle.handle()));
        }
    }

    NextImageInfo Surface::nextImage()
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        if (_next_image_fence_handle == VK_NULL_HANDLE)
        {
            constexpr VkFenceCreateInfo fence_create_info
            {
                .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO
            };

            VK_CHECK(vkCreateFence(_device.device(), &fence_create_info, nullptr, &_next_image_fence_handle.handle()));
        }

        VK_CHECK(vkAcquireNextImageKHR(
            _device.device(),
            _swapchain_handle,
            std::numeric_limits<uint64_t>::max(),
            VK_NULL_HANDLE, _next_image_fence_handle, 
            &_current_image_index
        ));

        VK_CHECK(vkWaitForFences(
            _device.device(),
            1, &_next_image_fence_handle.handle(),
            VK_TRUE, std::numeric_limits<uint64_t>::max()
        ));

        VK_CHECK(vkResetFences(_device.device(), 1, &_next_image_fence_handle.handle()));

        return NextImageInfo
        {
            .ptr_image  = &_images[_current_image_index],
            .index      = _current_image_index
        };
    }   
}