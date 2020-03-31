//
//  Image.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 06/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include "Image.hpp"

namespace pbrlib
{
    /**
     * @brief Функция вычисляющая размер формата текселя.
     * @details На данный момент поддерживаются только типы float.
     * 
     * @param format формат.
     * @return размер в байтах.
    */
    uint32_t getFormatSize(VkFormat format)
    {
        if (format == VK_FORMAT_R32G32B32A32_SFLOAT) {
            return 16;
        } else if (format == VK_FORMAT_R32G32B32_SFLOAT) {
            return 12;
        } else if (format == VK_FORMAT_R32G32_SFLOAT ||
                   format == VK_FORMAT_R16G16B16A16_SFLOAT) {
            return 8;
        } else if (format == VK_FORMAT_R16G16B16_SFLOAT) {
            return 6;
        } else if (format == VK_FORMAT_R32_SFLOAT ||
                   format == VK_FORMAT_R16G16_SFLOAT) {
            return 4;
        } else if (format == VK_FORMAT_R16_SFLOAT) {
            return 2;
        }
        
        return 0;
    }

    Image::Image(
        const PtrDevice&    ptr_device, 
        uint32_t            memory_type_index, 
        const ImageInfo&    image_info, 
        uint32_t            queue_family_index
    ) :
        DeviceMemory(
            ptr_device, 
            image_info.image_extend.width   * 
            image_info.image_extend.height  * 
            image_info.image_extend.depth   * 
            getFormatSize(image_info.format), 
            memory_type_index
        ),
        _image_handle           (VK_NULL_HANDLE),
        _image_info             (image_info),
        _queue_family_indicies  (1)
    {
        _create();
    }

    Image::Image(
        const PtrDevice&        ptr_device,
        uint32_t                memory_type_index, 
        const ImageInfo&        image_info, 
        const vector<uint32_t>& queue_family_indices
    ) :
        DeviceMemory(
            ptr_device, 
            image_info.image_extend.width   * 
            image_info.image_extend.height  * 
            image_info.image_extend.depth   * 
            getFormatSize(image_info.format), 
            memory_type_index
        ),
        _image_handle           (VK_NULL_HANDLE),
        _image_info             (image_info),
        _queue_family_indicies  (queue_family_indices)
    {
        _create();
    }

    Image::Image(
        const PtrDevice&    ptr_device,
        uint32_t            memory_type_index, 
        const ImageInfo&    image_info, 
        vector<uint32_t>&&  queue_family_indices
    ) :
        DeviceMemory(
            ptr_device, 
            image_info.image_extend.width   * 
            image_info.image_extend.height  * 
            image_info.image_extend.depth   * 
            getFormatSize(image_info.format), 
            memory_type_index
        ),
        _image_handle           (VK_NULL_HANDLE),
        _image_info             (image_info),
        _queue_family_indicies  (move(queue_family_indices))
    {
        _create();
    }

    Image::Image(
        const PtrDevice&    ptr_device,
        VkImage             image,
        ImageInfo           image_info,
        uint32_t            queue_family_index
    ) :
        DeviceMemory(
            ptr_device
        ),
        _image_handle           (image),
        _image_info             (image_info),
        _queue_family_indicies  (1)
    {
        _queue_family_indicies[0] = queue_family_index;
    }

    Image::Image(
        const PtrDevice&        ptr_device,
        VkImage                 image,
        ImageInfo               image_info,
        const vector<uint32_t>& queue_family_indicies
    ) :
        DeviceMemory(
            ptr_device
        ),
        _image_handle           (image),
        _image_info             (image_info),
        _queue_family_indicies  (queue_family_indicies)
    {}

    Image::Image(
        const PtrDevice&    ptr_device,
        VkImage             image,
        ImageInfo           image_info,
        vector<uint32_t>&&  queue_family_indicies
    ) :
        DeviceMemory(
            ptr_device
        ),
        _image_handle(image),
        _image_info(image_info),
        _queue_family_indicies(move(queue_family_indicies))
    {}

    Image::Image(Image&& image) :
        DeviceMemory            (move(image)),
        _image_handle           (VK_NULL_HANDLE),
        _image_info             (image._image_info),
        _queue_family_indicies  (move(image._queue_family_indicies))
    {
        swap(_image_handle, image._image_handle);
    }

    Image::~Image()
    {
        if (_image_handle != VK_NULL_HANDLE) {
            vkDestroyImage(_ptr_device->getDeviceHandle(), _image_handle, nullptr);
        }
    }

