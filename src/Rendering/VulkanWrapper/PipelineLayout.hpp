//
//  PipelineLayout.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 23/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef PipelineLayout_hpp
#define PipelineLayout_hpp

#include "Sampler.hpp"

#include <vector>
#include <memory>

using namespace std;

namespace pbrlib
{
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
        inline DescriptorSetLayoutBindings(
            const shared_ptr<Device>&   ptr_device,
            size_t                      num_reserve_samplers
        );
        
        inline DescriptorSetLayoutBindings(DescriptorSetLayoutBindings&& descriptor_set_layout_bindings);
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
        inline void addBinding(
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
        inline void addBinding(
            uint32_t            binding,
            VkDescriptorType    descriptor_type,
            VkShaderStageFlags  stage_flags,
            const SamplerInfo&  sampler_info
        );

        inline const vector<VkDescriptorSetLayoutBinding>&  getDescriptorSetLayoutBindings()    const noexcept;
        inline shared_ptr<Device>&                          getDevice()                         noexcept;
        inline const shared_ptr<Device>&                    getDevice()                         const noexcept;
        inline const vector<Sampler>&                       getSamplers()                       const noexcept;

    private:
        shared_ptr<Device>                      _ptr_device;
        vector<VkDescriptorSetLayoutBinding>    _descriptor_set_layout_bindings;
        vector<Sampler>                         _samplers;
    };

    class DescriptorSetLayout
    {
    public:
        /**
         * @brief Конструктор.
         * 
         * @param descriptor_set_layout_bindings привязки.
        */
        inline DescriptorSetLayout(DescriptorSetLayoutBindings&& descriptor_set_layout_bindings);

        inline DescriptorSetLayout(DescriptorSetLayout&& descriptor_set_layout);
        inline DescriptorSetLayout(const DescriptorSetLayout&) = delete;

        inline ~DescriptorSetLayout() noexcept;

        DescriptorSetLayout& operator = (DescriptorSetLayout&&)        = delete;
        DescriptorSetLayout& operator = (const DescriptorSetLayout&)   = delete;

        inline shared_ptr<Device>&                  getDevice()                         noexcept;
        inline const shared_ptr<Device>&            getDevice()                         const noexcept;
        inline VkDescriptorSetLayout                getDescriptorSetLayoutHandle()      const noexcept;
        inline DescriptorSetLayoutBindings&         getDescriptorSetLayoutBindings()    noexcept;
        inline const DescriptorSetLayoutBindings&   getDescriptorSetLayoutBindings()    const noexcept;
    
    private:
        inline void _create();

    private:
        DescriptorSetLayoutBindings _descriptor_set_layout_bindings;
        VkDescriptorSetLayout       _descriptor_set_layout_handle;
    };

    class PipelineLayout
    {
    public:
        inline PipelineLayout(
            const shared_ptr<Device>&                       ptr_device,
            const vector<shared_ptr<DescriptorSetLayout>>&  descriptor_set_layouts
        );
        
        inline PipelineLayout(
            const shared_ptr<Device>&                   ptr_device, 
            vector<shared_ptr<DescriptorSetLayout>>&&   descriptor_set_layouts
        );
        
        inline PipelineLayout(
            const shared_ptr<Device>&                       ptr_device, 
            const vector<shared_ptr<DescriptorSetLayout>>&  descriptor_set_layouts,
            const vector<VkPushConstantRange>&              push_constant_ranges
        );
        
        inline PipelineLayout(
            const shared_ptr<Device>&                   ptr_device, 
            vector<shared_ptr<DescriptorSetLayout>>&&   descriptor_set_layouts,
            const vector<VkPushConstantRange>&          push_constant_ranges
        );
        
        inline PipelineLayout(
            const shared_ptr<Device>&                       ptr_device, 
            const vector<shared_ptr<DescriptorSetLayout>>&  descriptor_set_layouts,
            vector<VkPushConstantRange>&&                   push_constant_ranges
        );
        
        inline PipelineLayout(
            const shared_ptr<Device>&                   ptr_device, 
            vector<shared_ptr<DescriptorSetLayout>>&&   descriptor_set_layouts,
            vector<VkPushConstantRange>&&               push_constant_ranges
        );

        inline PipelineLayout(PipelineLayout&& pipeline_layout);
        inline PipelineLayout(const PipelineLayout& pipeline_layout) = delete;

        inline ~PipelineLayout() noexcept;

        PipelineLayout& operator = (PipelineLayout&&)       = delete;
        PipelineLayout& operator = (const PipelineLayout&)  = delete;

        inline shared_ptr<Device>&                              getDevice()                 noexcept;
        inline const shared_ptr<Device>&                        getDevice()                 const noexcept;
        inline vector<shared_ptr<DescriptorSetLayout>>&         getDescriptorSetLayouts()   noexcept;
        inline const vector<shared_ptr<DescriptorSetLayout>>&   getDescriptorSetLayouts()   const noexcept;
        inline const vector<VkPushConstantRange>&               getPushConstantRanges()     const noexcept;
        inline const VkPipelineLayout&                          getPipelineLayoutHandle()   const noexcept;

        inline static shared_ptr<PipelineLayout> make(
            const shared_ptr<Device>&                       ptr_device, 
            const vector<shared_ptr<DescriptorSetLayout>>&  descriptor_set_layouts
        );

        inline static shared_ptr<PipelineLayout> make(
            const shared_ptr<Device>&                   ptr_device, 
            vector<shared_ptr<DescriptorSetLayout>>&&   descriptor_set_layouts
        );

        inline static shared_ptr<PipelineLayout> make(
            const shared_ptr<Device>&                       ptr_device, 
            const vector<shared_ptr<DescriptorSetLayout>>&  descriptor_set_layouts,
            const vector<VkPushConstantRange>&              push_constant_ranges
        );

        inline static shared_ptr<PipelineLayout> make(
            const shared_ptr<Device>&                   ptr_device, 
            vector<shared_ptr<DescriptorSetLayout>>&&   descriptor_set_layouts,
            const vector<VkPushConstantRange>&          push_constant_ranges
        );

        inline static shared_ptr<PipelineLayout> make(
            const shared_ptr<Device>&                       ptr_device, 
            const vector<shared_ptr<DescriptorSetLayout>>&  descriptor_set_layouts,
            vector<VkPushConstantRange>&&                   push_constant_ranges
        );

        inline static shared_ptr<PipelineLayout> make(
            const shared_ptr<Device>&                       ptr_device, 
            vector<shared_ptr<DescriptorSetLayout>>&&   descriptor_set_layouts,
            vector<VkPushConstantRange>&&               push_constant_ranges
        );

    private:
        inline void _create_pipeline_layout();
    
    private:
        shared_ptr<Device>                      _ptr_device;
        vector<shared_ptr<DescriptorSetLayout>> _descriptor_set_layouts;
        vector<VkPushConstantRange>             _push_constant_ranges;
        VkPipelineLayout                        _pipeline_layout_handle;
    };
}

#include "PipelineLayout.inl"

#endif /* PipelineLayout_hpp */
