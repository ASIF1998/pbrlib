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

namespace pbrlib
{
    class ImageView;

    using PtrAttachments = std::shared_ptr<std::vector<ImageView>>;

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
            std::shared_ptr<const Window>   ptr_window, 
            const Device*                   ptr_device, 
            const PhysicalDevice*           ptr_physical_device
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
            std::shared_ptr<const SceneItem>            ptr_camera,
            const VisibleList&                          visible_list, 
            std::span<std::shared_ptr<SceneItem>>       point_lights,
            std::span<std::shared_ptr<SceneItem>>       spot_lights,
            std::span<std::shared_ptr<SceneItem>>       direction_lights,
            float                                       delta_time
        ) override;

        const GBufferPass*  getGBUfferPass()    const noexcept;
        const PBRPass*      getPBRPass()        const noexcept;

        static std::unique_ptr<PBR> make(const PBRPass::Optionals& optionals = PBRPass::Optionals());

    private:
        std::unique_ptr<GBufferPass>    _ptr_gbuffer_pass;
        std::unique_ptr<PBRPass>  _ptr_pbr_pass;

        const Device* _ptr_device;

        std::shared_ptr<const DescriptorPool> _ptr_descriptor_pool;

        std::shared_ptr<const Swapchain>    _ptr_swapchain;
        std::shared_ptr<const Sampler>      _ptr_sampler_linear;
        std::shared_ptr<const Sampler>      _ptr_sampler_nearest;

        std::unique_ptr<CommandPool>            _ptr_command_pool;
        std::shared_ptr<PrimaryCommandBuffer>   _ptr_command_buffer;
        std::shared_ptr<DeviceQueue>            _ptr_device_queue;

        PBRPass::Optionals  _optionals;
    };
}

#endif /* PBR_hpp */
