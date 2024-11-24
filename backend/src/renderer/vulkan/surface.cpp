#include <pbrlib/rendering/window.hpp>

#include <backend/renderer/vulkan/device.hpp>
#include <backend/renderer/vulkan/surface.hpp>
#include <backend/utils/vulkan.hpp>

#include <SDL3/SDL_vulkan.h>

#include <ranges>

namespace pbrlib::vk
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

        throw std::runtime_error("[Vulkan] Failed find surface format");
    }
}

namespace pbrlib::vk
{
    Surface::Surface(const Device* ptr_device, const Window* ptr_window) :
        _ptr_device(ptr_device)
    {
        createSurface(ptr_window);
        
        _surface_format = getFormat(getSurfaceFormats());

        createSwapchain(ptr_window);

        getImages();

        const auto [width, height] = ptr_window->size();
        createImageViews(static_cast<uint32_t>(width), static_cast<uint32_t>(height));
    }

    Surface::Surface(Surface&& surface) :
        _surface_format (surface._surface_format),
        _ptr_device     (surface._ptr_device)
    {
        std::swap(_surface_handle, surface._surface_handle);
        std::swap(_swapchain_handle, surface._swapchain_handle);

        std::swap(_image_handles, surface._image_handles);
        std::swap(_image_view_handles, surface._image_view_handles);
    }

    Surface::~Surface()
    {
        for (const auto image_view_handle: _image_view_handles)
            vkDestroyImageView(_ptr_device->device(), image_view_handle, nullptr);

        if (_swapchain_handle != VK_NULL_HANDLE)
            vkDestroySwapchainKHR(_ptr_device->device(), _swapchain_handle, nullptr);

        if (_surface_handle != VK_NULL_HANDLE)
            vkDestroySurfaceKHR(_ptr_device->instance(), _surface_handle, nullptr);
    }

    Surface& Surface::operator = (Surface&& surface)
    {
        std::swap(_surface_handle, surface._surface_handle);
        std::swap(_swapchain_handle, surface._swapchain_handle);

        std::swap(_image_handles, surface._image_handles);
        std::swap(_image_view_handles, surface._image_view_handles);

        _surface_format = surface._surface_format;

        _ptr_device = surface._ptr_device;

        return *this;
    }

    void Surface::createSurface(const Window* ptr_window)
    {
        auto ptr_sdl_window = static_cast<SDL_Window*>(ptr_window->_ptr_window);

        if (SDL_Vulkan_CreateSurface(ptr_sdl_window, _ptr_device->instance(), nullptr, &_surface_handle) == SDL_FALSE)
            throw std::runtime_error("[Vulkan] Failed create surface");
    }

    void Surface::createSwapchain(const Window* ptr_window)
    {
        VkSurfaceCapabilitiesKHR capabilities = { };

        VK_CHECK(
            vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
                _ptr_device->physicalDevice(), 
                _surface_handle, 
                &capabilities
            )
        );

        constexpr uint32_t image_count = 2;

        const auto [width, height] = ptr_window->size();
        
        const auto family_index = _ptr_device->graphicsQueue().family_index;

        VkSwapchainCreateInfoKHR swapchain_info = { };
        swapchain_info.sType                    = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        swapchain_info.clipped                  = VK_TRUE;
        swapchain_info.compositeAlpha           = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        swapchain_info.surface                  = _surface_handle;
        swapchain_info.minImageCount            = image_count;
        swapchain_info.imageFormat              = _surface_format.format;
        swapchain_info.imageColorSpace          = _surface_format.colorSpace;
        swapchain_info.imageExtent              = {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
        swapchain_info.imageArrayLayers         = 1;
        swapchain_info.imageUsage               = capabilities.supportedUsageFlags;
        swapchain_info.imageSharingMode         = VK_SHARING_MODE_EXCLUSIVE;
        swapchain_info.queueFamilyIndexCount    = 1;
        swapchain_info.pQueueFamilyIndices      = &family_index;
        swapchain_info.preTransform             = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
        swapchain_info.compositeAlpha           = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        swapchain_info.presentMode              = VK_PRESENT_MODE_IMMEDIATE_KHR;

        VK_CHECK(vkCreateSwapchainKHR(_ptr_device->device(), &swapchain_info, nullptr, &_swapchain_handle));
    }

    std::vector<VkSurfaceFormatKHR> Surface::getSurfaceFormats()
    {
        uint32_t num_formats = 0;

        VK_CHECK(
            vkGetPhysicalDeviceSurfaceFormatsKHR(
                _ptr_device->physicalDevice(), 
                _surface_handle, 
                &num_formats, 
                nullptr
            )
        );

        std::vector<VkSurfaceFormatKHR> formats(num_formats);

        VK_CHECK(
            vkGetPhysicalDeviceSurfaceFormatsKHR(
                _ptr_device->physicalDevice(), 
                _surface_handle, 
                &num_formats, 
                formats.data()
            )
        );

        return formats;
    }

    void Surface::getImages()
    {
        uint32_t count = 0;

        VK_CHECK(
            vkGetSwapchainImagesKHR(
                _ptr_device->device(), 
                _swapchain_handle, 
                &count, nullptr
            )
        );
        
        _image_handles.resize(count);
        
        VK_CHECK(
            vkGetSwapchainImagesKHR(
                _ptr_device->device(), 
                _swapchain_handle, 
                &count, _image_handles.data()
            )
        );
    }

    void Surface::createImageViews(uint32_t width, uint32_t height)
    {
        _image_view_handles.resize(_image_handles.size());

        VkImageSubresourceRange subresource = { };
        subresource.aspectMask      = VK_IMAGE_ASPECT_COLOR_BIT;
        subresource.baseArrayLayer  = 0;
        subresource.layerCount      = 1;
        subresource.baseMipLevel    = 0;
        subresource.levelCount      = 1;

        VkComponentMapping component_mapping = { };
        component_mapping.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        component_mapping.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        component_mapping.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        component_mapping.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        VkImageViewCreateInfo image_view_info = { };
        image_view_info.sType               = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        image_view_info.format              = _surface_format.format;
        image_view_info.subresourceRange    = subresource;
        image_view_info.components          = component_mapping;
        image_view_info.viewType            = VK_IMAGE_VIEW_TYPE_2D;

        for (uint32_t i = 0; i < _image_view_handles.size(); ++i) 
        {
            image_view_info.image = _image_handles[i];
            VK_CHECK(vkCreateImageView(_ptr_device->device(), &image_view_info, nullptr, &_image_view_handles[i]));
        }
    }
}