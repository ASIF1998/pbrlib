#include <backend/renderer/vulkan/unique_handler.hpp>
#include <backend/logger/logger.hpp>

#include <pbrlib/exceptions.hpp>

namespace pbrlib::backend::vk
{
    VkDevice HandleDispatcher::_device_handle = VK_NULL_HANDLE;

    void HandleDispatcher::initForDeviceResources(VkDevice device_handle)
    {
        if (device_handle == VK_NULL_HANDLE)
            throw exception::InvalidArgument("[vk-handle-dispatcher] device handle is null");

        _device_handle = device_handle;
    }
    
    void HandleDispatcher::destroy(VkInstance instance_handle) noexcept
    {
        vkDestroyInstance(instance_handle, nullptr);
    }

    void HandleDispatcher::destroy(VkDevice device_handle) noexcept
    {
        vkDestroyDevice(device_handle, nullptr);
    }

    void HandleDispatcher::destroy(VkShaderModule shader_module_handle) noexcept
    {
        vkDestroyShaderModule(_device_handle, shader_module_handle, nullptr);
    }

    void HandleDispatcher::destroy(VkDescriptorSetLayout descriptor_set_layout_handle) noexcept
    {
        vkDestroyDescriptorSetLayout(_device_handle, descriptor_set_layout_handle, nullptr);
    }

    void HandleDispatcher::destroy(VkDescriptorPool descriptor_pool_handle) noexcept
    {
        if (descriptor_pool_handle != VK_NULL_HANDLE)
            vkDestroyDescriptorPool(_device_handle, descriptor_pool_handle, nullptr);
    }
    
    void HandleDispatcher::destroy(VkDescriptorSet descriptor_set_handle, VkDescriptorPool descriptor_pool_handle) noexcept
    {
        if (
                descriptor_set_handle != VK_NULL_HANDLE 
            &&  vkFreeDescriptorSets(_device_handle, descriptor_pool_handle, 1, &descriptor_set_handle) != VK_SUCCESS
        ) [[unlikely]]
            log::error("[vk-handle-dispatcher] failed free vulkan descriptor set: {}", reinterpret_cast<uint64_t>(descriptor_set_handle));
    }

    void HandleDispatcher::destroy(VkCommandBuffer command_buffer_handle, VkCommandPool command_pool_handle) noexcept
    {
        vkFreeCommandBuffers(_device_handle, command_pool_handle, 1, &command_buffer_handle);
    }

    void HandleDispatcher::destroy(VkPipelineLayout pipeline_layout_handle) noexcept
    {
        vkDestroyPipelineLayout(_device_handle, pipeline_layout_handle, nullptr);
    }

    void HandleDispatcher::destroy(VkPipeline pipeline_handle) noexcept
    {
        vkDestroyPipeline(_device_handle, pipeline_handle, nullptr);
    }

    void HandleDispatcher::destroy(VkSampler sampler_handle) noexcept
    {
        vkDestroySampler(_device_handle, sampler_handle, nullptr);
    }

    void HandleDispatcher::destroy(VmaAllocator allocator_handle) noexcept
    {
        vmaDestroyAllocator(allocator_handle);
    }

    void HandleDispatcher::destroy(VkRenderPass render_pass_handle) noexcept
    {
        vkDestroyRenderPass(_device_handle, render_pass_handle, nullptr);
    }
}