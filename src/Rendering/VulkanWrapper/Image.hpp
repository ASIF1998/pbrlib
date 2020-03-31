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

#include <vector>
#include <memory>

using namespace std;

namespace pbrlib
{
    class Image;

    using PtrImage = shared_ptr<Image>;

    /**
     * @struct ImageInfo.
     * @brief Структура описывающая информацию об изображении.
    */
    struct ImageInfo
    {
        VkImageType             image_type;
        VkExtent3D              image_extend;
        VkFormat                format;
        VkImageUsageFlags       usage;
        VkSampleCountFlagBits   num_samples;
        VkImageTiling           tiling;
        uint32_t                mip_levels;
        uint32_t                array_layers;
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
         * @param ptr_device            указатель на устройство.
         * @param memory_type_index     индекс типа памяти.
         * @param image_info            информация об изображении.
         * @param queue_family_index    индекс семейства очередей.
        */
        Image(
            const PtrDevice&            ptr_device,
            uint32_t                    memory_type_index, 
            const ImageInfo&            image_info, 
            uint32_t                    queue_family_index
        );

        /**
         * @brief Конструктор.
         * @details На данный момент поддерживаются только типы float.
         * 
         * @param ptr_device            указатель на устройство.
         * @param memory_type_index     индекс типа памяти.
         * @param image_info            информация об изображении.
         * @param queue_family_indices  индексы семейства очередей.
        */
        Image(
            const PtrDevice&            ptr_device,
            uint32_t                    memory_type_index, 
            const ImageInfo&            image_info, 
            const vector<uint32_t>&     queue_family_indices
        );

        /**
         * @brief Конструктор.
         * @details На данный момент поддерживаются только типы float.
         * 
         * @param ptr_device            указатель на устройство.
         * @param memory_type_index     индекс типа памяти.
         * @param image_info            информация об изображении.
         * @param queue_family_indices  индексы семейства очередей.
        */
        Image(
            const PtrDevice&            ptr_device,
            uint32_t                    memory_type_index, 
            const ImageInfo&            image_info, 
            vector<uint32_t>&&          queue_family_indices
        );

        /**
         * @brief Конструктор.
         *
         * @param ptr_device            указатель на устройство.
         * @param image                 дескриптор уже созданного, но не прикреплённого к памяти изображения.
         * @param image_info            информация об изображении.
         * @param queue_family_index    индекс семейства очередей.
        */
        Image(
            const PtrDevice&            ptr_device,
            VkImage                     image,
            ImageInfo                   image_info,
            uint32_t                    queue_family_index
        );
        
        /**
         * @brief Конструктор.
         *
         * @param ptr_device            указатель на устройство.
         * @param image                 дескриптор уже созданного, но не прикреплённого к памяти изображения.
         * @param image_info            информация об изображении.
         * @param queue_family_indicies индексы семейства очередей.
        */
        Image(
            const PtrDevice&            ptr_device,
            VkImage                     image,
            ImageInfo                   image_info,
            const vector<uint32_t>&     queue_family_indicies
        );

        /**
         * @brief Конструктор.
         *
         * @param ptr_device            указатель на устройство.
         * @param image                 дескриптор уже созданного, но не прикреплённого к памяти изображения.
         * @param image_info            информация об изображении.
         * @param queue_family_indicies индексы семейства очередей.
        */
        Image(
            const PtrDevice&            ptr_device,
            VkImage                     image,
            ImageInfo                   image_info,
            vector<uint32_t>&&          queue_family_indicies
        );

        Image(Image&& image);
        Image(const Image&) = delete;

        ~Image();

        Image& operator = (Image&&)         = delete;
        Image& operator = (const Image&)    = delete;

        ImageInfo&       getImageInfo();
        const ImageInfo& getImageInfo()      const;
        const VkImage&   getImageHandle()    const;

        /**
         * @brief Статический метод для создания Image.
         * 
         * @param ptr_device            указатель на устройство.
         * @param memory_type_index     индекс типа памяти.
         * @param image_info            информация об изображении.
         * @param queue_family_index    индекс семейства очередей.
         * @return указатель на Image.
        */
        static PtrImage make(
            const PtrDevice&            ptr_device,
            uint32_t                    memory_type_index,
            const ImageInfo&            image_info,
            uint32_t                    queue_family_index
        );

        /**
         * @brief Статический метод для создания Image.
         * 
         * @param ptr_device            указатель на устройство.
         * @param memory_type_index     индекс типа памяти.
         * @param image_info            информация об изображении.
         * @param queue_family_indices  индексы семейства очередей.
         * @return указатель на Image.
        */
        static PtrImage make(
            const PtrDevice&            ptr_device,
            uint32_t                    memory_type_index,
            const ImageInfo&            image_info,
            const vector<uint32_t>&     queue_family_indices
        );

        /**
         * @brief Статический метод для создания Image.
         *
         * @param ptr_device            указатель на устройство.
         * @param image                 дескриптор уже созданного, но не прикреплённого к памяти изображения.
         * @param image_info            информация об изображении.
         * @param queue_family_index    индекс семейства очередей.
         * @return указатель на Image.
        */
        static PtrImage make(
            const PtrDevice&            ptr_device,
            VkImage                     image,
            const ImageInfo&            image_info,
            uint32_t                    queue_family_index
        );

        /**
         * @brief Статический метод для создания Image.
         *
         * @param ptr_device            указатель на устройство.
         * @param image                 дескриптор уже созданного, но не прикреплённого к памяти изображения.
         * @param image_info            информация об изображении.
         * @param queue_family_indicies индексы семейства очередей.
         * @return указатель на Image.
        */
        static PtrImage make(
            const PtrDevice&            ptr_device,
            VkImage                     image,
            const ImageInfo&            image_info,
            const vector<uint32_t>&     queue_family_indicies
        );

    private:
        void _create();

    private:
        VkImage             _image_handle;
        ImageInfo           _image_info;
        vector<uint32_t>    _queue_family_indicies;
    };

    class ImageView
    {
        friend class Swapchain;
        
    public:
        /**
         * @brief Конструктор.
         * 
         * @param ptr_image         указатель нк изображение.
         * @param format            формат текселя изображения.
         * @param subresource_range аргумент задающий подмножество изображения на которое указывает ptr_image.
         * @param type              тип вида изображения.
        */
        ImageView(
            const PtrImage&                 ptr_image,
            VkFormat                        format, 
            const VkImageSubresourceRange&  subresource_range, 
            VkImageViewType                 type
        );

        ImageView(ImageView&& image_view);
        ImageView(const ImageView&) = delete;

        ~ImageView();

        ImageView& operator = (ImageView&&)         = delete;
        ImageView& operator = (const ImageView&)    = delete;

        PtrImage&                        getImage()              noexcept;
        const PtrImage&                  getImage()              const noexcept;
        VkImageViewType                  getImageViewType()      const noexcept;
        VkFormat                         getFormat()             const noexcept;
        VkImageSubresourceRange&         getSubresourceRange()   noexcept;
        const VkImageSubresourceRange&   getSubresourceRange()   const noexcept;
        const VkImageView&               getImageViewHandle()    const noexcept;

    private:
        PtrImage                _ptr_image;
        VkImageView             _image_view_handle;
        VkFormat                _format;
        VkImageSubresourceRange _subresource_range;
        VkImageViewType         _type;
    };
}

#endif /* Image_hpp */
