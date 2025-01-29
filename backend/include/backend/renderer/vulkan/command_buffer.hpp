#pragma once

#include <vulkan/vulkan.h>

#include <functional>

namespace pbrlib::vk
{
    class   Device;
    struct  Queue;
}

namespace pbrlib::vk
{
    class CommandBuffer final
    {
        friend class Device;

        CommandBuffer() = default;

    public:
        CommandBuffer(CommandBuffer&& command_buffer);
        CommandBuffer(const CommandBuffer& command_buffer) = delete;

        CommandBuffer& operator = (CommandBuffer&& command_buffer);
        CommandBuffer& operator = (const CommandBuffer& command_buffer) = delete;

        void write(const std::function<void (VkCommandBuffer command_buffer)>& callback);

        VkCommandBuffer         handle  = VK_NULL_HANDLE;
        VkCommandBufferLevel    level   = VK_COMMAND_BUFFER_LEVEL_PRIMARY; 
    };
}