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
        friend class Swapchain;

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
              const vector<uint32_t>& queue_family_indices);

        /**
         * @brief Конструктор.
         *
         * @param ptr_device указатель на устройство.
         * @param image дескриптор уже созданного, но не прикреплённого к памяти изображения.
         * @param image_info информация об изображении.
         * @param queue_family_index индекс семейства очередей.
        */
        Image(const shared_ptr<Device>& ptr_device,
              VkImage image,
              ImageInfo image_info,
              uint32_t queue_family_index);
        
        /**
         * @brief Конструктор.
         *
         * @param ptr_device указатель на устройство.
         * @param image дескриптор уже созданного, но не прикреплённого к памяти изображения.
         * @param image_info информация об изображении.
         * @param queue_family_indicies индексы семейства очередей.
        */
        Image(const shared_ptr<Device>& ptr_device,
              VkImage image,
              ImageInfo image_info,
              const vector<uint32_t>& queue_family_indicies);

        inline Image(Image&& image);
        Image(const Image&) = delete;

        inline ~Image();

        Image& operator = (Image&&) = delete;
        Image& operator = (const Image&) = delete;

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
        inline static shared_ptr<Image> make(const shared_ptr<Device>& ptr_device,
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
        inline static shared_ptr<Image> make(const shared_ptr<Device>& ptr_device,
                                             uint32_t memory_type_index,
                                             const ImageInfo& image_info,
                                             const vector<uint32_t>& queue_family_indices);

        /**
         * @brief Статический метод для создания Image.
         *
         * @param ptr_device указатель на устройство.
         * @param image дескриптор уже созданного, но не прикреплённого к памяти изображения.
         * @param image_info информация об изображении.
         * @param queue_family_index индекс семейства очередей.
         * @return указатель на Image.
        */
        inline static shared_ptr<Image> make(const shared_ptr<Device>& ptr_device,
                                             VkImage image,
                                             const ImageInfo& image_info,
                                             uint32_t queue_family_index);

        /**
         * @brief Статический метод для создания Image.
         *
         * @param ptr_device указатель на устройство.
         * @param image дескриптор уже созданного, но не прикреплённого к памяти изображения.
         * @param image_info информация об изображении.
         * @param queue_family_indicies индексы семейства очередей.
         * @return указатель на Image.
        */
        inline static shared_ptr<Image> make(const shared_ptr<Device>& ptr_device,
                                             VkImage image,
                                             const ImageInfo& image_info,
                                             const vector<uint32_t>& queue_family_indicies);

    private:
        VkImage _image_handle;
        ImageInfo _image_info;
        vector<uint32_t> _queue_family_indicies;
    };

    class ImageView
    {
        friend class Swapchain;
        
    public:
        /**
         * @brief Конструктор.
         * 
         * @param ptr_image указатель нк изображение.
         * @param format формат текселя изображения.
         * @param subresource_range аргумент задающий подмножество изображения на которое указывает ptr_image.
         * @param type тип вида изображения.
        */
        ImageView(const shared_ptr<Image>& ptr_image, 
                  VkFormat format, 
                  const VkImageSubresourceRange& subresource_range, 
                  VkImageViewType type);

        inline ImageView(ImageView&& image_view);
        ImageView(const ImageView&) = delete;

        inline ~ImageView();

        ImageView& operator = (ImageView&&) = delete;
        ImageView& operator = (const ImageView&) = delete;

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
    inline Image::Image(Image&& image) :
        DeviceMemory(move(image)),
        _image_handle(VK_NULL_HANDLE),
        _image_info(image._image_info),
        _queue_family_indicies(move(image._queue_family_indicies))
    {
        swap(_image_handle, image._image_handle);
    }

    inline Image::~Image()
    {
        if (_image_handle != VK_NULL_HANDLE) {
            vkDestroyImage(_ptr_device->getDeviceHandle(), _image_handle, nullptr);
        }
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

    inline shared_ptr<Image> Image::make(const shared_ptr<Device>& ptr_device,
                                         uint32_t memory_type_index,
                                         const ImageInfo& image_info,
                                         uint32_t queue_family_index)
    {
        return make_shared<Image>(ptr_device, memory_type_index, image_info, queue_family_index);
    }

    inline shared_ptr<Image> Image::make(const shared_ptr<Device>& ptr_device,
                                         uint32_t memory_type_index,
                                         const ImageInfo& image_info,
                                         const vector<uint32_t>& queue_family_indices)
    {
        return make_shared<Image>(ptr_device, memory_type_index, image_info, queue_family_indices);
    }

    inline shared_ptr<Image> Image::make(const shared_ptr<Device>& ptr_device,
                                         VkImage image,
                                         const ImageInfo& image_info,
                                         uint32_t queue_family_index)
    {
        return make_shared<Image>(ptr_device, image, image_info, queue_family_index);
    }

    inline shared_ptr<Image> Image::make(const shared_ptr<Device>& ptr_device,
                                         VkImage image,
                                         const ImageInfo& image_info,
                                         const vector<uint32_t>& queue_family_indicies)
    {
        return make_shared<Image>(ptr_device, image, image_info, queue_family_indicies);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inline ImageView::ImageView(ImageView&& image_view) :
        _image_view_handle(VK_NULL_HANDLE),
        _ptr_image(image_view._ptr_image),
        _format(image_view._format),
        _subresource_range(image_view._subresource_range),
        _type(image_view._type)
    {
        swap(image_view._image_view_handle, _image_view_handle);
    }

    inline ImageView::~ImageView()
    {
        if (_image_view_handle != VK_NULL_HANDLE) {
            vkDestroyImageView(_ptr_image->getDevice()->getDeviceHandle(), _image_view_handle, nullptr);
        }
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