    void Image::_create()
    {
        uint32_t num_queue_indicies = static_cast<uint32_t>(_queue_family_indicies.size());

        assert(num_queue_indicies);

        VkImageCreateInfo image_create_info {
            .sType                  = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
            .pNext                  = nullptr,
            .flags                  = 0,
            .imageType              = _image_info.image_type,
            .format                 = _image_info.format,
            .extent                 = _image_info.image_extend,
            .mipLevels              = _image_info.mip_levels,
            .arrayLayers            = _image_info.array_layers,
            .samples                = _image_info.num_samples,
            .tiling                 = _image_info.tiling,
            .usage                  = _image_info.usage,
            .sharingMode            = (num_queue_indicies != 1 ? VK_SHARING_MODE_CONCURRENT : VK_SHARING_MODE_EXCLUSIVE),
            .queueFamilyIndexCount  = num_queue_indicies,
            .pQueueFamilyIndices    = _queue_family_indicies.data(),
            .initialLayout          = VK_IMAGE_LAYOUT_UNDEFINED
        };

        assert(vkCreateImage(_ptr_device->getDeviceHandle(), &image_create_info, nullptr, &_image_handle) == VK_SUCCESS);
        assert(_image_handle);
        assert(vkBindImageMemory(_ptr_device->getDeviceHandle(), _image_handle, _device_memory_handle, 0) == VK_SUCCESS);
    }

    ImageInfo& Image::getImageInfo()
    {
        return _image_info;
    }

    const ImageInfo& Image::getImageInfo() const
    {
        return _image_info;
    }

    const VkImage& Image::getImageHandle() const
    {
        return _image_handle;
    }

    PtrImage Image::make(
        const PtrDevice&            ptr_device,
        uint32_t                    memory_type_index,
        const ImageInfo&            image_info,
        uint32_t                    queue_family_index
    )
    {
        return make_shared<Image>(ptr_device, memory_type_index, image_info, queue_family_index);
    }

    PtrImage Image::make(
        const PtrDevice&            ptr_device,
        uint32_t                   memory_type_index,
        const ImageInfo&           image_info,
        const vector<uint32_t>&    queue_family_indices
    )
    {
        return make_shared<Image>(ptr_device, memory_type_index, image_info, queue_family_indices);
    }

    PtrImage Image::make(
        const PtrDevice&            ptr_device,
        VkImage                    image,
        const ImageInfo&           image_info,
        uint32_t                   queue_family_index
    )
    {
        return make_shared<Image>(ptr_device, image, image_info, queue_family_index);
    }

    PtrImage Image::make(
        const PtrDevice&           ptr_device,
        VkImage                    image,
        const ImageInfo&           image_info,
        const vector<uint32_t>&    queue_family_indicies
    )
    {
        return make_shared<Image>(ptr_device, image, image_info, queue_family_indicies);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ImageView::ImageView(
        const PtrImage&                 ptr_image, 
        VkFormat                        format, 
        const VkImageSubresourceRange&  subresource_range, 
        VkImageViewType                 type
    ) :
        _ptr_image          (ptr_image),
        _format             (format),
        _image_view_handle  (VK_NULL_HANDLE),
        _subresource_range  (subresource_range),
        _type               (type)
    {
        VkImageViewCreateInfo image_view_info {
            .sType      = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .pNext      = nullptr,
            .flags      = 0,
            .image      = _ptr_image->getImageHandle(),
            .viewType   = _type,
            .format     = _format,
            .components = {
                VK_COMPONENT_SWIZZLE_IDENTITY, 
                VK_COMPONENT_SWIZZLE_IDENTITY, 
                VK_COMPONENT_SWIZZLE_IDENTITY, 
                VK_COMPONENT_SWIZZLE_IDENTITY
            },
            .subresourceRange = _subresource_range
        };

        assert(vkCreateImageView(
            _ptr_image->getDevice()->getDeviceHandle(), 
            &image_view_info, 
            nullptr, 
            &_image_view_handle
        ) == VK_SUCCESS);
        
        assert(_image_view_handle != VK_NULL_HANDLE);
    }

    ImageView::ImageView(ImageView&& image_view) :
        _image_view_handle  (VK_NULL_HANDLE),
        _ptr_image          (move(image_view._ptr_image)),
        _format             (image_view._format),
        _subresource_range  (image_view._subresource_range),
        _type               (image_view._type)
    {
        swap(image_view._image_view_handle, _image_view_handle);
    }

    ImageView::~ImageView()
    {
        if (_image_view_handle != VK_NULL_HANDLE) {
            vkDestroyImageView(_ptr_image->getDevice()->getDeviceHandle(), _image_view_handle, nullptr);
        }
    }

    PtrImage& ImageView::getImage() noexcept
    {
        return _ptr_image;
    }

    const PtrImage& ImageView::getImage() const noexcept 
    {
        return _ptr_image;
    }

    VkImageViewType ImageView::getImageViewType() const noexcept
    {
        return _type;
    }

    VkFormat ImageView::getFormat() const noexcept
    {
        return _format;
    }

    VkImageSubresourceRange& ImageView::getSubresourceRange() noexcept
    {
        return _subresource_range;
    }

    const VkImageSubresourceRange& ImageView::getSubresourceRange() const noexcept
    {
        return _subresource_range;
    }

    const VkImageView& ImageView::getImageViewHandle() const noexcept
    {
        return _image_view_handle;
    }
}
