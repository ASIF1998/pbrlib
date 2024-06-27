//
//  PBRPass.inl
//  PBRLib
//
//  Created by Асиф Мамедов on 19/08/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include "../../VulkanWrapper/DescriptorSet.hpp"

namespace pbrlib
{
    inline PBRPass::Optionals::Optionals(
        DistributionFunction    distrib_func,
        GeometryFunction        geom_func,
        FresnelApproximation    fresnel_approx,
        bool                    use_bent_normal
    ) :
        _distrib_func   (distrib_func),
        _geom_func      (geom_func),
        _fresnel_approx (fresnel_approx)
    {
        _other_options.use_bent_normal_tangent_btangent = use_bent_normal;
    }

    inline void PBRPass::Optionals::setDistributionFunction(DistributionFunction dist_func) noexcept
    {
        _distrib_func = dist_func;
    }

    inline void PBRPass::Optionals::setGeometryFunction(GeometryFunction geom_func) noexcept
    {
        _geom_func = geom_func;
    }

    inline void PBRPass::Optionals::setFresnelApproximation(FresnelApproximation fresnel_approx) noexcept
    {
        _fresnel_approx = fresnel_approx;
    }

    inline void PBRPass::Optionals::useBentNormalTangentBtangent(bool to_use) noexcept
    {
        _other_options.use_bent_normal_tangent_btangent = to_use;
    }

    inline PBRPass::DistributionFunction PBRPass::Optionals::getDistributionFunction() const noexcept
    {
        return _distrib_func;
    }

    inline PBRPass::GeometryFunction PBRPass::Optionals::getGeometryFunction() const noexcept
    {
        return _geom_func;
    }

    inline PBRPass::FresnelApproximation PBRPass::Optionals::getFresnelApproximation() const noexcept
    {
        return _fresnel_approx;
    }

