#pragma once

#include <vulkan/vulkan.h>

#include <vector>
#include <optional>

namespace pbrlib::backend::vk
{
    class Device;
}

namespace pbrlib::backend::vk
{
    class PipelineLayout final
    {
        explicit PipelineLayout(Device& device);

    public:
        class Builder;

        PipelineLayout(PipelineLayout&& layout);
        PipelineLayout(const PipelineLayout& layout) = delete;

        ~PipelineLayout();

        PipelineLayout& operator = (PipelineLayout&& layout);
        PipelineLayout& operator = (const PipelineLayout& layout) = delete;

        VkPipelineLayout handle = VK_NULL_HANDLE;

        std::vector<VkDescriptorSetLayout> sets_layout;

    private:
        Device& _device;
    };

    class PipelineLayout::Builder final
    {
    public:
        explicit Builder(Device& device);

        Builder(Builder&& builder)      = delete;
        Builder(const Builder& builder) = delete;

        Builder& operator = (Builder&& builder)         = delete;
        Builder& operator = (const Builder& builder)    = delete;

        Builder& addSet();
        
        Builder& addBinding (
            uint32_t            binding, 
            VkDescriptorType    desc_type, 
            uint32_t            count, 
            VkShaderStageFlags  stages
        );

        Builder& pushConstant(const VkPushConstantRange& push_constant);

        [[nodiscard]] PipelineLayout build();
        
    private:
        Device& _device;

        std::optional<VkPushConstantRange> _push_constant;

        std::vector<std::vector<VkDescriptorSetLayoutBinding>> _sets;
    };
}