#pragma once

#include <vulkan/vulkan.h>

#include <functional>

namespace pbrlib::vk
{
    class Device;
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

        [[maybe_unused]] [[nodiscard]] VkCommandBufferLevel level() const noexcept;

        void write(const std::function<void (VkCommandBuffer command_buffer)>& callback);

    private:
        VkCommandBuffer         _handle = VK_NULL_HANDLE;
        VkCommandBufferLevel    _level = VK_COMMAND_BUFFER_LEVEL_PRIMARY; 
    };
}