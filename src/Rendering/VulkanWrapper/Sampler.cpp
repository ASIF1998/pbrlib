//
//  Sampler.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 22/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <pbrlib/Rendering/VulkanWrapper/Sampler.hpp>

#include <cassert>

namespace pbrlib
{
    SamplerInfo::SamplerInfo() noexcept 
    {
        _sampler_ci = { };
        _sampler_ci.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    }

    void SamplerInfo::setMagFilter(VkFilter filter) noexcept
    {
        _sampler_ci.magFilter = filter;
    }

    void SamplerInfo::setMinFilter(VkFilter filter) noexcept
    {
        _sampler_ci.minFilter = filter;
    }

    void SamplerInfo::setMipmapMode(VkSamplerMipmapMode mipmap_mode) noexcept
    {
        _sampler_ci.mipmapMode = mipmap_mode;
    }

    void SamplerInfo::setAdressMode(
        VkSamplerAddressMode u, 
        VkSamplerAddressMode v, 
        VkSamplerAddressMode w
    ) noexcept
    {
        _sampler_ci.addressModeU = u;
        _sampler_ci.addressModeV = v;    
        _sampler_ci.addressModeW = w;    
    }

    void SamplerInfo::setMipLodBias(float mip_lod_bias) noexcept
    {
        _sampler_ci.mipLodBias = mip_lod_bias;
    }

    void SamplerInfo::anisotropyEnable(VkBool32 is_enable) noexcept
    {
        _sampler_ci.anisotropyEnable = is_enable;
    }

    void SamplerInfo::setMaxAnisotropy(float max_anisotropy) noexcept
    {
        _sampler_ci.maxAnisotropy = max_anisotropy;
    }

    void SamplerInfo::compareEnable(VkBool32 is_enable) noexcept
    {
        _sampler_ci.compareEnable = is_enable;
    }

    void SamplerInfo::setCompareOp(VkCompareOp compare_op) noexcept
    {
        _sampler_ci.compareOp = compare_op;
    }

    void SamplerInfo::setLodRange(float min_lod, float max_lod) noexcept
    {
        _sampler_ci.minLod = min_lod;
        _sampler_ci.maxLod = max_lod;        
    }

    void SamplerInfo::setBorderColor(VkBorderColor border_color) noexcept
    {
        _sampler_ci.borderColor = border_color;
    }

    void SamplerInfo::unnormalizedCoordinates(VkBool32 unnormalized_coordinates) noexcept
    {
        _sampler_ci.unnormalizedCoordinates = unnormalized_coordinates;
    }

    const VkSamplerCreateInfo& SamplerInfo::getSamplerCreateInfo() const noexcept
    {
        return _sampler_ci;
    }

    VkSamplerCreateInfo& SamplerInfo::getSamplerCreateInfo() noexcept
    {
        return _sampler_ci;
    }

    VkFilter SamplerInfo::getMagFilter() const noexcept
    {
        return _sampler_ci.magFilter;
    }

    VkFilter SamplerInfo::getMinFilter() const noexcept
    {
        return _sampler_ci.minFilter;
    }

    VkSamplerMipmapMode SamplerInfo::getMipmapMode() const noexcept
    {
        return _sampler_ci.mipmapMode;
    }

    VkBool32 SamplerInfo::anisotropyEnable() const noexcept
    {
        return _sampler_ci.anisotropyEnable;
    }

    float SamplerInfo::getMaxAnisotropy() const noexcept
    {
        return _sampler_ci.maxAnisotropy;
    }

    VkBool32 SamplerInfo::compareEnable() const noexcept
    {
        return _sampler_ci.compareEnable;
    }

    VkCompareOp SamplerInfo::getCompareOp() const noexcept
    {
        return _sampler_ci.compareOp;
    }

    float SamplerInfo::getMinLod() const noexcept
    {
        return _sampler_ci.minLod;
    }

    float SamplerInfo::getMaxLod() const noexcept
    {
        return _sampler_ci.maxLod;
    }

    VkBorderColor SamplerInfo::getBorderColor() const noexcept
    {
        return _sampler_ci.borderColor;
    }

    VkBool32 SamplerInfo::unnormalizedCoordinates() const noexcept
    {
        return _sampler_ci.unnormalizedCoordinates;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Sampler::Sampler(const PtrDevice& ptr_device, const SamplerInfo& sampler_info) :
        _ptr_device     (ptr_device),
        _sampler_info   (sampler_info),
        _sampler_handle (VK_NULL_HANDLE)
    {
        assert(vkCreateSampler(
            _ptr_device->getDeviceHandle(),
            &_sampler_info.getSamplerCreateInfo(),
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
