#include <backend/renderer/vulkan/device.hpp>
#include <backend/renderer/vulkan/pipeline_layout.hpp>

#include <backend/utils/vulkan.hpp>

#include <ranges>

#include <stdexcept>

namespace pbrlib::backend::vk
{
    PipelineLayout::PipelineLayout(Device& device) : 
        _device (device)
    { }

    PipelineLayout::PipelineLayout(PipelineLayout&& layout) :
        _device (layout._device)
    {
        std::swap(handle, layout.handle);
        std::swap(sets_layout, layout.sets_layout);
    }

    PipelineLayout::~PipelineLayout()
    {
        const auto device_handle = _device.device();

        for (const auto set_layout_handle : sets_layout)
            vkDestroyDescriptorSetLayout(device_handle, set_layout_handle, nullptr);

        vkDestroyPipelineLayout(device_handle, handle, nullptr);
    }

    PipelineLayout& PipelineLayout::operator = (PipelineLayout&& layout)
    {
        std::swap(handle, layout.handle);
        std::swap(sets_layout, layout.sets_layout);

        return *this;
    }
}

namespace pbrlib::backend::vk::builders
{
    PipelineLayout::PipelineLayout(Device& device) noexcept :
        _device (device)
    { }

    PipelineLayout& PipelineLayout::addSet()
    {
        _sets.emplace_back();
        return *this;
    }

    PipelineLayout& PipelineLayout::addBinding (
        uint32_t            binding, 
        VkDescriptorType    desc_type, 
        uint32_t            count, 
        VkShaderStageFlags  stages
    )
    {
        _sets.back().emplace_back
        (
            VkDescriptorSetLayoutBinding
            {
                .binding            = binding,
                .descriptorType     = desc_type,
                .descriptorCount    = count,
                .stageFlags         = stages
            }
        );

        return *this;
    }

    PipelineLayout& PipelineLayout::pushConstant(const VkPushConstantRange& push_constant)
    {
        _push_constant.emplace(push_constant);
        return *this;
    }

    vk::PipelineLayout PipelineLayout::build()
    {
        vk::PipelineLayout layout (_device);

        VkPipelineLayoutCreateInfo pipeline_layout_create_info
        {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO
        };

        if (!_sets.empty())
        {
            layout.sets_layout.resize(_sets.size());

            VkDescriptorSetLayoutCreateInfo desc_set_create_info
            {
                .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO
            };

            for (const auto i: std::views::iota(0u, _sets.size()))
            {
                desc_set_create_info.bindingCount   = static_cast<uint32_t>(_sets[i].size());
                desc_set_create_info.pBindings      = _sets[i].data();

                VK_CHECK(vkCreateDescriptorSetLayout(
                    _device.device(),
                    &desc_set_create_info,
                    nullptr,
                    &layout.sets_layout[i]
                ));
            }

            pipeline_layout_create_info.setLayoutCount  = static_cast<uint32_t>(layout.sets_layout.size());
            pipeline_layout_create_info.pSetLayouts     = layout.sets_layout.data();
        }

        if (_push_constant)
        {
            pipeline_layout_create_info.pushConstantRangeCount  = 1;
            pipeline_layout_create_info.pPushConstantRanges     = &_push_constant.value();
        }

        VK_CHECK(vkCreatePipelineLayout(
            _device.device(),
            &pipeline_layout_create_info,
            nullptr,
            &layout.handle
        ));

        return layout;
    }
}