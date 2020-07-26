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

using namespace std;

namespace pbrlib
{
    class ComputePipeline;
    class Device;
    class DescriptorSet;
    class CommandBuffer;
    class ImageView;
    class PBRPass;
    class Sampler;
    class DescriptorPool;

    using PtrComputePipeline    = shared_ptr<ComputePipeline>;
    using PtrDevice             = shared_ptr<Device>;
    using PtrDescriptorSet      = shared_ptr<DescriptorSet>;
    using PtrCommandBuffer      = shared_ptr<CommandBuffer>;
    using PtrImageView          = shared_ptr<ImageView>;
    using PtrPBRPass            = unique_ptr<PBRPass>;
    using PtrSampler            = shared_ptr<Sampler>;
    using PtrDescriptorPool     = shared_ptr<DescriptorPool>;

    class PBRPass
    {
    public:
        PBRPass(const PtrDevice& ptr_device, const PtrDescriptorPool& ptr_descriptor_pool);

        /**
         * @brief Метод, позволяющий запустить проход физически-корректного рендеринга.
         * 
         * @param ptr_command_buffer                указатель на командный буфер.
         * @param position_and_metallic_image_view  указатель на вид изображения с позициями и металичностью.
         * @param normal_and_roughness_image        указатель на вид изображения с нормалями и шероховатостью.
         * @param albedo_and_baked_AO_image         указатель на вид изображения с альбедо и запечённым AO.
         * @param out_image_view                    указатель на вид изображения в который будет записан резкльтат прохода.
         * @param ptr_sampler                       указатель на сэмплер.
        */
        void draw(
            const PtrCommandBuffer& ptr_command_buffer,
            const ImageView&        position_and_metallic_image_view,
            const ImageView&        normal_and_roughness_image,
            const ImageView&        albedo_and_baked_AO_image,
            const ImageView&        out_image_view,
            const PtrSampler&       ptr_sampler
        );

        PtrComputePipeline&         getPipeline() noexcept;
        const PtrComputePipeline&   getPipeline() const noexcept;

        static PtrPBRPass make(const PtrDevice& ptr_device, const PtrDescriptorPool& ptr_descriptor_pool);

    private:
        PtrComputePipeline  _ptr_pipeline;
        PtrDescriptorSet    _ptr_descriptor_set;
    };
}

#endif /* PBRPass_hpp */
