#include <backend/renderer/vulkan/device.hpp>
#include <backend/renderer/vulkan/pipeline_layout.hpp>

#include <backend/utils/vulkan.hpp>

#include <pbrlib/exceptions.hpp>

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
    }

    PipelineLayout::~PipelineLayout()
    {
        const auto device_handle = _device.device();
        vkDestroyPipelineLayout(device_handle, handle, nullptr);
    }

    PipelineLayout& PipelineLayout::operator = (PipelineLayout&& layout)
    {
        std::swap(handle, layout.handle);
        return *this;
    }
}

namespace pbrlib::backend::vk::builders
{
    PipelineLayout::PipelineLayout(Device& device) noexcept :
        _device (device)
    { }

    PipelineLayout& PipelineLayout::addSetLayout(VkDescriptorSetLayout layout_handle)
    {
        _sets_layout.emplace_back(layout_handle);
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

        if (!_sets_layout.empty())
        {
            pipeline_layout_create_info.setLayoutCount  = static_cast<uint32_t>(_sets_layout.size());
            pipeline_layout_create_info.pSetLayouts     = _sets_layout.data();
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

namespace pbrlib::backend::vk::builders
{
    DescriptorSetLayout::DescriptorSetLayout(Device& device) noexcept :
        _device (device)
    { }

    DescriptorSetLayout& DescriptorSetLayout::addBinding (
        uint32_t            binding, 
        VkDescriptorType    desc_type, 
        uint32_t            count, 
        VkShaderStageFlags  stages
    )
    {
        _bindings.emplace_back
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

    VkDescriptorSetLayout DescriptorSetLayout::build()
    {
        if (_bindings.empty()) [[unlikely]]
            throw exception::InvalidState("[descritor-set-layout-builder] bindings count is 0");

        const VkDescriptorSetLayoutCreateInfo desc_set_create_info
        {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
            .bindingCount   = static_cast<uint32_t>(_bindings.size()),
            .pBindings      = _bindings.data()
        };

        VkDescriptorSetLayout sets_layout = VK_NULL_HANDLE;

        VK_CHECK(vkCreateDescriptorSetLayout(
            _device.device(),
            &desc_set_create_info,
            nullptr,
            &sets_layout
        ));

        return sets_layout;
    }
}