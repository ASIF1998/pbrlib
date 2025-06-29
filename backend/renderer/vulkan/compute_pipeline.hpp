#pragma once

#include <vulkan/vulkan.h>

#include <filesystem>

namespace pbrlib::backend::vk
{
    class Device;
}

namespace pbrlib::backend::vk
{
    class ComputePipelineBuilder final
    {
    public:
        explicit ComputePipelineBuilder(Device& device) noexcept;

        ComputePipelineBuilder(ComputePipelineBuilder&& builder)        = delete;
        ComputePipelineBuilder(const ComputePipelineBuilder& builder)   = delete;

        ComputePipelineBuilder& operator = (ComputePipelineBuilder&& builder)       = delete;
        ComputePipelineBuilder& operator = (const ComputePipelineBuilder& builder)  = delete;

        ComputePipelineBuilder& shader(const std::filesystem::path& shader_name);
        ComputePipelineBuilder& pipelineLayoutHandle(VkPipelineLayout layout_handle) noexcept;

        [[nodiscard]] VkPipeline build();

    private:
        Device& _device;

        std::filesystem::path _shader_name;

        VkPipelineLayout _pipeline_layout_handle = VK_NULL_HANDLE; 
    };
}