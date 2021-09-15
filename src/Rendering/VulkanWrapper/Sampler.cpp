//
//  Sampler.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 22/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include "Sampler.hpp"

#include <cassert>

namespace pbrlib
{
    SamplerInfo::SamplerInfo() noexcept :
        VkSamplerCreateInfo {
            .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0
        }
    {}

    void SamplerInfo::setMagFilter(VkFilter filter) noexcept
    {
        VkSamplerCreateInfo::magFilter = filter;
    }

    void SamplerInfo::setMinFilter(VkFilter filter) noexcept
    {
        VkSamplerCreateInfo::minFilter = filter;
    }

    void SamplerInfo::setMipmapMode(VkSamplerMipmapMode mipmap_mode) noexcept
    {
        VkSamplerCreateInfo::mipmapMode = mipmap_mode;
    }

    void SamplerInfo::setAdressMode(
        VkSamplerAddressMode u, 
        VkSamplerAddressMode v, 
        VkSamplerAddressMode w
    ) noexcept
    {
        VkSamplerCreateInfo::addressModeU = u;
        VkSamplerCreateInfo::addressModeV = v;
        VkSamplerCreateInfo::addressModeW = w;
    }

    void SamplerInfo::setMipLodBias(float mip_lod_bias) noexcept
    {
        VkSamplerCreateInfo::mipLodBias = mip_lod_bias;
    }

    void SamplerInfo::anisotropyEnable(VkBool32 is_enable) noexcept
    {
        VkSamplerCreateInfo::anisotropyEnable = is_enable;
    }

    void SamplerInfo::setMaxAnisotropy(float max_anosotropy) noexcept
    {
        VkSamplerCreateInfo::maxAnisotropy = max_anosotropy;
    }

    void SamplerInfo::compareEnable(VkBool32 is_enable) noexcept
    {
        VkSamplerCreateInfo::compareEnable = is_enable;
    }

    void SamplerInfo::setCompareOp(VkCompareOp compare_op) noexcept
    {
        VkSamplerCreateInfo::compareOp = compare_op;
    }

    void SamplerInfo::setLodRange(float min_lod, float max_lod) noexcept
    {
        VkSamplerCreateInfo::minLod = min_lod;
        VkSamplerCreateInfo::maxLod = max_lod;
    }

    void SamplerInfo::setBorderColor(VkBorderColor border_color) noexcept
    {
        VkSamplerCreateInfo::borderColor = border_color;
    }

    void SamplerInfo::unnormalizedCoordinates(VkBool32 unnormalized_coordinates) noexcept
    {
        VkSamplerCreateInfo::unnormalizedCoordinates = unnormalized_coordinates;
    }

    VkFilter SamplerInfo::getMagFilter() const noexcept
    {
        return VkSamplerCreateInfo::magFilter;
    }

    VkFilter SamplerInfo::getMinFilter() const noexcept
    {
        return VkSamplerCreateInfo::minFilter;
    }

    VkSamplerMipmapMode SamplerInfo::getMipmapMode() const noexcept
    {
        return VkSamplerCreateInfo::mipmapMode;
    }

    VkBool32 SamplerInfo::anisotropyEnable() const noexcept
    {
        return VkSamplerCreateInfo::anisotropyEnable;
    }

    float SamplerInfo::getMaxAnisotropy() const noexcept
    {
        return VkSamplerCreateInfo::maxAnisotropy;
    }

    VkBool32 SamplerInfo::compareEnable() const noexcept
    {
        return VkSamplerCreateInfo::compareEnable;
    }

    VkCompareOp SamplerInfo::getCompareOp() const noexcept
    {
        return VkSamplerCreateInfo::compareOp;
    }

    float SamplerInfo::getMinLod() const noexcept
    {
        return VkSamplerCreateInfo::minLod;
    }

    float SamplerInfo::getMaxLod() const noexcept
    {
        return VkSamplerCreateInfo::maxLod;
    }

    VkBorderColor SamplerInfo::getBorderColor() const noexcept
    {
        return VkSamplerCreateInfo::borderColor;
    }

    VkBool32 SamplerInfo::unnormalizedCoordinates() const noexcept
    {
        return VkSamplerCreateInfo::unnormalizedCoordinates;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Sampler::Sampler(const PtrDevice& ptr_device, const SamplerInfo& sampler_info) :
        _ptr_device     (ptr_device),
        _sampler_info   (sampler_info),
        _sampler_handle (VK_NULL_HANDLE)
    {
        assert(vkCreateSampler(
            _ptr_device->getDeviceHandle(),
            reinterpret_cast<VkSamplerCreateInfo*>(&_sampler_info),
            nullptr,
            &_sampler_handle
        ) == VK_SUCCESS);

        assert(_sampler_handle != VK_NULL_HANDLE);
    }

    Sampler::Sampler(Sampler&& sampler) :
        _ptr_device     (move(sampler._ptr_device)),
        _sampler_info   (sampler._sampler_info),
        _sampler_handle (VK_NULL_HANDLE)
    {
        swap(_sampler_handle, sampler._sampler_handle);
    }

    Sampler::~Sampler()
    {
        if (_sampler_handle != VK_NULL_HANDLE) {
            vkDestroySampler(_ptr_device->getDeviceHandle(), _sampler_handle, nullptr);
        }
    }

    PtrDevice& Sampler::getDevice() noexcept
    {
        return _ptr_device;
    }

    const PtrDevice& Sampler::getDevice() const noexcept
    {
        return _ptr_device;
    }

    SamplerInfo& Sampler::getSamplerInfo() noexcept
    {
        return _sampler_info;
    }

    const SamplerInfo& Sampler::getSamplerInfo() const noexcept
    {
        return _sampler_info;
    }

    const VkSampler& Sampler::getSamplerHandle() const noexcept
    {
        return _sampler_handle;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Sampler::Builder::Builder() :
        SamplerInfo()
    {}

    void Sampler::Builder::setDevice(const PtrDevice& ptr_device)
    {
        _ptr_device = ptr_device;
    }

    Sampler Sampler::Builder::build() const
    {
        return Sampler(
            _ptr_device,
            *this
        );
    }

    PtrSampler Sampler::Builder::buildPtr() const
    {
        return make_shared<Sampler>(
            _ptr_device,
            *this
        );
    }
}
