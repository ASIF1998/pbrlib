#pragma once

#include <vulkan/vulkan.h>

namespace pbrlib::vk
{
    class Device;
    class Image;
}

namespace pbrlib
{
    class SceneItem;
}

namespace pbrlib
{
    struct IRenderPass
    {
        IRenderPass() = default;

        IRenderPass(IRenderPass&& render_pass)      = delete;
        IRenderPass(const IRenderPass& render_pass) = delete;

        virtual ~IRenderPass() = default;

        IRenderPass& operator = (IRenderPass&& render_pass)         = delete;
        IRenderPass& operator = (const IRenderPass& render_pass)    = delete;

        virtual bool init(const vk::Device* ptr_device) = 0;
        virtual void render(const SceneItem* ptr_item)  = 0; 
    };
}