//
//  PipelineLayout.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 23/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef PipelineLayout_hpp
#define PipelineLayout_hpp

#include <vector>
#include <memory>

#include "Sampler.hpp"

namespace pbrlib
{
    class PipelineLayout;
    class DescriptorSetLayout;
    class SamplerInfo;

    using PtrPipelineLayout         = std::shared_ptr<PipelineLayout>;
    using PtrDescriptorSetLayout    = std::shared_ptr<DescriptorSetLayout>;

    /**
     * @class DescriptorSetLayoutBindings.
     * @brief Класс необходимый для накапливания привязок.
    */
    class DescriptorSetLayoutBindings
    {
    public:
        /**
         * @brief Конструктор.
         * 
         * @param ptr_device            указатель на устройство.
         * @param num_reserve_samplers  количество семплеров, необходимое для резервирования памяти.
        */
        DescriptorSetLayoutBindings(
            const PtrDevice&            ptr_device,
            size_t                      num_reserve_samplers
        );
        
        DescriptorSetLayoutBindings(DescriptorSetLayoutBindings&& descriptor_set_layout_bindings);
        DescriptorSetLayoutBindings(const DescriptorSetLayoutBindings&) = delete;

        DescriptorSetLayoutBindings& operator = (DescriptorSetLayoutBindings&&)         = delete;
        DescriptorSetLayoutBindings& operator = (const DescriptorSetLayoutBindings&)    = delete;

        /**
         * @brief Метод добавляющий привязку.
         * 
         * @param binding           номер привязки.
         * @param descriptor_type   тип дескриптора в данной точке привязки.
         * @param stage_flags       указывает какие шейдеры могут обращаться к данной привязке.
        */
        void addBinding(
            uint32_t            binding,
            VkDescriptorType    descriptor_type,
            VkShaderStageFlags  stage_flags
        );

        /**
         * @brief Метод добавляющий привязку.
         * @details 
         *      Данный метод следует использовать если descriptor_type равен 
         *      VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER или VK_DESCRIPTOR_TYPE_SAMPLER.
         * 
         * @param binding           номер привязки.
         * @param descriptor_type   тип дескриптора в данной точке привязки.
         * @param stage_flags       указывает какие шейдеры могут обращаться к данной привязке.
         * @param sampler_info      информация о сэмплере.
        */
        void addBinding(
            uint32_t            binding,
            VkDescriptorType    descriptor_type,
            VkShaderStageFlags  stage_flags,
            const SamplerInfo&  sampler_info
        );

        const std::vector<VkDescriptorSetLayoutBinding>&    getDescriptorSetLayoutBindings()    const noexcept;
        PtrDevice&                                          getDevice()                         noexcept;
        const PtrDevice&                                    getDevice()                         const noexcept;
        const std::vector<Sampler>&                         getSamplers()                       const noexcept;

    private:
        PtrDevice                                   _ptr_device;
        std::vector<VkDescriptorSetLayoutBinding>   _descriptor_set_layout_bindings;
        std::vector<Sampler>                        _samplers;
    };

    class DescriptorSetLayout
    {
    public:
        /**
         * @brief Конструктор.
         * 
         * @param descriptor_set_layout_bindings привязки.
        */
        DescriptorSetLayout(DescriptorSetLayoutBindings&& descriptor_set_layout_bindings);

        DescriptorSetLayout(DescriptorSetLayout&& descriptor_set_layout);
        DescriptorSetLayout(const DescriptorSetLayout&) = delete;

        ~DescriptorSetLayout() noexcept;

        DescriptorSetLayout& operator = (DescriptorSetLayout&&)        = delete;
        DescriptorSetLayout& operator = (const DescriptorSetLayout&)   = delete;

        PtrDevice&                           getDevice()                         noexcept;
        const PtrDevice&                     getDevice()                         const noexcept;
        const VkDescriptorSetLayout&         getDescriptorSetLayoutHandle()      const noexcept;
        DescriptorSetLayoutBindings&         getDescriptorSetLayoutBindings()    noexcept;
        const DescriptorSetLayoutBindings&   getDescriptorSetLayoutBindings()    const noexcept;

        static PtrDescriptorSetLayout make(DescriptorSetLayoutBindings&& descriptor_set_layout_bindings);
    
    private:
        void _create();

    private:
        DescriptorSetLayoutBindings _descriptor_set_layout_bindings;
        VkDescriptorSetLayout       _descriptor_set_layout_handle;
    };

    class PipelineLayout
    {
    public:
        PipelineLayout(
            const PtrDevice&                        ptr_device,
            std::span<const PtrDescriptorSetLayout> descriptor_set_layouts
        );
        
        PipelineLayout(
            const PtrDevice&                        ptr_device,
            std::span<const PtrDescriptorSetLayout> descriptor_set_layouts,
            std::span<const VkPushConstantRange>    push_constant_ranges
        );

        PipelineLayout(
            const PtrDevice&                ptr_device,
            const PtrDescriptorSetLayout&   descriptor_set_layout
        );
        
        PipelineLayout(
            const PtrDevice&                        ptr_device,
            const PtrDescriptorSetLayout&           descriptor_set_layout,
            std::span<const VkPushConstantRange>    push_constant_ranges
        );

        PipelineLayout(PipelineLayout&& pipeline_layout);
        PipelineLayout(const PipelineLayout& pipeline_layout) = delete;

        ~PipelineLayout() noexcept;

        PipelineLayout& operator = (PipelineLayout&&)       = delete;
        PipelineLayout& operator = (const PipelineLayout&)  = delete;

        PtrDevice&                                  getDevice()                 noexcept;
        const PtrDevice&                            getDevice()                 const noexcept;
        std::vector<PtrDescriptorSetLayout>&        getDescriptorSetLayouts()   noexcept;
        const std::vector<PtrDescriptorSetLayout>&  getDescriptorSetLayouts()   const noexcept;
        const std::vector<VkPushConstantRange>&     getPushConstantRanges()     const noexcept;
        const VkPipelineLayout&                     getPipelineLayoutHandle()   const noexcept;

        static PtrPipelineLayout make(
            const PtrDevice&                            ptr_device,
            std::span<const PtrDescriptorSetLayout>     descriptor_set_layouts
        );

        static PtrPipelineLayout make(
            const PtrDevice&                            ptr_device,
            std::span<const PtrDescriptorSetLayout>     descriptor_set_layouts,
            std::span<const VkPushConstantRange>        push_constant_ranges
        );

        static PtrPipelineLayout make(
            const PtrDevice&                ptr_device,
            const PtrDescriptorSetLayout&   descriptor_set_layouts
        );

        static PtrPipelineLayout make(
            const PtrDevice&                        ptr_device,
            const PtrDescriptorSetLayout&           descriptor_set_layouts,
            std::span<const VkPushConstantRange>    push_constant_ranges
        );

    private:
        void _create();
    
    private:
        PtrDevice                           _ptr_device;
        std::vector<PtrDescriptorSetLayout> _descriptor_set_layouts;
        std::vector<VkPushConstantRange>    _push_constant_ranges;
        VkPipelineLayout                    _pipeline_layout_handle;
    };
}

#endif /* PipelineLayout_hpp */