    inline bool PBRPass::Optionals::useBentNormalTangentBtangent() const noexcept
    {
        return _other_options.use_bent_normal_tangent_btangent;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inline void PBRPass::Builder::setDevice(const Device* ptr_device)
    {
        _ptr_device = ptr_device;
    }

    inline void PBRPass::Builder::setPhysicalDevice(const PhysicalDevice* ptr_physical_device)
    {
        _ptr_physical_deviec = ptr_physical_device;
    }

    inline void PBRPass::Builder::setDescriptorPool(std::shared_ptr<const DescriptorPool> ptr_descriptor_pool)
    {
        _ptr_descriptor_pool = ptr_descriptor_pool;
    }

    inline void PBRPass::Builder::setPositionAndMetallicImageView(const ImageView* position_and_metallic_image_view)
    {
        assert(position_and_metallic_image_view);
        _position_and_metallic_image_view = position_and_metallic_image_view;
    }

    inline void PBRPass::Builder::setNormalAndRoughnessImageView(const ImageView* normal_and_roughness_image_view)
    {
        assert(normal_and_roughness_image_view);
        _normal_and_roughness_image_view = normal_and_roughness_image_view;
    }

    inline void PBRPass::Builder::setAlbedoAndBakedAOImageView(const ImageView* albedo_and_baked_AO_image_view)
    {
        assert(albedo_and_baked_AO_image_view);
        _albedo_and_baked_AO_image_view = albedo_and_baked_AO_image_view;
    }

    inline void PBRPass::Builder::setAnisotropyImageView(const ImageView* anisotropy_image_view)
    {
        assert(anisotropy_image_view);
        _anisotropy_image_view = anisotropy_image_view;
    }

    inline void PBRPass::Builder::setSampler(std::shared_ptr<const Sampler> ptr_sampler)
    {
        _ptr_sampler = ptr_sampler;
    }

    inline void PBRPass::Builder::setOptionals(const Optionals& optionals)
    {
        _optionals = optionals;
    }

    inline void PBRPass::Builder::setQueue(std::shared_ptr<DeviceQueue> ptr_queue)
    {
        _ptr_device_queue = ptr_queue;
    }

    inline PBRPass PBRPass::Builder::build()
    {
        if (_position_and_metallic_image_view && _normal_and_roughness_image_view && _albedo_and_baked_AO_image_view && _anisotropy_image_view) {
            return PBRPass(
                _ptr_device,
                _ptr_physical_deviec,
                _ptr_device_queue,
                _ptr_descriptor_pool,
                *_position_and_metallic_image_view,
                *_normal_and_roughness_image_view,
                *_albedo_and_baked_AO_image_view,
                *_anisotropy_image_view,
                _ptr_sampler,
                _optionals
            );
        }

        return PBRPass(
            _ptr_device,
            _ptr_physical_deviec,
            _ptr_device_queue,
            _ptr_descriptor_pool,
            _ptr_sampler,
            _optionals
        );
    }

    inline std::unique_ptr<PBRPass> PBRPass::Builder::buildPtr()
    {
        if (_position_and_metallic_image_view && _normal_and_roughness_image_view && _albedo_and_baked_AO_image_view && _anisotropy_image_view) {
            return std::make_unique<PBRPass>(
                _ptr_device,
                _ptr_physical_deviec,
                _ptr_device_queue,
                _ptr_descriptor_pool,
                *_position_and_metallic_image_view,
                *_normal_and_roughness_image_view,
                *_albedo_and_baked_AO_image_view,
                *_anisotropy_image_view,
                _ptr_sampler,
                _optionals
            );
        }

        return std::make_unique<PBRPass>(
            _ptr_device,
            _ptr_physical_deviec,
            _ptr_device_queue,
            _ptr_descriptor_pool,
            _ptr_sampler,
            _optionals
        );
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inline std::shared_ptr<const ComputePipeline> PBRPass::getPipeline() const noexcept
    {
        return _ptr_pipeline;
    }

    inline const ImageView& PBRPass::outputImpl(size_t id) const
    {
        assert(_out_image_view && id < OutputImagesViewsIDs::Count);
        return *_out_image_view;
    }

    inline void PBRPass::outputImpl(const ImageView& image_view, size_t id)
    {
        assert(id < OutputImagesViewsIDs::Count);
        _out_image_view = &image_view;
    }

    inline void PBRPass::outputImpl(std::shared_ptr<const ImageView> ptr_image_view, size_t id)
    {
        assert(ptr_image_view && id < OutputImagesViewsIDs::Count);
        _out_image_view = ptr_image_view.get();
    }

    inline void PBRPass::inputImpl(std::shared_ptr<const ImageView> ptr_image_view, size_t id)
    {
        assert(id < InputImagesViewsIDs::Count);
        _ptr_images_views[id] = ptr_image_view.get();

        _ptr_descriptor_set->writeImageView(
            *_ptr_images_views[id],
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
            *_ptr_sampler,
            static_cast<uint32_t>(id),
            VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER
        );
    }

    inline std::unique_ptr<PBRPass> PBRPass::make(
        const Device*                           ptr_device, 
        const PhysicalDevice*                   ptr_physical_deviec,
        std::shared_ptr<DeviceQueue>            ptr_queue,
        std::shared_ptr<const DescriptorPool>   ptr_descriptor_pool,
        std::shared_ptr<const Sampler>          ptr_sampler,
        const Optionals&                        optionals
    )
    {
        return std::make_unique<PBRPass>(
            ptr_device,
            ptr_physical_deviec,
            ptr_queue,
            ptr_descriptor_pool,
            ptr_sampler,
            optionals
        );
    }

    inline std::unique_ptr<PBRPass> PBRPass::make(
        const Device*                           ptr_device, 
        const PhysicalDevice*                   ptr_physical_deviec,
        std::shared_ptr<DeviceQueue>            ptr_queue,
        std::shared_ptr<const DescriptorPool>   ptr_descriptor_pool,
        const ImageView&                        position_and_metallic_image_view,
        const ImageView&                        normal_and_roughness_image_view,
        const ImageView&                        albedo_and_baked_AO_image_view,
        const ImageView&                        anisotropy_image_view,
        std::shared_ptr<const Sampler>          ptr_sampler,
        const Optionals&                        optionals
    )
    {
        return std::make_unique<PBRPass>(
            ptr_device,
            ptr_physical_deviec,
            ptr_queue,
            ptr_descriptor_pool,
            position_and_metallic_image_view,
            normal_and_roughness_image_view,
            albedo_and_baked_AO_image_view,
            anisotropy_image_view,
            ptr_sampler,
            optionals
        );
    }
}
