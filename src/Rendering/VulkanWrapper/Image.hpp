//
//  Image.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 06/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef Image_hpp
#define Image_hpp

#include "DeviceMemory.hpp"
#include "Device.hpp"

#include <vector>

#include <memory>

using namespace std;

namespace pbrlib
{
    /**
     * @struct ImageInfo.
     * @brief Структура описывающая информацию об изображении.
    */
    struct ImageInfo
    {
        VkImageType image_type;
        VkExtent3D image_extend;
        VkFormat format;
        VkImageUsageFlags usage;
        VkSampleCountFlagBits num_samples;
        VkImageTiling tiling;
        uint32_t mip_levels;
        uint32_t array_layers;
    };

    class Image: 
        public DeviceMemory
    {
    public:
        /**
         * @brief Конструктор.
         * @details На данный момент поддерживаются только типы float.
         * 
         * @param ptr_device указатель на устройство.
         * @param memory_type_index индекс типа памяти.
         * @param image_info информация об изображении.
         * @param queue_family_index индекс семейства очередей.
        */
        Image(const shared_ptr<Device>& ptr_device, 
              uint32_t memory_type_index, 
              const ImageInfo& image_info, 
              uint32_t queue_family_index);

        /**
         * @brief Конструктор.
         * @details На данный момент поддерживаются только типы float.
         * 
         * @param ptr_device указатель на устройство.
         * @param memory_type_index индекс типа памяти.
         * @param image_info информация об изображении.
         * @param queue_family_indices индексы семейства очередей.
        */
        Image(const shared_ptr<Device>& ptr_device,
              uint32_t memory_type_index, 
              const ImageInfo& image_info, 
              vector<uint32_t> queue_family_indices);

        inline ~Image();

        inline ImageInfo& getImageInfo();
        inline const ImageInfo& getImageInfo() const;
        inline VkImage getImageHandle() const;

        /**
         * @brief Статический метод для создания Image.
         * 
         * @param ptr_device указатель на устройство.
         * @param memory_type_index индекс типа памяти.
         * @param image_info информация об изображении.
         * @param queue_family_index индекс семейства очередей.
         * @return указатель на Image.
        */
        inline static shared_ptr<Image> makeImage(const shared_ptr<Device>& ptr_device, 
                                                  uint32_t memory_type_index, 
                                                  const ImageInfo& image_info, 
                                                  uint32_t queue_family_index);

        /**
         * @brief Статический метод для создания Image.
         * 
         * @param ptr_device указатель на устройство.
         * @param memory_type_index индекс типа памяти.
         * @param image_info информация об изображении.
         * @param queue_family_indices индексы семейства очередей.
         * @return указатель на Image.
        */
        static shared_ptr<Image> makeImage(const shared_ptr<Device>& ptr_device, 
                                           uint32_t memory_type_index, 
                                           const ImageInfo& image_info, 
                                           vector<uint32_t> queue_family_indices);

    private:
        VkImage _image_handle;
        ImageInfo _image_info;
        vector<uint32_t> _queue_family_indicies;
    };

    class ImageView
    {
    public:
        ImageView(const shared_ptr<Image>& ptr_image, 
                  VkFormat format, 
                  const VkImageSubresourceRange& subresource_range, 
                  VkImageViewType type);

        inline ~ImageView();

        inline shared_ptr<Image>& getImage() noexcept;
        inline const shared_ptr<Image>& getImage() const noexcept;
        inline VkImageViewType getImageViewType() const noexcept;
        inline VkFormat getFormat() const noexcept;
        inline VkImageSubresourceRange& getSubresourceRange() noexcept;
        inline const VkImageSubresourceRange& getSubresourceRange() const noexcept;
        inline VkImageView getImageViewHandle() const noexcept;

    private:
        shared_ptr<Image> _ptr_image;
        VkImageView _image_view_handle;
        VkFormat _format;
        VkImageSubresourceRange _subresource_range;
        VkImageViewType _type;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inline Image::~Image()
    {
        vkDestroyImage(_ptr_device->getDeviceHandle(), _image_handle, nullptr);
    }

    inline ImageInfo& Image::getImageInfo()
    {
        return _image_info;
    }

    inline const ImageInfo& Image::getImageInfo() const
    {
        return _image_info;
    }

    inline VkImage Image::getImageHandle() const
    {
        return _image_handle;
    }

    inline shared_ptr<Image> Image::makeImage(const shared_ptr<Device>& ptr_device, 
                                                  uint32_t memory_type_index, 
                                                  const ImageInfo& image_info, 
                                                  uint32_t queue_family_index)
    {
        return make_shared<Image>(ptr_device, memory_type_index, image_info, queue_family_index);
    }

    inline shared_ptr<Image> Image::makeImage(const shared_ptr<Device>& ptr_device, 
                                       uint32_t memory_type_index, 
                                       const ImageInfo& image_info, 
                                       vector<uint32_t> queue_family_indices)
    {
        return make_shared<Image>(ptr_device, memory_type_index, image_info, queue_family_indices);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inline ImageView::~ImageView()
    {
        vkDestroyImageView(_ptr_image->getDevice()->getDeviceHandle(), _image_view_handle, nullptr);
    }

    inline shared_ptr<Image>& ImageView::getImage() noexcept
    {
        return _ptr_image;
    }

    inline const shared_ptr<Image>& ImageView::getImage() const noexcept 
    {
        return _ptr_image;
    }

    inline VkImageViewType ImageView::getImageViewType() const noexcept
    {
        return _type;
    }

    inline VkFormat ImageView::getFormat() const noexcept
    {
        return _format;
    }

    inline VkImageSubresourceRange& ImageView::getSubresourceRange() noexcept
    {
        return _subresource_range;
    }

    inline const VkImageSubresourceRange& ImageView::getSubresourceRange() const noexcept
    {
        return _subresource_range;
    }

    inline VkImageView ImageView::getImageViewHandle() const noexcept
    {
        return _image_view_handle;
    }
}

#endif /* Image_hpp */
