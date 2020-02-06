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

        ~Image();

        ImageInfo& getImageInfo();
        const ImageInfo& getImageInfo() const;
        VkImage getImageHandle() const;

    private:
        VkImage _image_handle;
        ImageInfo _image_info;
        vector<uint32_t> _queue_family_indicies;
    };
}

#endif /* Image_hpp */
