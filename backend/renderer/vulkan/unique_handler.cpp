#include <backend/renderer/vulkan/unique_handler.hpp>
#include <backend/logger/logger.hpp>

#include <pbrlib/exceptions.hpp>

namespace pbrlib::backend::vk
{
    VkInstance      HandleDispatcher::_instance_handle  = VK_NULL_HANDLE;
    VkDevice        HandleDispatcher::_device_handle    = VK_NULL_HANDLE;
    VmaAllocator    HandleDispatcher::_allocator_handle = VK_NULL_HANDLE;
}

namespace pbrlib::backend::vk
{
    void HandleDispatcher::initForDeviceResources(VkInstance instance_handle, VkDevice device_handle, VmaAllocator allocator_handle)
    {
        if (instance_handle == VK_NULL_HANDLE) [[unlikely]]
            throw exception::InvalidArgument("[vk-handle-dispatcher] instance handle is null");

        if (device_handle == VK_NULL_HANDLE) [[unlikely]]
            throw exception::InvalidArgument("[vk-handle-dispatcher] device handle is null");

        if (allocator_handle == VK_NULL_HANDLE) [[unlikely]]
            throw exception::InvalidArgument("[vk-handle-dispatcher] allocator handle is null");

        _instance_handle    = instance_handle;
        _device_handle      = device_handle;
        _allocator_handle   = allocator_handle;
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

    void HandleDispatcher::destroy(VkFramebuffer framebuffer_handle) noexcept
    {
        vkDestroyFramebuffer(_device_handle, framebuffer_handle, nullptr);
    }

    void HandleDispatcher::destroy(VkCommandPool command_pool_handle) noexcept
    {
        vkDestroyCommandPool(_device_handle, command_pool_handle, nullptr);
    }

    void HandleDispatcher::destroy(VkBuffer buffer_handle, VmaAllocation allocation_handle) noexcept
    {
        vmaDestroyBuffer(_allocator_handle, buffer_handle, allocation_handle);
    }

    void HandleDispatcher::destroy(VkImage image_handle, VmaAllocation allocation_handle, bool is_own) noexcept
    {
        // The is_own flag exists because the image may belong to the swapchain,
        // in which case it must not (and cannot) be destroyed manually.
        if (is_own) [[likely]]
            vmaDestroyImage(_allocator_handle, image_handle, allocation_handle);
    }

    void HandleDispatcher::destroy(VkImageView image_view_handle) noexcept
    {
        vkDestroyImageView(_device_handle, image_view_handle, nullptr);
    }
    
    void HandleDispatcher::destroy(VkSurfaceKHR surface_handle) noexcept
    {
        vkDestroySurfaceKHR(_instance_handle, surface_handle, nullptr);
    }

    void HandleDispatcher::destroy(VkSwapchainKHR swapchain_handle) noexcept
    {
        vkDestroySwapchainKHR(_device_handle, swapchain_handle, nullptr);
    }
}