#pragma once

#include <backend/renderer/vulkan/unique_handler.hpp>

#include <limits>

namespace pbrlib::backend::vk
{
    [[nodiscard]] SemaphoreHandle create(VkDevice device_handle, const VkSemaphoreCreateInfo& create_info);
    [[nodiscard]] FenceHandle     create(VkDevice device_handle, const VkFenceCreateInfo& create_info);

    void sync(VkDevice device_handle, VkFence fence_handle, uint64_t timeout = std::numeric_limits<uint64_t>::max());
}
