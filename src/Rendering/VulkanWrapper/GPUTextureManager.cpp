//
//  GPUTextureManager.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 02/06/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef STB_IMAGE_IMPLEMENTATION
#   define STB_IMAGE_IMPLEMENTATION
#endif

#ifndef STB_IMAGE_STATIC
#   define STB_IMAGE_STATIC
#endif

#define STBI_NO_GIF

#include "GPUTextureManager.hpp"

#include <stb/stb_image.h>

namespace pbrlib
{
    GPUTextureManager::GPUTextureManager(
        const PtrDevice&    ptr_device,
        uint32_t            memory_type_index,
        VkImageTiling       image_tiling,
        VkSampleCountFlags  num_samples
    ) :
        _ptr_device         (ptr_device),
        _memory_type_index  (memory_type_index),
        _image_tiling       (image_tiling),
        _num_samples        (num_samples)
    {}

    bool GPUTextureManager::load(const string_view path, const string_view name)
    {
        if (_textures.find(name.data()) != end(_textures)) {
            return false;
        }

        int width           = 0;
        int height          = 0;
        int num_channels    = 0;

        float* data = stbi_loadf(path.data(), &width, &height, &num_channels, 0);

        if (data && width && height && num_channels) {
            PtrImage ptr_image;

            switch (num_channels) {
                case 1: {
                    using ImageBuilder = Image::BuilderWithData<Image::TexelType::R, float, Image::NumBits::NB32>;

                    ImageBuilder image_builder;

                    image_builder.setData(data, width * height * num_channels);
                    image_builder.setDevice(_ptr_device);
                    image_builder.setExtend(width, height, 1);
                    image_builder.setImageType(VK_IMAGE_TYPE_2D);
                    image_builder.setMemoryTypeIndex(_memory_type_index);
                    image_builder.setNumArrayLayers(1);
                    image_builder.setNumMipLevels(1);
                    image_builder.setNumSamples(VK_SAMPLE_COUNT_1_BIT);
                    image_builder.setTiling(_image_tiling);
                    image_builder.setUsage(VK_IMAGE_USAGE_SAMPLED_BIT);

                    ptr_image = image_builder.buildPtr();
                    
                    break;
                }

                case 2: {
                    using ImageBuilder = Image::BuilderWithData<Image::TexelType::RG, float, Image::NumBits::NB32>;

                    ImageBuilder image_builder;

                    image_builder.setData(data, width * height * num_channels);
                    image_builder.setDevice(_ptr_device);
                    image_builder.setExtend(width, height, 1);
                    image_builder.setImageType(VK_IMAGE_TYPE_2D);
                    image_builder.setMemoryTypeIndex(_memory_type_index);
                    image_builder.setNumArrayLayers(1);
                    image_builder.setNumMipLevels(1);
                    image_builder.setNumSamples(VK_SAMPLE_COUNT_1_BIT);
                    image_builder.setTiling(_image_tiling);
                    image_builder.setUsage(VK_IMAGE_USAGE_SAMPLED_BIT);

                    ptr_image = image_builder.buildPtr();

                    break;
                }

                case 3: {
                    using ImageBuilder = Image::BuilderWithData<Image::TexelType::RGB, float, Image::NumBits::NB32>;

                    ImageBuilder image_builder;

                    image_builder.setData(data, width * height * num_channels);
                    image_builder.setDevice(_ptr_device);
                    image_builder.setExtend(width, height, 1);
                    image_builder.setImageType(VK_IMAGE_TYPE_2D);
                    image_builder.setMemoryTypeIndex(_memory_type_index);
                    image_builder.setNumArrayLayers(1);
                    image_builder.setNumMipLevels(1);
                    image_builder.setNumSamples(VK_SAMPLE_COUNT_1_BIT);
                    image_builder.setTiling(_image_tiling);
                    image_builder.setUsage(VK_IMAGE_USAGE_SAMPLED_BIT);

                    ptr_image = image_builder.buildPtr();

                    break;
                }

                case 4: {
                    using ImageBuilder = Image::BuilderWithData<Image::TexelType::RGBA, float, Image::NumBits::NB32>;

                    ImageBuilder image_builder;

                    image_builder.setData(data, width * height * num_channels);
                    image_builder.setDevice(_ptr_device);
                    image_builder.setExtend(width, height, 1);
                    image_builder.setImageType(VK_IMAGE_TYPE_2D);
                    image_builder.setMemoryTypeIndex(_memory_type_index);
                    image_builder.setNumArrayLayers(1);
                    image_builder.setNumMipLevels(1);
                    image_builder.setNumSamples(VK_SAMPLE_COUNT_1_BIT);
                    image_builder.setTiling(_image_tiling);
                    image_builder.setUsage(VK_IMAGE_USAGE_SAMPLED_BIT);

                    ptr_image = image_builder.buildPtr();

                    break;
                }
            }

            ImageView::Builder image_view_builder;

            image_view_builder.setAspectMask(VK_IMAGE_ASPECT_COLOR_BIT);
            image_view_builder.setBaseArrayLayer(0);
            image_view_builder.setBaseMipLevel(0);
            image_view_builder.setFormat(VK_FORMAT_R32G32B32_SFLOAT);
            image_view_builder.setImage(ptr_image);
            image_view_builder.setLayerCount(1);
            image_view_builder.setLevelCount(1);
            image_view_builder.setType(VK_IMAGE_VIEW_TYPE_2D);

            _textures.insert(make_pair(name, image_view_builder.buildPtr()));
            
            stbi_image_free(data);

            return true;
        }

        return false;
    }

    bool GPUTextureManager::add(const PtrImageView& ptr_image_view, const string_view name)
    {
        if (_textures.find(name.data()) != end(_textures)) {
            return false;
        }
        
        _textures.insert(make_pair(name, ptr_image_view));
        
        return true;
    }

    bool GPUTextureManager::has(const string_view name)
    {
        return _textures.find(name.data()) != end(_textures);
    }

    optional<PtrImageView> GPUTextureManager::get(const string_view name)
    {
        auto it = _textures.find(name.data());
        return it != end(_textures) ? make_optional(it->second) : nullopt;
    }

    optional<const PtrImageView> GPUTextureManager::get(const string_view name) const
    {
        auto it = _textures.find(name.data());
        return it != end(_textures) ? make_optional(it->second) : nullopt;
    }
}
