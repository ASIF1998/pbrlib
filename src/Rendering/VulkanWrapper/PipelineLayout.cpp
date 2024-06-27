//
//  PipelineLayout.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 23/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <pbrlib/Rendering/VulkanWrapper/PipelineLayout.hpp>

#include <cassert>

namespace pbrlib
{
    DescriptorSetLayoutBindings::DescriptorSetLayoutBindings(
        const Device*   ptr_device,
        size_t                          num_reserve_samplers
    ) :
        _ptr_device(ptr_device)
    {
        _samplers.reserve(num_reserve_samplers);
    }

    DescriptorSetLayoutBindings::DescriptorSetLayoutBindings(DescriptorSetLayoutBindings&& descriptor_set_layout_bindings) :
        _ptr_device                     (std::move(descriptor_set_layout_bindings._ptr_device)),
        _descriptor_set_layout_bindings (std::move(descriptor_set_layout_bindings._descriptor_set_layout_bindings)),
        _samplers                       (std::move(descriptor_set_layout_bindings._samplers))
    {}

    void DescriptorSetLayoutBindings::addBinding(
        uint32_t            binding, 
        VkDescriptorType    descriptor_type, 
        VkShaderStageFlags  stage_flags
    )
    {
        _descriptor_set_layout_bindings.push_back({
            binding,
            descriptor_type,
            1,
            stage_flags,
            nullptr
        });
    }

    void DescriptorSetLayoutBindings::addBinding(
        uint32_t            binding,
        VkDescriptorType    descriptor_type,
        VkShaderStageFlags  stage_flags,
        const SamplerInfo&  sampler_info
    )
    {
        assert(_samplers.capacity() >= _samplers.size() + 1);
        assert((descriptor_type & VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER) || (descriptor_type & VK_DESCRIPTOR_TYPE_SAMPLER));
        
        Sampler sampler (_ptr_device, sampler_info);
        
        _samplers.push_back(std::move(sampler));

        _descriptor_set_layout_bindings.push_back({
            binding,
            descriptor_type,
            1,
            stage_flags,
            &_samplers.back().getSamplerHandle()
        });
    }

    const std::vector<VkDescriptorSetLayoutBinding>& DescriptorSetLayoutBindings::getDescriptorSetLayoutBindings() const noexcept
    {
        return _descriptor_set_layout_bindings;
    }

    const Device* DescriptorSetLayoutBindings::getDevice() const noexcept
    {
        return _ptr_device;
    }

