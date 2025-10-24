#pragma once

#include <pbrlib/math/vec3.hpp>

#include <backend/renderer/vulkan/unique_handler.hpp>

#include <functional>

#include <string_view>

namespace pbrlib::backend::vk
{
    class   Device;
    struct  Queue;
}

namespace pbrlib::backend::vk
{
    class CommandBuffer final
    {
        friend class Device;

        using WriteFunctionType = std::function<void (VkCommandBuffer command_buffer)>;

        CommandBuffer(const Device& device, VkCommandPool command_pool_handle);

    public:
        CommandBuffer(CommandBuffer&& command_buffer) noexcept;
        CommandBuffer(const CommandBuffer& command_buffer) = delete;

        CommandBuffer& operator = (CommandBuffer&& command_buffer) noexcept;
        CommandBuffer& operator = (const CommandBuffer& command_buffer) = delete;

        void write (
            const WriteFunctionType&    callback,
            std::string_view            name    = "",
            const pbrlib::math::vec3&   col     = pbrlib::math::vec3(0)
        );

        CommandBufferHandle     handle;
        VkCommandBufferLevel    level   = VK_COMMAND_BUFFER_LEVEL_PRIMARY; 

    private:
        const Device&   _device;
        bool            _is_recording_started = false;
    };
}