//
//  Sampler.inl
//  PBRLib
//
//  Created by Асиф Мамедов on 22/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

namespace pbrlib
{
    inline SamplerInfo::SamplerInfo() noexcept :
        VkSamplerCreateInfo {
            .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0
        }
    {}

    inline void SamplerInfo::setMagFilter(VkFilter filter) noexcept
    {
        VkSamplerCreateInfo::magFilter = filter;
    }

    inline void SamplerInfo::setMinFilter(VkFilter filter) noexcept
    {
        VkSamplerCreateInfo::minFilter = filter;
    }

    inline void SamplerInfo::setMipmapMode(VkSamplerMipmapMode mipmap_mode) noexcept
    {
        VkSamplerCreateInfo::mipmapMode = mipmap_mode;
    }

    inline void SamplerInfo::setAdressMode(VkSamplerAddressMode u, VkSamplerAddressMode v, VkSamplerAddressMode w) noexcept
    {
        VkSamplerCreateInfo::addressModeU = u;
        VkSamplerCreateInfo::addressModeV = v;
        VkSamplerCreateInfo::addressModeW = w;
    }

    inline void SamplerInfo::setMipLodBias(float mip_lod_bias) noexcept
    {
        VkSamplerCreateInfo::mipLodBias = mip_lod_bias;
    }

    inline void SamplerInfo::anisotropyEnable(VkBool32 is_enable) noexcept
    {
        VkSamplerCreateInfo::anisotropyEnable = is_enable;
    }

    inline void SamplerInfo::setMaxAnisotropy(float max_anosotropy) noexcept
    {
        VkSamplerCreateInfo::maxAnisotropy = max_anosotropy;
    }

    inline void SamplerInfo::compareEnable(VkBool32 is_enable) noexcept
    {
        VkSamplerCreateInfo::compareEnable = is_enable;
    }

    inline void SamplerInfo::setCompareOp(VkCompareOp compare_op) noexcept
    {
        VkSamplerCreateInfo::compareOp = compare_op;
    }

    inline void SamplerInfo::setLodRange(float min_lod, float max_lod) noexcept
    {
        VkSamplerCreateInfo::minLod = min_lod;
        VkSamplerCreateInfo::maxLod = max_lod;
    }

    inline void SamplerInfo::setBorderColor(VkBorderColor border_color) noexcept
    {
        VkSamplerCreateInfo::borderColor = border_color;
    }

    inline void SamplerInfo::unnormalizedCoordinates(VkBool32 unnormalized_coordinates) noexcept
    {
        VkSamplerCreateInfo::unnormalizedCoordinates = unnormalized_coordinates;
    }

    inline VkFilter SamplerInfo::getMagFilter() const noexcept
    {
        return VkSamplerCreateInfo::magFilter;
    }

    inline VkFilter SamplerInfo::getMinFilter() const noexcept
    {
        return VkSamplerCreateInfo::minFilter;
    }

    inline VkSamplerMipmapMode SamplerInfo::getMipmapMode() const noexcept
    {
        return VkSamplerCreateInfo::mipmapMode;
    }

    inline VkBool32 SamplerInfo::anisotropyEnable() const noexcept
    {
        return VkSamplerCreateInfo::anisotropyEnable;
    }

    inline float SamplerInfo::getMaxAnisotropy() const noexcept
    {
        return VkSamplerCreateInfo::maxAnisotropy;
    }

    inline VkBool32 SamplerInfo::compareEnable() const noexcept
    {
        return VkSamplerCreateInfo::compareEnable;
    }

    inline VkCompareOp SamplerInfo::getCompareOp() const noexcept
    {
        return VkSamplerCreateInfo::compareOp;
    }

    inline float SamplerInfo::getMinLod() const noexcept
    {
        return VkSamplerCreateInfo::minLod;
    }

    inline float SamplerInfo::getMaxLod() const noexcept
    {
        return VkSamplerCreateInfo::maxLod;
    }

    inline VkBorderColor SamplerInfo::getBorderColor() const noexcept
    {
        return VkSamplerCreateInfo::borderColor;
    }

    inline VkBool32 SamplerInfo::unnormalizedCoordinates() const noexcept
    {
        return VkSamplerCreateInfo::unnormalizedCoordinates;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   inline Sampler::Sampler(const shared_ptr<Device>& ptr_device, const SamplerInfo& sampler_info) :
        _ptr_device(ptr_device),
        _sampler_info(sampler_info),
        _sampler_handle(VK_NULL_HANDLE)
    {
        assert(vkCreateSampler(_ptr_device->getDeviceHandle(),
                               reinterpret_cast<VkSamplerCreateInfo*>(&_sampler_info),
                               nullptr,
                               &_sampler_handle) == VK_SUCCESS);
        assert(_sampler_handle != VK_NULL_HANDLE);
    }

    inline Sampler::Sampler(Sampler&& sampler) :
        _ptr_device(move(sampler._ptr_device)),
        _sampler_info(sampler._sampler_info),
        _sampler_handle(VK_NULL_HANDLE)
    {
        swap(_sampler_handle, sampler._sampler_handle);
    }

    inline Sampler::~Sampler()
    {
        if (_sampler_handle != VK_NULL_HANDLE) {
            vkDestroySampler(_ptr_device->getDeviceHandle(), _sampler_handle, nullptr);
        }
    }

    inline shared_ptr<Device>& Sampler::getDevice() noexcept
    {
        return _ptr_device;
    }

    inline const shared_ptr<Device>& Sampler::getDevice() const noexcept
    {
        return _ptr_device;
    }

    inline SamplerInfo& Sampler::getSamplerInfo() noexcept
    {
        return _sampler_info;
    }

    inline const SamplerInfo& Sampler::getSamplerInfo() const noexcept
    {
        return _sampler_info;
    }

    inline VkSampler Sampler::getSamplerHandle() const noexcept
    {
        return _sampler_handle;
    }
}