    const std::vector<Sampler>& DescriptorSetLayoutBindings::getSamplers() const noexcept
    {
        return _samplers;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    DescriptorSetLayout::DescriptorSetLayout(DescriptorSetLayoutBindings&& descriptor_set_layout_bindings) :
        _descriptor_set_layout_bindings (std::move(descriptor_set_layout_bindings)),
        _descriptor_set_layout_handle   (VK_NULL_HANDLE)
    {
        _create();
    }

    DescriptorSetLayout::DescriptorSetLayout(DescriptorSetLayout&& descriptor_set_layout) :
        _descriptor_set_layout_bindings (std::move(descriptor_set_layout._descriptor_set_layout_bindings)),
        _descriptor_set_layout_handle   (VK_NULL_HANDLE)
    {
        std::swap(_descriptor_set_layout_handle, descriptor_set_layout._descriptor_set_layout_handle);
    }

    DescriptorSetLayout::~DescriptorSetLayout() noexcept
    {
        if (_descriptor_set_layout_handle != VK_NULL_HANDLE) {
            vkDestroyDescriptorSetLayout(
                _descriptor_set_layout_bindings.getDevice()->getDeviceHandle(),
                _descriptor_set_layout_handle,
                nullptr
            );
        }
    }

    void DescriptorSetLayout::_create()
    {
        VkDescriptorSetLayoutCreateInfo descriptor_set_layout_info = { };
        descriptor_set_layout_info.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        descriptor_set_layout_info.bindingCount = static_cast<uint32_t>(_descriptor_set_layout_bindings.getDescriptorSetLayoutBindings().size());
        descriptor_set_layout_info.pBindings    = _descriptor_set_layout_bindings.getDescriptorSetLayoutBindings().data();

        assert(vkCreateDescriptorSetLayout(
            _descriptor_set_layout_bindings.getDevice()->getDeviceHandle(),
            &descriptor_set_layout_info,
            nullptr,
            &_descriptor_set_layout_handle
        ) == VK_SUCCESS);

        assert(_descriptor_set_layout_handle != VK_NULL_HANDLE);
    }

    const Device* DescriptorSetLayout::getDevice() const noexcept
    {
        return _descriptor_set_layout_bindings.getDevice();
    }

    const VkDescriptorSetLayout& DescriptorSetLayout::getDescriptorSetLayoutHandle() const noexcept
    {
        return _descriptor_set_layout_handle;
    }

    DescriptorSetLayoutBindings& DescriptorSetLayout::getDescriptorSetLayoutBindings() noexcept
    {
        return _descriptor_set_layout_bindings;
    }

    const DescriptorSetLayoutBindings& DescriptorSetLayout::getDescriptorSetLayoutBindings() const noexcept
    {
        return _descriptor_set_layout_bindings;
    }

    std::unique_ptr<DescriptorSetLayout> DescriptorSetLayout::make(DescriptorSetLayoutBindings&& descriptor_set_layout_bindings)
    {
        return std::make_unique<DescriptorSetLayout>(std::move(descriptor_set_layout_bindings));
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PipelineLayout::PipelineLayout(
        const Device*                                           ptr_device,
        std::span<std::shared_ptr<const DescriptorSetLayout>>   descriptor_set_layouts
    ) :
        _ptr_device             (ptr_device),
        _descriptor_set_layouts (std::begin(descriptor_set_layouts), std::end(descriptor_set_layouts)),
        _push_constant_ranges   (0),
        _pipeline_layout_handle (VK_NULL_HANDLE)
    {
        _create();
    }
    
    PipelineLayout::PipelineLayout(
        const Device*                                           ptr_device,
        std::span<std::shared_ptr<const DescriptorSetLayout>>   descriptor_set_layouts,
        std::span<const VkPushConstantRange>                    push_constant_ranges
    ) :
        _ptr_device             (ptr_device),
        _descriptor_set_layouts (std::begin(descriptor_set_layouts), std::end(descriptor_set_layouts)),
        _push_constant_ranges   (std::begin(push_constant_ranges), std::end(push_constant_ranges)),
        _pipeline_layout_handle (VK_NULL_HANDLE)
    {
        _create();
    }

    PipelineLayout::PipelineLayout(
        const Device*                               ptr_device,
        std::shared_ptr<const DescriptorSetLayout>  descriptor_set_layout
    ) :
        _ptr_device             (ptr_device),
        _descriptor_set_layouts ({descriptor_set_layout}),
        _push_constant_ranges   (0)
    {
        _create();
    }
    
    PipelineLayout::PipelineLayout(
        const Device*                               ptr_device,
        std::shared_ptr<const DescriptorSetLayout>  descriptor_set_layout,
        std::span<const VkPushConstantRange>        push_constant_ranges
    ) :
        _ptr_device             (ptr_device),
        _descriptor_set_layouts ({descriptor_set_layout}),
        _push_constant_ranges   (std::begin(push_constant_ranges), std::end(push_constant_ranges))
    {
        _create();
    }

    PipelineLayout::PipelineLayout(PipelineLayout&& pipeline_layout) :
        _ptr_device             (std::move(pipeline_layout._ptr_device)),
        _descriptor_set_layouts (std::move(pipeline_layout._descriptor_set_layouts)),
        _push_constant_ranges   (std::move(pipeline_layout._push_constant_ranges)),
        _pipeline_layout_handle (VK_NULL_HANDLE)
    {
        std::swap(_pipeline_layout_handle, pipeline_layout._pipeline_layout_handle);
    }

    PipelineLayout::~PipelineLayout() noexcept
    {
        if (_pipeline_layout_handle != VK_NULL_HANDLE)
            vkDestroyPipelineLayout(_ptr_device->getDeviceHandle(), _pipeline_layout_handle, nullptr);
    }

    void PipelineLayout::_create()
    {
        std::vector<VkDescriptorSetLayout> set_layout_handles (_descriptor_set_layouts.size());

        for (size_t i{0}; i < _descriptor_set_layouts.size(); i++) {
            set_layout_handles[i] = _descriptor_set_layouts[i]->getDescriptorSetLayoutHandle();
        }

        VkPipelineLayoutCreateInfo pipeline_layout_info = { };
        pipeline_layout_info.sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipeline_layout_info.setLayoutCount         = static_cast<uint32_t>(set_layout_handles.size());
        pipeline_layout_info.pSetLayouts            = set_layout_handles.data();
        pipeline_layout_info.pushConstantRangeCount = static_cast<uint32_t>(_push_constant_ranges.size());
        pipeline_layout_info.pPushConstantRanges    = _push_constant_ranges.data();

        assert(vkCreatePipelineLayout(
            _ptr_device->getDeviceHandle(), 
            &pipeline_layout_info, 
            nullptr, 
            &_pipeline_layout_handle
        ) == VK_SUCCESS);

        assert(_pipeline_layout_handle != VK_NULL_HANDLE);
    }

    const Device* PipelineLayout::getDevice() const noexcept
    {
        return _ptr_device;
    }

    const std::vector<std::shared_ptr<const DescriptorSetLayout>>& PipelineLayout::getDescriptorSetLayouts() const noexcept
    {
        return _descriptor_set_layouts;
    }

    const std::vector<VkPushConstantRange>& PipelineLayout::getPushConstantRanges() const noexcept
    {
        return _push_constant_ranges;
    }

    const VkPipelineLayout& PipelineLayout::getPipelineLayoutHandle() const noexcept
    {
        return _pipeline_layout_handle;
    }

    std::unique_ptr<PipelineLayout> PipelineLayout::make(
        const Device*                                           ptr_device,
        std::span<std::shared_ptr<const DescriptorSetLayout>>   descriptor_set_layouts
    )
    {
        return make_unique<PipelineLayout>(ptr_device, descriptor_set_layouts);
    }

    std::unique_ptr<PipelineLayout> PipelineLayout::make(
        const Device*                                           ptr_device,
        std::span<std::shared_ptr<const DescriptorSetLayout>>   descriptor_set_layouts,
        std::span<const VkPushConstantRange>                    push_constant_ranges
    )
    {
        return std::make_unique<PipelineLayout>(ptr_device, descriptor_set_layouts, push_constant_ranges);
    }

    std::unique_ptr<PipelineLayout> PipelineLayout::make(
        const Device*                               ptr_device,
        std::shared_ptr<const DescriptorSetLayout>  descriptor_set_layout
    )
    {
        return make_unique<PipelineLayout>(ptr_device, descriptor_set_layout);
    }

    std::unique_ptr<PipelineLayout> PipelineLayout::make(
        const Device*                               ptr_device,
        std::shared_ptr<const DescriptorSetLayout>  descriptor_set_layout,
        std::span<const VkPushConstantRange>        push_constant_ranges
    )
    {
        return std::make_unique<PipelineLayout>(ptr_device, descriptor_set_layout, push_constant_ranges);
    }
}
