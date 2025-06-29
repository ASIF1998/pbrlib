#pragma once

#include <vulkan/vulkan.h>

#include <filesystem>

namespace pbrlib::backend::vk
{
    class Device;
}

namespace pbrlib::backend::vk::builders
{
    class ComputePipeline final
    {
    public:
        explicit ComputePipeline(Device& device) noexcept;

        ComputePipeline(ComputePipeline&& builder)      = delete;
        ComputePipeline(const ComputePipeline& builder) = delete;

        ComputePipeline& operator = (ComputePipeline&& builder)         = delete;
        ComputePipeline& operator = (const ComputePipeline& builder)    = delete;

        ComputePipeline& shader(const std::filesystem::path& shader_name);
        ComputePipeline& pipelineLayoutHandle(VkPipelineLayout layout_handle) noexcept;

        [[nodiscard]] VkPipeline build();

    private:
        Device& _device;

        std::filesystem::path _shader_name;

        VkPipelineLayout _pipeline_layout_handle = VK_NULL_HANDLE; 
    };
}