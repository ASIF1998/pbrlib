#include <backend/renderer/vulkan/unique_handler.hpp>
#include <backend/logger/logger.hpp>

#include <pbrlib/exceptions.hpp>

namespace pbrlib::backend::vk
{
    VkInstance      ResourceDestroyer::_instance_handle     = VK_NULL_HANDLE;
    VkDevice        ResourceDestroyer::_device_handle       = VK_NULL_HANDLE;
    VmaAllocator    ResourceDestroyer::_allocator_handle    = VK_NULL_HANDLE;
}

namespace pbrlib::backend::vk
{
    void ResourceDestroyer::initForDeviceResources(VkInstance instance_handle, VkDevice device_handle, VmaAllocator allocator_handle)
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
    
    void ResourceDestroyer::destroy(VkInstance instance_handle) noexcept
    {
        vkDestroyInstance(instance_handle, nullptr);
    }

    void ResourceDestroyer::destroy(VkDevice device_handle) noexcept
    {
        vkDestroyDevice(device_handle, nullptr);
    }

    void ResourceDestroyer::destroy(VkShaderModule shader_module_handle) noexcept
    {
        vkDestroyShaderModule(_device_handle, shader_module_handle, nullptr);
    }

    void ResourceDestroyer::destroy(VkDescriptorSetLayout descriptor_set_layout_handle) noexcept
    {
        vkDestroyDescriptorSetLayout(_device_handle, descriptor_set_layout_handle, nullptr);
    }

    void ResourceDestroyer::destroy(VkDescriptorPool descriptor_pool_handle) noexcept
    {
        if (descriptor_pool_handle != VK_NULL_HANDLE)
            vkDestroyDescriptorPool(_device_handle, descriptor_pool_handle, nullptr);
    }
    
    void ResourceDestroyer::destroy(VkDescriptorSet descriptor_set_handle, VkDescriptorPool descriptor_pool_handle) noexcept
    {
        if (
                descriptor_set_handle != VK_NULL_HANDLE 
            &&  vkFreeDescriptorSets(_device_handle, descriptor_pool_handle, 1, &descriptor_set_handle) != VK_SUCCESS
        ) [[unlikely]]
            log::error("[vk-handle-dispatcher] failed free vulkan descriptor set: {}", reinterpret_cast<uint64_t>(descriptor_set_handle));
    }

    void ResourceDestroyer::destroy(VkCommandBuffer command_buffer_handle, VkCommandPool command_pool_handle) noexcept
    {
        vkFreeCommandBuffers(_device_handle, command_pool_handle, 1, &command_buffer_handle);
    }

    void ResourceDestroyer::destroy(VkPipelineLayout pipeline_layout_handle) noexcept
    {
        vkDestroyPipelineLayout(_device_handle, pipeline_layout_handle, nullptr);
    }

    void ResourceDestroyer::destroy(VkPipeline pipeline_handle) noexcept
    {
        vkDestroyPipeline(_device_handle, pipeline_handle, nullptr);
    }

    void ResourceDestroyer::destroy(VkSampler sampler_handle) noexcept
    {
        vkDestroySampler(_device_handle, sampler_handle, nullptr);
    }

    void ResourceDestroyer::destroy(VmaAllocator allocator_handle) noexcept
    {
        vmaDestroyAllocator(allocator_handle);
    }

    void ResourceDestroyer::destroy(VkRenderPass render_pass_handle) noexcept
    {
        vkDestroyRenderPass(_device_handle, render_pass_handle, nullptr);
    }

    void ResourceDestroyer::destroy(VkFramebuffer framebuffer_handle) noexcept
    {
        vkDestroyFramebuffer(_device_handle, framebuffer_handle, nullptr);
    }

    void ResourceDestroyer::destroy(VkCommandPool command_pool_handle) noexcept
    {
        vkDestroyCommandPool(_device_handle, command_pool_handle, nullptr);
    }

    void ResourceDestroyer::destroy(VkBuffer buffer_handle, VmaAllocation allocation_handle) noexcept
    {
        vmaDestroyBuffer(_allocator_handle, buffer_handle, allocation_handle);
    }

    void ResourceDestroyer::destroy(VkImage image_handle, VmaAllocation allocation_handle, bool is_own) noexcept
    {
        // The is_own flag exists because the image may belong to the swapchain,
        // in which case it must not (and cannot) be destroyed manually.
        if (is_own) [[likely]]
            vmaDestroyImage(_allocator_handle, image_handle, allocation_handle);
    }

    void ResourceDestroyer::destroy(VkImageView image_view_handle) noexcept
    {
        vkDestroyImageView(_device_handle, image_view_handle, nullptr);
    }
    
    void ResourceDestroyer::destroy(VkSurfaceKHR surface_handle) noexcept
    {
        vkDestroySurfaceKHR(_instance_handle, surface_handle, nullptr);
    }

    void ResourceDestroyer::destroy(VkSwapchainKHR swapchain_handle) noexcept
    {
        vkDestroySwapchainKHR(_device_handle, swapchain_handle, nullptr);
    }

    void ResourceDestroyer::destroy(VkFence fence_handle) noexcept
    {
        vkDestroyFence(_device_handle, fence_handle, nullptr);
    }

    void ResourceDestroyer::destroy(VkSemaphore semaphore_handle) noexcept
    {
        vkDestroySemaphore(_device_handle, semaphore_handle, nullptr);
    }

#ifdef PBRLIB_ENABLE_PROPFILING
    void ResourceDestroyer::destroy(TracyVkCtx tracy_ctx_handle) noexcept
    {
        if (tracy_ctx_handle)
            TracyVkDestroy(tracy_ctx_handle);
    }
#endif
}