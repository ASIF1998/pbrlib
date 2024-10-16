//
//  PBRPass.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 19/07/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef PBRPass_hpp
#define PBRPass_hpp

#include <pbrlib/scene/Scene.hpp>
#include <pbrlib/Rendering/Renderers/SubPasses/Pass.hpp>

#include <memory>
#include <span>

namespace pbrlib
{
    class DescriptorPool;
}

namespace pbrlib
{
    /**
     * @class PBRPass.
     * @brief Данный класс необходим для прохода физически-корректного рендеринга.
    */
    class PBRPass : 
        public IPassInputOutput<PBRPass>
    {
    public:
        enum class DistributionFunction :
            uint32_t
        {
            Beckmann = 0,
            GGX,
            GGX_Anisotropy
        };

        enum class GeometryFunction :
            uint32_t 
        {
            Beckmann = 0,
            GGX,
            SchlickBeckmann,
            SchlickGGX,
            Implicit,
            Neumann,
            CookTorrance,
            Kelemen,
            Smith
        };

        enum class FresnelApproximation :
            uint32_t
        {
            None,
            Schlick,
            CookTorrance
        };

        struct InputImagesViewsIDs
        {
            enum :
                size_t
            {
                PositionAndMetallic = 0,
                NormalAndRoughness,
                AlbedoAndBaked,
                Anisotropy,

                Count
            };
        };

        struct OutputImagesViewsIDs
        {
            enum :
                size_t
            {
                Result = 0,

                Count
            };
        };

        class Optionals
        {
            friend class PBRPass;
            
        public:
            inline Optionals(
                DistributionFunction    distrib_func    = DistributionFunction::GGX,
                GeometryFunction        geom_func       = GeometryFunction::Smith,
                FresnelApproximation    fresnel_approx  = FresnelApproximation::Schlick,
                bool                    use_bent_normal = false
            );

            inline void setDistributionFunction(DistributionFunction dist_func)        noexcept;
            inline void setGeometryFunction(GeometryFunction geom_func)                noexcept;
            inline void setFresnelApproximation(FresnelApproximation fresnel_approx)   noexcept;
            inline void useBentNormalTangentBtangent(bool to_use)                      noexcept;

            inline DistributionFunction     getDistributionFunction()       const noexcept;
            inline GeometryFunction         getGeometryFunction()           const noexcept;
            inline FresnelApproximation     getFresnelApproximation()       const noexcept;
            inline bool                     useBentNormalTangentBtangent()  const noexcept;

        private:
            DistributionFunction    _distrib_func;
            GeometryFunction        _geom_func;
            FresnelApproximation    _fresnel_approx;

            union
            {
                struct
                {
                    bool        use_bent_normal_tangent_btangent    :  1;
                    uint32_t    other                               :  31;
                };

                uint32_t mask;

            } _other_options;
        };

        class Builder
        {
        public:
            Builder() = default;

            Builder(Builder&&)      = delete;
            Builder(const Builder&) = delete;

            Builder& operator = (Builder&&)         = delete;
            Builder& operator = (const Builder&)    = delete;

            inline void setDevice(const Device* ptr_device);
            inline void setPhysicalDevice(const PhysicalDevice* ptr_physical_device);
            inline void setDescriptorPool(std::shared_ptr<const DescriptorPool> ptr_descriptor_pool);
            inline void setPositionAndMetallicImageView(const ImageView* position_and_metallic_image_view);
            inline void setNormalAndRoughnessImageView(const ImageView* normal_and_roughness_image_view);
            inline void setAlbedoAndBakedAOImageView(const ImageView* albedo_and_baked_AO_image_view);
            inline void setAnisotropyImageView(const ImageView* anisotropy_image_view);
            inline void setSampler(std::shared_ptr<const Sampler> ptr_sampler);
            inline void setOptionals(const Optionals& optionals);
            inline void setQueue(std::shared_ptr<DeviceQueue> ptr_queue);

            inline PBRPass                  build();
            inline std::unique_ptr<PBRPass> buildPtr();

        private:
            const Device*                           _ptr_device;
            const PhysicalDevice*                   _ptr_physical_deviec;
            std::shared_ptr<const DescriptorPool>   _ptr_descriptor_pool;
            const ImageView*                        _position_and_metallic_image_view;
            const ImageView*                        _normal_and_roughness_image_view;
            const ImageView*                        _albedo_and_baked_AO_image_view;
            const ImageView*                        _anisotropy_image_view;
            std::shared_ptr<const Sampler>          _ptr_sampler;
            Optionals                               _optionals;
            std::shared_ptr<DeviceQueue>            _ptr_device_queue;
        };

    public:
        /**
         * @brief Конструктор.
         *
         * @param ptr_device            указатель на логическое устройство.
         * @param ptr_physical_deviec   указатель на физическое устройство.
         * @param ptr_queue             указатель на очередь.
         * @param ptr_descriptor_pool   указатель на пул дескрипторов.
         * @param ptr_sampler           указатель на сэмплер.
         * @param optionals             опции рендера.
        */
        PBRPass(
            const Device*                           ptr_device, 
            const PhysicalDevice*                   ptr_physical_deviec,
            std::shared_ptr<DeviceQueue>            ptr_queue,
            std::shared_ptr<const DescriptorPool>   ptr_descriptor_pool,
            std::shared_ptr<const Sampler>          ptr_sampler,
            const Optionals&                        optionals = Optionals()
        );

        /**
         * @brief Конструктор.
         *
         * @param ptr_device                        указатель на логическое устройство.
         * @param ptr_physical_deviec               указатель на физическое устройство.
         * @param ptr_queue                         указатель на очередь.
         * @param ptr_descriptor_pool               указатель на пул дескрипторов.
         * @param position_and_metallic_image_view  указатель на вид изображения с позициями и металличностью.
         * @param normal_and_roughness_image_view   указатель на вид изображения с нормалями и шероховатостью.
         * @param albedo_and_baked_AO_image_view    указатель на вид изображения с альбедо и запечённым AO.
         * @param anisotropy_image_view             указатель на вид изображения с анизотропностью.
         * @param ptr_sampler                       указатель на сэмплер.
         * @param optionals                         опции рендера.
        */
        PBRPass(
            const Device*                           ptr_device, 
            const PhysicalDevice*                   ptr_physical_deviec,
            std::shared_ptr<DeviceQueue>            ptr_queue,
            std::shared_ptr<const DescriptorPool>   ptr_descriptor_pool,
            const ImageView&                        position_and_metallic_image_view,
            const ImageView&                        normal_and_roughness_image_view,
            const ImageView&                        albedo_and_baked_AO_image_view,
            const ImageView&                        anisotropy_image_view,
            std::shared_ptr<const Sampler>          ptr_sampler,
            const Optionals&                        optionals = Optionals()
        );

        /**
         * @brief Метод, позволяющий запустить проход физически-корректного рендеринга.
         * 
         * @param ptr_camera            указатель на камеру.
         * @param ptr_command_buffer    указатель на командный буфер.
         * @param point_lights          точечные источники света.
         * @param spot_lights           прожекторные источники света.
         * @param direction_lights      направленные источники света.
        */
        void draw(
            std::shared_ptr<const SceneItem>            ptr_camera,
            std::shared_ptr<const PrimaryCommandBuffer> ptr_command_buffer,
            std::span<std::shared_ptr<SceneItem>>       point_lights,
            std::span<std::shared_ptr<SceneItem>>       spot_lights,
            std::span<std::shared_ptr<SceneItem>>       direction_lights
        );

        inline std::shared_ptr<const ComputePipeline> getPipeline() const noexcept;

        /**
         * @brief Метод, позволяющий создавать указатель на объект типа PBRPass.
         * 
         * @param ptr_device            указатель на логическое устройство.
         * @param ptr_physical_deviec   указатель на физическое устройство.
         * @param ptr_queue             указатель на очередь.
         * @param ptr_descriptor_pool   указатель на пул дескрипторов.
         * @param ptr_sampler           указатель на сэмплер.
         * @param optionals             опции рендера.
        */
        inline static std::unique_ptr<PBRPass> make(
            const Device*                           ptr_device, 
            const PhysicalDevice*                   ptr_physical_deviec,
            std::shared_ptr<DeviceQueue>            ptr_queue,
            std::shared_ptr<const DescriptorPool>   ptr_descriptor_pool,
            std::shared_ptr<const Sampler>          ptr_sampler,
            const Optionals&                        optionals = Optionals()
        );

        /**
         * @brief Метод, позволяющий создавать указатель на объект типа PBRPass.
         * 
         * @param ptr_device                        указатель на логическое устройство.
         * @param ptr_physical_deviec               указатель на физическое устройство.
         * @param ptr_queue                         указатель на очередь.
         * @param ptr_descriptor_pool               указатель на пул дескрипторов.
         * @param position_and_metallic_image_view  указатель на вид изображения с позициями и металличностью.
         * @param normal_and_roughness_image_view   указатель на вид изображения с нормалями и шероховатостью.
         * @param albedo_and_baked_AO_image_view    указатель на вид изображения с альбедо и запечённым AO.
         * @param anisotropy_image_view             указатель на вид изображения с анизотропностью.
         * @param ptr_sampler                       указатель на сэмплер.
         * @param optionals                         опции рендера.
        */
        inline static std::unique_ptr<PBRPass> make(
            const Device*                           ptr_device, 
            const PhysicalDevice*                   ptr_physical_deviec,
            std::shared_ptr<DeviceQueue>            ptr_queue,
            std::shared_ptr<const DescriptorPool>   ptr_descriptor_pool,
            const ImageView&                        position_and_metallic_image_view,
            const ImageView&                        normal_and_roughness_image_view,
            const ImageView&                        albedo_and_baked_AO_image_view,
            const ImageView&                        anisotropy_image_view,
            std::shared_ptr<const Sampler>          ptr_sampler,
            const Optionals&                        optionals = Optionals()
        );

    private:
        friend class IPassInputOutput<PBRPass>;
        friend class IPassOutput<PBRPass>;
        
        inline const ImageView&    outputImpl(size_t id) const;
        inline void                outputImpl(const ImageView& image_view, size_t id);
        inline void                outputImpl(std::shared_ptr<const ImageView> ptr_image_view, size_t id);

        inline void inputImpl(std::shared_ptr<const ImageView> ptr_image_view, size_t id);

    private:
        std::shared_ptr<const ComputePipeline>  _ptr_pipeline;
        std::shared_ptr<const DescriptorSet>    _ptr_descriptor_set;
        std::shared_ptr<const Sampler>          _ptr_sampler;

        std::shared_ptr<DeviceQueue> _ptr_device_queue;

        std::shared_ptr<Buffer> _ptr_uniform_point_lights_data_buffer;
        std::shared_ptr<Buffer> _ptr_uniform_spot_lights_data_buffer;
        std::shared_ptr<Buffer> _ptr_uniform_direction_lights_data_buffer;
        std::shared_ptr<Buffer> _ptr_uniform_num_lights_buffer;

        std::shared_ptr<Buffer> _ptr_uniform_camera_data_buffer;

        union
        {
            struct 
            {
                const ImageView* _ptr_position_and_metallic_image_view;
                const ImageView* _ptr_normal_and_roughness_image_view;
                const ImageView* _ptr_albedo_and_baked_AO_image_view;
                const ImageView* _ptr_anisotropy_image_view;
            };

            const ImageView* _ptr_images_views[InputImagesViewsIDs::Count];
        };

        const ImageView* _out_image_view;
    };
}

#include "PBRPass.inl"

#endif /* PBRPass_hpp */
