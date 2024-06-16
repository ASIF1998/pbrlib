//
//  PBRPass.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 19/07/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef PBRPass_hpp
#define PBRPass_hpp

#include <memory>

#include "../../../SceneGraph/Scene.hpp"

#include "Pass.hpp"

using namespace std;

namespace pbrlib
{
    class   ComputePipeline;
    class   Device;
    class   DescriptorSet;
    class   CommandBuffer;
    class   ImageView;
    class   PBRPass;
    class   Sampler;
    class   DescriptorPool;
    class   Buffer;
    struct  PhysicalDevice;
    class   CameraBase;
    class   Scene;

    using PtrComputePipeline    = shared_ptr<ComputePipeline>;
    using PtrDevice             = shared_ptr<Device>;
    using PtrDescriptorSet      = shared_ptr<DescriptorSet>;
    using PtrCommandBuffer      = shared_ptr<CommandBuffer>;
    using PtrPBRPass            = unique_ptr<PBRPass>;
    using PtrSampler            = shared_ptr<Sampler>;
    using PtrDescriptorPool     = shared_ptr<DescriptorPool>;
    using PtrBuffer             = shared_ptr<Buffer>;
    using PtrPhysicalDevice     = shared_ptr<PhysicalDevice>;

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

            inline void setDevice(const PtrDevice& ptr_device);
            inline void setPhysicalDevice(const PtrPhysicalDevice& ptr_physical_device);
            inline void setDescriptorPool(const PtrDescriptorPool& ptr_descriptor_pool);
            inline void setPositionAndMetallicImageView(const ImageView* position_and_metallic_image_view);
            inline void setNormalAndRoughnessImageView(const ImageView* normal_and_roughness_image_view);
            inline void setAlbedoAndBakedAOImageView(const ImageView* albedo_and_baked_AO_image_view);
            inline void setAnisotropyImageView(const ImageView* anisotropy_image_view);
            inline void setSampler(const PtrSampler& ptr_sampler);
            inline void setOptionals(const Optionals& optionals);
            inline void setQueue(const PtrDeviceQueue& ptr_queue);

            inline PBRPass      build();
            inline PtrPBRPass   buildPtr();

        private:
            PtrDevice           _ptr_device;
            PtrPhysicalDevice   _ptr_physical_deviec;
            PtrDescriptorPool   _ptr_descriptor_pool;
            const ImageView*    _position_and_metallic_image_view;
            const ImageView*    _normal_and_roughness_image_view;
            const ImageView*    _albedo_and_baked_AO_image_view;
            const ImageView*    _anisotropy_image_view;
            PtrSampler          _ptr_sampler;
            Optionals           _optionals;
            PtrDeviceQueue      _ptr_device_queue;
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
            const PtrDevice&            ptr_device, 
            const PtrPhysicalDevice&    ptr_physical_deviec,
            const PtrDeviceQueue&       ptr_queue,
            const PtrDescriptorPool&    ptr_descriptor_pool,
            const PtrSampler&           ptr_sampler,
            const Optionals&            optionals = Optionals()
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
            const PtrDevice&            ptr_device, 
            const PtrPhysicalDevice&    ptr_physical_deviec,
            const PtrDeviceQueue&       ptr_queue,
            const PtrDescriptorPool&    ptr_descriptor_pool,
            const ImageView&            position_and_metallic_image_view,
            const ImageView&            normal_and_roughness_image_view,
            const ImageView&            albedo_and_baked_AO_image_view,
            const ImageView&            anisotropy_image_view,
            const PtrSampler&           ptr_sampler,
            const Optionals&            optionals = Optionals()
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
            const PtrSceneItem&             ptr_camera,
            const PtrPrimaryCommandBuffer&  ptr_command_buffer,
            const vector<PtrSceneItem>      point_lights,
            const vector<PtrSceneItem>      spot_lights,
            const vector<PtrSceneItem>      direction_lights
        );

        inline PtrComputePipeline&         getPipeline() noexcept;
        inline const PtrComputePipeline&   getPipeline() const noexcept;

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
        inline static PtrPBRPass make(
            const PtrDevice&            ptr_device, 
            const PtrPhysicalDevice&    ptr_physical_deviec,
            const PtrDeviceQueue&       ptr_queue,
            const PtrDescriptorPool&    ptr_descriptor_pool,
            const PtrSampler&           ptr_sampler,
            const Optionals&            optionals = Optionals()
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
        inline static PtrPBRPass make(
            const PtrDevice&            ptr_device, 
            const PtrPhysicalDevice&    ptr_physical_deviec,
            const PtrDeviceQueue&       ptr_queue,
            const PtrDescriptorPool&    ptr_descriptor_pool,
            const ImageView&            position_and_metallic_image_view,
            const ImageView&            normal_and_roughness_image_view,
            const ImageView&            albedo_and_baked_AO_image_view,
            const ImageView&            anisotropy_image_view,
            const PtrSampler&           ptr_sampler,
            const Optionals&            optionals = Optionals()
        );

    private:
        friend class IPassInputOutput<PBRPass>;
        friend class IPassOutput<PBRPass>;
        
        inline const ImageView&    outputImpl(size_t id) const;
        inline void                outputImpl(ImageView& image_view, size_t id);
        inline void                outputImpl(PtrImageView& ptr_image_view, size_t id);

        inline void inputImpl(const PtrImageView& ptr_image_view, size_t id);

    private:
        PtrComputePipeline  _ptr_pipeline;
        PtrDescriptorSet    _ptr_descriptor_set;
        PtrSampler          _ptr_sampler;

        PtrDeviceQueue _ptr_device_queue;

        PtrBuffer _ptr_uniform_point_lights_data_buffer;
        PtrBuffer _ptr_uniform_spot_lights_data_buffer;
        PtrBuffer _ptr_uniform_direction_lights_data_buffer;
        PtrBuffer _ptr_uniform_num_lights_buffer;

        PtrBuffer _ptr_uniform_camera_data_buffer;

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
