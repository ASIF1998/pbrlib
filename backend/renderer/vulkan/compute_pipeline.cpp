#include <backend/renderer/vulkan/compute_pipeline.hpp>
#include <backend/renderer/vulkan/device.hpp>
#include <backend/renderer/vulkan/shader_compiler.hpp>

#include <backend/utils/vulkan.hpp>

#include <pbrlib/exceptions.hpp>

namespace pbrlib::backend::vk::builders
{
    ComputePipeline::ComputePipeline(Device& device) noexcept :
        _device (device)
    { }

    ComputePipeline& ComputePipeline::shader(const std::filesystem::path& shader_name)
    {
        _shader_name = shader_name;
        return *this;
    }

    ComputePipeline& ComputePipeline::pipelineLayoutHandle(VkPipelineLayout layout_handle) noexcept
    {
        _pipeline_layout_handle = layout_handle;
        return *this;
    }

    VkPipeline ComputePipeline::build()
    {
        if (_pipeline_layout_handle == VK_NULL_HANDLE)
            throw exception::InvalidState("[vk-compute-pipeline-builder] pipeline layout handle is null");

        const VkPipelineShaderStageCreateInfo stage 
        {
            .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .stage  = VK_SHADER_STAGE_COMPUTE_BIT,
            .module = shader::compile(_device, _shader_name),
            .pName  = "main"
        };

        const VkComputePipelineCreateInfo pipeline_info 
        {
            .sType  = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,
            .stage  = stage,
            .layout = _pipeline_layout_handle
        };

        VkPipeline pipeline_handle = VK_NULL_HANDLE;

        VK_CHECK(vkCreateComputePipelines(
            _device.device(),
            VK_NULL_HANDLE,
            1, &pipeline_info,
            nullptr, 
            &pipeline_handle
        ));

        vkDestroyShaderModule(_device.device(), stage.module, nullptr);
        
        return pipeline_handle;
    }
}