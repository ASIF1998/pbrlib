#include <backend/renderer/vulkan/unique_handler.hpp>

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

    void HandleDispatcher::destroy(VkCommandBuffer command_buffer_handle, VkCommandPool command_pool_handle) noexcept
    {
        vkFreeCommandBuffers(_device_handle, command_pool_handle, 1, &command_buffer_handle);
    }
}