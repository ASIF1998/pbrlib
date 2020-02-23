//
//  PipelineLayout.inl
//  PBRLib
//
//  Created by Асиф Мамедов on 23/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

namespace pbrlib
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inline DescriptorSetLayoutBindings::DescriptorSetLayoutBindings(const shared_ptr<Device>& ptr_device,
                                                                    size_t num_reserve_samplers) :
        _ptr_device(ptr_device)
    {
        _samplers.reserve(num_reserve_samplers);
    }

    inline DescriptorSetLayoutBindings::DescriptorSetLayoutBindings(DescriptorSetLayoutBindings&& descriptor_set_layout_bindings) :
        _ptr_device(move(descriptor_set_layout_bindings._ptr_device)),
        _descriptor_set_layout_bindings(move(descriptor_set_layout_bindings._descriptor_set_layout_bindings)),
        _samplers(move(descriptor_set_layout_bindings._samplers))
    {}

    inline void DescriptorSetLayoutBindings::addBinding(uint32_t binding, VkDescriptorType descriptor_type, VkShaderStageFlags stage_flags)
    {
        _descriptor_set_layout_bindings.push_back({
            .binding = binding,
            .descriptorType = descriptor_type,
            .descriptorCount = 1,
            .stageFlags = stage_flags,
            .pImmutableSamplers = nullptr
        });
    }

    inline void DescriptorSetLayoutBindings::addBinding(uint32_t binding,
                                                        VkDescriptorType descriptor_type,
                                                        VkShaderStageFlags stage_flags,
                                                        const SamplerInfo& sampler_info)
    {
        assert(_samplers.capacity() >= _samplers.size() + 1);
        assert((descriptor_type & VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER) || (descriptor_type & VK_DESCRIPTOR_TYPE_SAMPLER));
        
        Sampler sampler (_ptr_device, sampler_info);
        
        _samplers.push_back(move(sampler));

        _descriptor_set_layout_bindings.push_back({
            .binding = binding,
            .descriptorType = descriptor_type,
            .descriptorCount = 1,
            .stageFlags = stage_flags,
            .pImmutableSamplers = &_samplers.back().getSamplerHandle()
        });
    }

    inline const vector<VkDescriptorSetLayoutBinding>& DescriptorSetLayoutBindings::getDescriptorSetLayoutBindings() const noexcept
    {
        return _descriptor_set_layout_bindings;
    }

    inline shared_ptr<Device>& DescriptorSetLayoutBindings::getDevice() noexcept
    {
        return _ptr_device;
    }

    inline const shared_ptr<Device>& DescriptorSetLayoutBindings::getDevice() const noexcept
    {
        return _ptr_device;
    }

    inline const vector<Sampler>& DescriptorSetLayoutBindings::getSamplers() const noexcept
    {
        return _samplers;
    }

    inline DescriptorSetLayout::DescriptorSetLayout(DescriptorSetLayoutBindings&& descriptor_set_layout_bindings) :
        _descriptor_set_layout_bindings(move(descriptor_set_layout_bindings)),
        _descriptor_set_layout_handle(VK_NULL_HANDLE)
    {
        _create_descriptor_set_layout();
    }

    inline DescriptorSetLayout::DescriptorSetLayout(DescriptorSetLayout&& descriptor_set_layout) :
        _descriptor_set_layout_bindings(move(descriptor_set_layout._descriptor_set_layout_bindings)),
        _descriptor_set_layout_handle(VK_NULL_HANDLE)
    {
        swap(_descriptor_set_layout_handle, descriptor_set_layout._descriptor_set_layout_handle);
    }

    inline DescriptorSetLayout::~DescriptorSetLayout() noexcept
    {
        if (_descriptor_set_layout_handle != VK_NULL_HANDLE) {
            vkDestroyDescriptorSetLayout(_descriptor_set_layout_bindings.getDevice()->getDeviceHandle(),
                                         _descriptor_set_layout_handle,
                                         nullptr);
        }
    }

    inline void DescriptorSetLayout::_create_descriptor_set_layout()
    {
        VkDescriptorSetLayoutCreateInfo descriptor_set_layout_info {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .bindingCount = static_cast<uint32_t>(_descriptor_set_layout_bindings.getDescriptorSetLayoutBindings().size()),
            .pBindings = _descriptor_set_layout_bindings.getDescriptorSetLayoutBindings().data()
        };

        assert(vkCreateDescriptorSetLayout(_descriptor_set_layout_bindings.getDevice()->getDeviceHandle(),
                                          &descriptor_set_layout_info,
                                          nullptr,
                                          &_descriptor_set_layout_handle) == VK_SUCCESS);
        assert(_descriptor_set_layout_handle != VK_NULL_HANDLE);
    }

    inline shared_ptr<Device>& DescriptorSetLayout::getDevice() noexcept
    {
        return _descriptor_set_layout_bindings.getDevice();
    }

    inline const shared_ptr<Device>& DescriptorSetLayout::getDevice() const noexcept
    {
        return _descriptor_set_layout_bindings.getDevice();
    }

    inline VkDescriptorSetLayout DescriptorSetLayout::getDescriptorSetLayoutHandle() const noexcept
    {
        return _descriptor_set_layout_handle;
    }

    inline DescriptorSetLayoutBindings& DescriptorSetLayout::getDescriptorSetLayoutBindings() noexcept
    {
        return _descriptor_set_layout_bindings;
    }

    inline const DescriptorSetLayoutBindings& DescriptorSetLayout::getDescriptorSetLayoutBindings() const noexcept
    {
        return _descriptor_set_layout_bindings;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inline PipelineLayout::PipelineLayout(const vector<shared_ptr<DescriptorSetLayout>>& descriptor_set_layouts) :
        _descriptor_set_layouts(descriptor_set_layouts),
        _push_constant_ranges(0),
        _pipeline_layout_handle(VK_NULL_HANDLE)
    {
        _create_pipeline_layout();
    }
        
    inline PipelineLayout::PipelineLayout(vector<shared_ptr<DescriptorSetLayout>>&& descriptor_set_layouts) :
        _descriptor_set_layouts(move(descriptor_set_layouts)),
        _push_constant_ranges(0),
        _pipeline_layout_handle(VK_NULL_HANDLE)
    {
        _create_pipeline_layout();
    }
    
    inline PipelineLayout::PipelineLayout(const vector<shared_ptr<DescriptorSetLayout>>& descriptor_set_layouts,
                                          const vector<VkPushConstantRange>& push_constant_ranges) :
        _descriptor_set_layouts(descriptor_set_layouts),
        _push_constant_ranges(push_constant_ranges),
        _pipeline_layout_handle(VK_NULL_HANDLE)
    {
        _create_pipeline_layout();
    }
    
    inline PipelineLayout::PipelineLayout(vector<shared_ptr<DescriptorSetLayout>>&& descriptor_set_layouts,
                                          const vector<VkPushConstantRange>& push_constant_ranges) :
        _descriptor_set_layouts(move(descriptor_set_layouts)),
        _push_constant_ranges(push_constant_ranges),
        _pipeline_layout_handle(VK_NULL_HANDLE)
    {
        _create_pipeline_layout();
    }
    
    inline PipelineLayout::PipelineLayout(const vector<shared_ptr<DescriptorSetLayout>>& descriptor_set_layouts,
                                          vector<VkPushConstantRange>&& push_constant_ranges) :
        _descriptor_set_layouts(descriptor_set_layouts),
        _push_constant_ranges(move(push_constant_ranges)),
        _pipeline_layout_handle(VK_NULL_HANDLE)
    {
        _create_pipeline_layout();
    }
    
    inline PipelineLayout::PipelineLayout(vector<shared_ptr<DescriptorSetLayout>>&& descriptor_set_layouts,
                                          vector<VkPushConstantRange>&& push_constant_ranges) :
        _descriptor_set_layouts(move(descriptor_set_layouts)),
        _push_constant_ranges(move(push_constant_ranges)),
        _pipeline_layout_handle(VK_NULL_HANDLE)
    {
        _create_pipeline_layout();
    }

    inline PipelineLayout::PipelineLayout(PipelineLayout&& pipeline_layout) :
        _descriptor_set_layouts(move(pipeline_layout._descriptor_set_layouts)),
        _push_constant_ranges(move(pipeline_layout._push_constant_ranges)),
        _pipeline_layout_handle(VK_NULL_HANDLE)
    {
        swap(_pipeline_layout_handle, pipeline_layout._pipeline_layout_handle);
    }

    inline PipelineLayout::~PipelineLayout() noexcept
    {
        if (_pipeline_layout_handle != VK_NULL_HANDLE) {
            vkDestroyPipelineLayout(getDevice()->getDeviceHandle(), _pipeline_layout_handle, nullptr);
        }
    }

    inline void PipelineLayout::_create_pipeline_layout()
    {
        vector<VkDescriptorSetLayout> set_layout_handles (_descriptor_set_layouts.size());

        for (size_t i{0}; i < _descriptor_set_layouts.size(); i++) {
            set_layout_handles[i] = _descriptor_set_layouts[i]->getDescriptorSetLayoutHandle();
        }

        VkPipelineLayoutCreateInfo pipeline_layout_info {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
            .pNext = 0,
            .flags = 0,
            .setLayoutCount = static_cast<uint32_t>(set_layout_handles.size()),
            .pSetLayouts = set_layout_handles.data(),
            .pushConstantRangeCount = static_cast<uint32_t>(_push_constant_ranges.size()),
            .pPushConstantRanges = _push_constant_ranges.data()
        };

        assert(vkCreatePipelineLayout(getDevice()->getDeviceHandle(), &pipeline_layout_info, nullptr, &_pipeline_layout_handle) == VK_SUCCESS);
        assert(_pipeline_layout_handle != VK_NULL_HANDLE);
    }

    inline shared_ptr<Device>& PipelineLayout::getDevice() noexcept
    {
        return _descriptor_set_layouts[0]->getDevice();
    }

    inline const shared_ptr<Device>& PipelineLayout::getDevice() const noexcept
    {
        return _descriptor_set_layouts[0]->getDevice();
    }

    inline vector<shared_ptr<DescriptorSetLayout>>& PipelineLayout::getDescriptorSetLayouts() noexcept
    {
        return _descriptor_set_layouts;
    }

    inline const vector<shared_ptr<DescriptorSetLayout>>& PipelineLayout::getDescriptorSetLayouts() const noexcept
    {
        return _descriptor_set_layouts;
    }

    inline const vector<VkPushConstantRange>& PipelineLayout::getPushConstantRanges() const noexcept
    {
        return _push_constant_ranges;
    }

    inline const VkPipelineLayout& PipelineLayout::getPipelineLayoutHandle() const noexcept
    {
        return _pipeline_layout_handle;
    }
}
