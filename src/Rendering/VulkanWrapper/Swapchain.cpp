//
//  Swapchain.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 09/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include "Swapchain.hpp"

namespace pbrlib
{
    void Swapchain::_create(
        const shared_ptr<Device>&   ptr_device,
        const vector<uint32_t>&     queue_family_indices,
        VkSharingMode               sharing_mode
    )
    {
        VkSwapchainCreateInfoKHR swapchain_info {
            .sType                  = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
            .pNext                  = nullptr,
            .flags                  = 0,
            .surface                = _ptr_surface->getSurfaceHandle(),
            .minImageCount          = _ptr_surface->getSurfaceCapabilities().maxImageCount,
            .imageFormat            = _ptr_surface->getSurfaceFormat().format,
            .imageColorSpace        = _ptr_surface->getSurfaceFormat().colorSpace,
            .imageExtent            = _ptr_surface->getSurfaceCapabilities().minImageExtent,
            .imageArrayLayers       = 1,
            .imageUsage             = _ptr_surface->getSurfaceCapabilities().supportedUsageFlags,
            .imageSharingMode       = sharing_mode,
            .queueFamilyIndexCount  = static_cast<uint32_t>(queue_family_indices.size()),
            .pQueueFamilyIndices    = queue_family_indices.data(),
            .preTransform           = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
            .compositeAlpha         = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
            .presentMode            = VK_PRESENT_MODE_FIFO_KHR,
            .clipped                = VK_TRUE,
            .oldSwapchain           = VK_NULL_HANDLE
        };

        ImageInfo image_info {
            .image_type     = VK_IMAGE_TYPE_2D,
            .format         = swapchain_info.imageFormat,
            .usage          = swapchain_info.imageUsage,
            .num_samples    = VK_SAMPLE_COUNT_1_BIT,
            .tiling         = VK_IMAGE_TILING_LINEAR,
            .mip_levels     = 1,
            .array_layers   = 1,
            .image_extend = {
                swapchain_info.imageExtent.width, 
                swapchain_info.imageExtent.height, 
                1
            }
        };

        VkImageSubresourceRange image_subresource_range {
            .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel   = 0,
            .levelCount     = 1,
            .baseArrayLayer = 0,
            .layerCount     = 1
        };

        assert(vkCreateSwapchainKHR(
            ptr_device->getDeviceHandle(), 
            &swapchain_info, 
            nullptr, 
            &_swapchain_handle
        ) == VK_SUCCESS);

        assert(_swapchain_handle != VK_NULL_HANDLE);

        uint32_t num_images = 0;

        vkGetSwapchainImagesKHR(ptr_device->getDeviceHandle(), _swapchain_handle, &num_images, nullptr);
        assert(num_images);

        vector<VkImage>             vk_images (num_images);
        vector<shared_ptr<Image>>   ptr_images;

        vkGetSwapchainImagesKHR(ptr_device->getDeviceHandle(), _swapchain_handle, &num_images, vk_images.data());
        
        ptr_images.reserve(num_images);
        _images_view.reserve(num_images);

        for (size_t i{0}; i < vk_images.size(); i++) {
            ptr_images.push_back(Image::make(ptr_device, vk_images[i], image_info, queue_family_indices));
        }

        for (size_t i{0}; i < ptr_images.size(); i++) {
            _images_view.push_back(ImageView(ptr_images[i], swapchain_info.imageFormat, image_subresource_range, VK_IMAGE_VIEW_TYPE_2D));
        }
    }
}
