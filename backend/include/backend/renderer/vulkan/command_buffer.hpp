#pragma once

#include <pbrlib/math/vec3.hpp>

#include <vulkan/vulkan.h>

#include <functional>

#include <string_view>

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

        CommandBuffer(const Device* ptr_device);

        using WriteFunctionType = std::function<void (VkCommandBuffer command_buffer)>;

    public:
        CommandBuffer(CommandBuffer&& command_buffer);
        CommandBuffer(const CommandBuffer& command_buffer) = delete;

        CommandBuffer& operator = (CommandBuffer&& command_buffer);
        CommandBuffer& operator = (const CommandBuffer& command_buffer) = delete;

        void write(
            const WriteFunctionType&    callback,
            std::string_view            name    = "",
            const pbrlib::math::vec3&   col     = pbrlib::math::vec3(0)
        );

        VkCommandBuffer         handle  = VK_NULL_HANDLE;
        VkCommandBufferLevel    level   = VK_COMMAND_BUFFER_LEVEL_PRIMARY; 

    private:
        const Device* _ptr_device;
    };
}