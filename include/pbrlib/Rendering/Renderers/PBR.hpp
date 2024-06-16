//
//  PBR.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 19/07/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef PBR_hpp
#define PBR_hpp

#include "IRenderer.h"

#include "SubPasses/GBufferPass.hpp"
#include "SubPasses/PBRPass.hpp"

#include <vector>
#include <memory>

using namespace std;

namespace pbrlib
{
    class ImageView;
    class Framebuffer;
    class PrimaryCommandBuffer;
    class PBR;

    using PtrAttachments            = shared_ptr<vector<ImageView>>;
    using PtrFramebuffer            = shared_ptr<Framebuffer>; 
    using PtrPrimaryCommandBuffer   = shared_ptr<PrimaryCommandBuffer>;
    using PtrPBR                    = shared_ptr<PBR>;

    class PBR :
        public IRenderer
    {
    public:
        PBR(const PBRPass::Optionals& optionals = PBRPass::Optionals());

        PBR(PBR&&)      = delete;
        PBR(const PBR&) = delete;

        PBR& operator = (PBR&&)         = delete;
        PBR& operator = (const PBR&)    = delete;

        /**
         * @brief Метод, предназначенный для инициализации.
         * 
         * @param ptr_window            указатель на окно.
         * @param ptr_device            указатель на логическое устройство.
         * @param ptr_physical_device   указатель на физическое устройство.
        */
        virtual void init(
            const PtrWindow&            ptr_window, 
            const PtrDevice&            ptr_device, 
            const PtrPhysicalDevice&    ptr_physical_device
        ) override;

        /**
         * @brief Метод, отвечающий за отрисовку.
         * 
         * @param ptr_camera        указатель на камеру.
         * @param visible_list      видимые узлы.
         * @param point_lights      точечные источники света.
         * @param spot_lights       прожекторные источники света.
         * @param direction_lights  направленные источники света.
         * @param delta_time        количество пройденного времени с момента завершения последнего кадра.
        */
        void draw(
            const PtrSceneItem&         ptr_camera,
            const VisibleList&          visible_list, 
            const vector<PtrSceneItem>  point_lights,
            const vector<PtrSceneItem>  spot_lights,
            const vector<PtrSceneItem>  direction_lights,
            float                       delta_time
        ) override;

        PtrGBufferPass&          getGBUfferPass()    noexcept;
        const PtrGBufferPass&    getGBUfferPass()    const noexcept;
        PtrPBRPass&              getPBRPass()        noexcept;
        const PtrPBRPass&        getPBRPass()        const noexcept;

        static PtrPBR make(const PBRPass::Optionals& optionals = PBRPass::Optionals());

    private:
        PtrGBufferPass  _ptr_gbuffer_pass;
        PtrPBRPass      _ptr_pbr_pass;

        PtrDevice _ptr_device;

        PtrDescriptorPool _ptr_descriptor_pool;

        PtrSwapchain            _ptr_swapchain;
        PtrSampler              _ptr_sampler_linear;
        PtrSampler              _ptr_sampler_nearest;

        PtrPrimaryCommandBuffer _ptr_command_buffer;
        PtrDeviceQueue          _ptr_device_queue;

        PBRPass::Optionals  _optionals;
    };
}

#endif /* PBR_hpp */
