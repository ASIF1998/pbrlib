#include <backend/renderer/vulkan/sync.hpp>
#include <backend/renderer/vulkan/check.hpp>

#include <pbrlib/exceptions.hpp>

namespace pbrlib::backend::vk
{
    vk::SemaphoreHandle create(VkDevice device_handle, const VkSemaphoreCreateInfo& create_info)
    {
        VkSemaphore semaphore_handle = VK_NULL_HANDLE;

        VK_CHECK(vkCreateSemaphore (
            device_handle,
            &create_info,
            nullptr,
            &semaphore_handle
        ));

        if (semaphore_handle == VK_NULL_HANDLE) [[unlikely]]
            throw pbrlib::exception::InitializeError("failed create semaphore");

        return vk::SemaphoreHandle(semaphore_handle);
    }

    vk::FenceHandle create(VkDevice device_handle, const VkFenceCreateInfo& create_info)
    {
        VkFence fence_handle = VK_NULL_HANDLE;

        VK_CHECK(vkCreateFence (
            device_handle,
            &create_info,
            nullptr,
            &fence_handle
        ));

        if (fence_handle == VK_NULL_HANDLE) [[unlikely]]
            throw pbrlib::exception::InitializeError("failed create fence");

        return vk::FenceHandle(fence_handle);
    }

    void sync(VkDevice device_handle, VkFence fence_handle, uint64_t timeout)
    {
        VK_CHECK(vkWaitForFences (
            device_handle, 
            1, &fence_handle,
            VK_TRUE,
            timeout
        ));

        VK_CHECK(vkResetFences(device_handle, 1, &fence_handle));
    }
}