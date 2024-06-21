//
//  Image.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 06/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <pbrlib/Rendering/VulkanWrapper/Image.hpp>

using namespace std;

namespace pbrlib
{
    /**
     * @brief Функция вычисляющая размер формата текселя.
     * @details На данный момент поддерживаются только типы float.
     *
     * @param format формат.
     * @return Размер в байтах.
    */
    uint32_t getFormatSize(VkFormat format)
    {
        if (format == VK_FORMAT_R32G32B32A32_SFLOAT) {
            return 16;
        } else if (format == VK_FORMAT_R32G32B32_SFLOAT) {
            return 12;
        } else if (format == VK_FORMAT_R32G32_SFLOAT || format == VK_FORMAT_R16G16B16A16_SFLOAT) {
            return 8;
        } else if (format == VK_FORMAT_R16G16B16_SFLOAT || format == VK_FORMAT_D32_SFLOAT_S8_UINT) {
            return 6;
        } else if (
            format == VK_FORMAT_R32_SFLOAT      ||
            format == VK_FORMAT_R16G16_SFLOAT   ||
            format == VK_FORMAT_R8G8B8A8_SRGB   ||
            format == VK_FORMAT_D32_SFLOAT
            ) {
            return 4;
        } else if (format == VK_FORMAT_R8G8B8_SRGB) {
            return 3;
        } else if (format == VK_FORMAT_R16_SFLOAT || format == VK_FORMAT_R8G8_SRGB) {
            return 2;
        } else if (format == VK_FORMAT_R8_SRGB || format == VK_FORMAT_R8_UINT) {
            return 1;
        }

        return 0;
    }

    Image::Image(
        const PtrDevice&    ptr_device,
        uint32_t            memory_type_index,
        const ImageInfo&    image_info,
        uint32_t            queue_family_index
    ) :
        _image_handle           (VK_NULL_HANDLE),
        _image_info             (image_info),
        _queue_family_indicies  (1),
        _ptr_device             (ptr_device),
        _ptr_device_memory      (nullptr)
    {
        _queue_family_indicies[0] = queue_family_index;
        _create(memory_type_index);
    }

    Image::Image(
        const PtrDevice&        ptr_device,
        uint32_t                memory_type_index,
        const ImageInfo&        image_info,
        span<const uint32_t>    queue_family_indices
    ) :
        _image_handle           (VK_NULL_HANDLE),
        _image_info             (image_info),
        _queue_family_indicies  (std::begin(queue_family_indices), std::end(queue_family_indices)),
        _ptr_device             (ptr_device),
        _ptr_device_memory      (nullptr)
    {
        _create(memory_type_index);
    }

    Image::Image(
        const PtrDevice&    ptr_device,
        VkImage             image,
        ImageInfo           image_info,
        uint32_t            queue_family_index
    ) :
        _image_handle           (image),
        _image_info             (image_info),
        _queue_family_indicies  (1),
        _ptr_device             (ptr_device),
        _ptr_device_memory      (nullptr)
    {
        _queue_family_indicies[0] = queue_family_index;
    }

    Image::Image(
        const PtrDevice&        ptr_device,
        VkImage                 image,
        ImageInfo               image_info,
        span<const uint32_t>    queue_family_indicies
    ) :
        _image_handle           (image),
        _image_info             (image_info),
        _queue_family_indicies  (std::begin(queue_family_indicies), std::end(queue_family_indicies)),
        _ptr_device             (ptr_device),
        _ptr_device_memory      (nullptr)
    {}

    Image::Image(Image&& image) :
        _image_handle           (VK_NULL_HANDLE),
        _image_info             (image._image_info),
        _queue_family_indicies  (move(image._queue_family_indicies)),
        _ptr_device             (image._ptr_device),
        _ptr_device_memory      (nullptr)
    {
        swap(_ptr_device_memory, image._ptr_device_memory);
        swap(_image_handle, image._image_handle);
    }

