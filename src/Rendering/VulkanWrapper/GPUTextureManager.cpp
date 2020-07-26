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

#include "CommandBuffer.hpp"

#include <stb/stb_image.h>

namespace pbrlib
{
    GPUTextureManager::GPUTextureManager(
        const PtrDevice&        ptr_device,
        const PtrCommandPool&   ptr_command_pool,
        uint32_t                device_local_memory_type_index,
        uint32_t                host_local_memory_type_index,
        PtrDeviceQueue          ptr_device_queue,
        VkImageTiling           image_tiling,
        VkSampleCountFlags      num_samples
    ) :
        _ptr_device                     (ptr_device),
        _ptr_command_buffer             (PrimaryCommandBuffer::make(ptr_command_pool)),
        _device_local_memory_type_index (device_local_memory_type_index),
        _host_local_memory_type_index   (host_local_memory_type_index),
        _ptr_device_queue               (ptr_device_queue),
        _image_tiling                   (image_tiling),
        _num_samples                    (num_samples)
    {}

    bool GPUTextureManager::loadR(const string_view path, const string_view name)
    {
        if (_textures.find(name.data()) != end(_textures)) {
            return true;
        }

        int width           = 0;
        int height          = 0;
        int num_channels    = 0;

        auto* data = stbi_loadf(path.data(), &width, &height, &num_channels, STBI_grey);

        if (data && width && height) {
            using ImageBuilder = Image::BuilderWithData<Image::TexelType::R, float, Image::NumBits::NB32>;

            PtrImage            ptr_image;
            ImageBuilder        image_builder;
            ImageView::Builder  image_view_builder;

            image_builder.setExtend(width, height, 1);
            image_builder.setData(data, width * height);
            image_builder.setDevice(_ptr_device);
            image_builder.setImageType(VK_IMAGE_TYPE_2D);
            image_builder.setHostLocalMemoryTypeIndex(_host_local_memory_type_index);
            image_builder.setDeviceLocalMemoryTypeIndex(_device_local_memory_type_index);
            image_builder.setCommandBuffer(_ptr_command_buffer);
            image_builder.setDeviceQueue(_ptr_device_queue);
            image_builder.setNumArrayLayers(1);
            image_builder.setNumMipLevels(1);
            image_builder.setNumSamples(VK_SAMPLE_COUNT_1_BIT);
            image_builder.setTiling(_image_tiling);
            image_builder.setUsage(VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT);

            ptr_image = image_builder.buildPtr();

            image_view_builder.setAspectMask(VK_IMAGE_ASPECT_COLOR_BIT);
            image_view_builder.setBaseArrayLayer(0);
            image_view_builder.setBaseMipLevel(0);
            image_view_builder.setFormat(ptr_image->getImageInfo().format);
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

    bool GPUTextureManager::loadRG(const string_view path, const string_view name)
    {
        if (_textures.find(name.data()) != end(_textures)) {
            return true;
        }

        int width           = 0;
        int height          = 0;
        int num_channels    = 0;

        auto* data = stbi_loadf(path.data(), &width, &height, &num_channels, STBI_grey_alpha);

        if (data && width && height) {
            using ImageBuilder = Image::BuilderWithData<Image::TexelType::RG, float, Image::NumBits::NB32>;

            PtrImage            ptr_image;
            ImageBuilder        image_builder;
            ImageView::Builder  image_view_builder;

            image_builder.setExtend(width, height, 1);
            image_builder.setData(data, width * height * 2);
            image_builder.setDevice(_ptr_device);
            image_builder.setImageType(VK_IMAGE_TYPE_2D);
            image_builder.setHostLocalMemoryTypeIndex(_host_local_memory_type_index);
            image_builder.setDeviceLocalMemoryTypeIndex(_device_local_memory_type_index);
            image_builder.setCommandBuffer(_ptr_command_buffer);
            image_builder.setDeviceQueue(_ptr_device_queue);
            image_builder.setNumArrayLayers(1);
            image_builder.setNumMipLevels(1);
            image_builder.setNumSamples(VK_SAMPLE_COUNT_1_BIT);
            image_builder.setTiling(_image_tiling);
            image_builder.setUsage(VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT);

            ptr_image = image_builder.buildPtr();

            image_view_builder.setAspectMask(VK_IMAGE_ASPECT_COLOR_BIT);
            image_view_builder.setBaseArrayLayer(0);
            image_view_builder.setBaseMipLevel(0);
            image_view_builder.setFormat(ptr_image->getImageInfo().format);
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

    bool GPUTextureManager::loadRGB(const string_view path, const string_view name)
    {
        if (_textures.find(name.data()) != end(_textures)) {
            return true;
        }

        int width           = 0;
        int height          = 0;
        int num_channels    = 0;

        auto* data = stbi_loadf(path.data(), &width, &height, &num_channels, STBI_rgb);

        if (data && width && height) {
            using ImageBuilder = Image::BuilderWithData<Image::TexelType::RGB, float, Image::NumBits::NB32>;
        
            PtrImage            ptr_image;
            ImageBuilder        image_builder;
            ImageView::Builder  image_view_builder;

            image_builder.setExtend(width, height, 1);
            image_builder.setData(data, width * height * 3);
            image_builder.setDevice(_ptr_device);
            image_builder.setImageType(VK_IMAGE_TYPE_2D);
            image_builder.setHostLocalMemoryTypeIndex(_host_local_memory_type_index);
            image_builder.setDeviceLocalMemoryTypeIndex(_device_local_memory_type_index);
            image_builder.setCommandBuffer(_ptr_command_buffer);
            image_builder.setDeviceQueue(_ptr_device_queue);
            image_builder.setNumArrayLayers(1);
            image_builder.setNumMipLevels(1);
            image_builder.setNumSamples(VK_SAMPLE_COUNT_1_BIT);
            image_builder.setTiling(_image_tiling);
            image_builder.setUsage(VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT);

            ptr_image = image_builder.buildPtr();

            image_view_builder.setAspectMask(VK_IMAGE_ASPECT_COLOR_BIT);
            image_view_builder.setBaseArrayLayer(0);
            image_view_builder.setBaseMipLevel(0);
            image_view_builder.setFormat(ptr_image->getImageInfo().format);
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

    bool GPUTextureManager::loadRGBA(const string_view path, const string_view name)
    {
        if (_textures.find(name.data()) != end(_textures)) {
            return true;
        }

        int width           = 0;
        int height          = 0;
        int num_channels    = 0;

        auto* data = stbi_loadf(path.data(), &width, &height, &num_channels, STBI_rgb_alpha);

        if (data && width && height) {
            using ImageBuilder = Image::BuilderWithData<Image::TexelType::RGBA, float, Image::NumBits::NB32>;

            PtrImage            ptr_image;
            ImageBuilder        image_builder;
            ImageView::Builder  image_view_builder;

            image_builder.setExtend(width, height, 1);
            image_builder.setData(data, width * height * 4);
            image_builder.setDevice(_ptr_device);
            image_builder.setImageType(VK_IMAGE_TYPE_2D);
            image_builder.setHostLocalMemoryTypeIndex(_host_local_memory_type_index);
            image_builder.setDeviceLocalMemoryTypeIndex(_device_local_memory_type_index);
            image_builder.setCommandBuffer(_ptr_command_buffer);
            image_builder.setDeviceQueue(_ptr_device_queue);
            image_builder.setNumArrayLayers(1);
            image_builder.setNumMipLevels(1);
            image_builder.setNumSamples(VK_SAMPLE_COUNT_1_BIT);
            image_builder.setTiling(_image_tiling);
            image_builder.setUsage(VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT);

            ptr_image = image_builder.buildPtr();

            image_view_builder.setAspectMask(VK_IMAGE_ASPECT_COLOR_BIT);
            image_view_builder.setBaseArrayLayer(0);
            image_view_builder.setBaseMipLevel(0);
            image_view_builder.setFormat(ptr_image->getImageInfo().format);
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
