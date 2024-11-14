#pragma once

#include <vulkan/vulkan.h>

#include <optional>

namespace pbrlib
{
    struct IRenderPass
    {
        virtual ~IRenderPass() = default;

        virtual bool init()     = 0;
        virtual void shutdown() = 0;

        virtual void render(VkCommandBuffer command_buffer_handle) = 0;
    };
}