#pragma once

#include <vulkan/vulkan.h>

#include <vector>
#include <optional>

namespace pbrlib::backend::vk
{
    class Device;
}

namespace pbrlib::backend::vk::builders
{
    class PipelineLayout;
}

namespace pbrlib::backend::vk
{
    class PipelineLayout final
    {
        explicit PipelineLayout(Device& device);

        friend class builders::PipelineLayout;

    public:
        PipelineLayout(PipelineLayout&& layout);
        PipelineLayout(const PipelineLayout& layout) = delete;

        ~PipelineLayout();

        PipelineLayout& operator = (PipelineLayout&& layout);
        PipelineLayout& operator = (const PipelineLayout& layout) = delete;

        VkPipelineLayout handle = VK_NULL_HANDLE;

    private:
        Device& _device;
    };
}

namespace pbrlib::backend::vk::builders
{
    class PipelineLayout final
    {
    public:
        explicit PipelineLayout(Device& device) noexcept;

        PipelineLayout(PipelineLayout&& builder)      = delete;
        PipelineLayout(const PipelineLayout& builder) = delete;

        PipelineLayout& operator = (PipelineLayout&& builder)         = delete;
        PipelineLayout& operator = (const PipelineLayout& builder)    = delete;

        PipelineLayout& addSetLayout(VkDescriptorSetLayout layout_handle);
        PipelineLayout& pushConstant(const VkPushConstantRange& push_constant);

        [[nodiscard]] vk::PipelineLayout build();
        
    private:
        Device& _device;

        std::optional<VkPushConstantRange> _push_constant;

        std::vector<VkDescriptorSetLayout> _sets_layout;
    };

    class DescriptorSetLayout final
    {
    public:
        explicit DescriptorSetLayout(Device& device) noexcept;

        DescriptorSetLayout(DescriptorSetLayout&& layout)       = delete;
        DescriptorSetLayout(const DescriptorSetLayout& layout)  = delete;

        DescriptorSetLayout& operator = (DescriptorSetLayout&& layout)      = delete;
        DescriptorSetLayout& operator = (const DescriptorSetLayout& layout) = delete;

        DescriptorSetLayout& addBinding (
            uint32_t            binding,
            VkDescriptorType    desc_type,
            uint32_t            count,
            VkShaderStageFlags  stages
        );

        
        VkDescriptorSetLayout build();
    private:
        Device& _device;

        std::vector<VkDescriptorSetLayoutBinding> _bindings;
    };
}