    Image::~Image()
    {
        if (_image_handle != VK_NULL_HANDLE) {
            vkDestroyImage(_ptr_device->getDeviceHandle(), _image_handle, nullptr);
        }
    }

    void Image::_create(uint32_t memory_type_index)
    {
        uint32_t num_queue_indicies = static_cast<uint32_t>(_queue_family_indicies.size());

        assert(num_queue_indicies);

        VkImageCreateInfo image_create_info = { };
        image_create_info.sType                 = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        image_create_info.imageType             = _image_info.image_type;
        image_create_info.format                = _image_info.format;
        image_create_info.extent                = _image_info.image_extend;
        image_create_info.mipLevels             = _image_info.mip_levels;
        image_create_info.arrayLayers           = _image_info.array_layers;
        image_create_info.samples               = _image_info.num_samples;
        image_create_info.tiling                = _image_info.tiling;
        image_create_info.usage                 = _image_info.usage;
        image_create_info.sharingMode           = (num_queue_indicies != 1 ? VK_SHARING_MODE_CONCURRENT : VK_SHARING_MODE_EXCLUSIVE);
        image_create_info.queueFamilyIndexCount = num_queue_indicies;
        image_create_info.pQueueFamilyIndices   = _queue_family_indicies.data();
        image_create_info.initialLayout         = VK_IMAGE_LAYOUT_UNDEFINED;

        assert(vkCreateImage(_ptr_device->getDeviceHandle(), &image_create_info, nullptr, &_image_handle) == VK_SUCCESS);
        assert(_image_handle);

        VkMemoryRequirements image_mr = { };
        vkGetImageMemoryRequirements(_ptr_device->getDeviceHandle(), _image_handle, &image_mr);

        _ptr_device_memory = DeviceMemory::make(_ptr_device, image_mr.size, memory_type_index);

        assert(vkBindImageMemory(_ptr_device->getDeviceHandle(), _image_handle, _ptr_device_memory->getDeviceMemoryHandle(), 0) == VK_SUCCESS);
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

    PtrDevice& Image::getDevice() noexcept
    {
        return _ptr_device;
    }

    const PtrDevice& Image::getDevice() const noexcept
    {
        return _ptr_device;
    }   

    PtrDeviceMemory& Image::getDeviceMemory() noexcept
    {
        return _ptr_device_memory;
    }

    const PtrDeviceMemory& Image::getDeviceMemory() const noexcept
    {
        return _ptr_device_memory;
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
        uint32_t                    memory_type_index,
        const ImageInfo&            image_info,
        std::span<const uint32_t>   queue_family_indices
    )
    {
        return make_shared<Image>(ptr_device, memory_type_index, image_info, queue_family_indices);
    }

    PtrImage Image::make(
        const PtrDevice&            ptr_device,
        VkImage                     image,
        const ImageInfo&            image_info,
        uint32_t                    queue_family_index
    )
    {
        return make_shared<Image>(ptr_device, image, image_info, queue_family_index);
    }

    PtrImage Image::make(
        const PtrDevice&            ptr_device,
        VkImage                     image,
        const ImageInfo&            image_info,
        span<const uint32_t>        queue_family_indicies
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
        _image_view_handle  (VK_NULL_HANDLE),
        _format             (format),
        _subresource_range  (subresource_range),
        _type               (type)
    {
        VkImageViewCreateInfo image_view_info = { };
        image_view_info.sType               = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        image_view_info.image               = _ptr_image->getImageHandle();
        image_view_info.viewType            = _type;
        image_view_info.format              = _format;
        image_view_info.components          = {VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY};
        image_view_info.subresourceRange    = _subresource_range;

        assert(vkCreateImageView(
            _ptr_image->getDevice()->getDeviceHandle(),
            &image_view_info,
            nullptr,
            &_image_view_handle
        ) == VK_SUCCESS);

        assert(_image_view_handle != VK_NULL_HANDLE);
    }

    ImageView::ImageView(ImageView&& image_view) :
        _ptr_image          (move(image_view._ptr_image)),
        _image_view_handle  (VK_NULL_HANDLE),
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
