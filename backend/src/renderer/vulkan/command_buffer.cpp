#include <backend/renderer/vulkan/config.hpp>

#include <backend/utils/vulkan.hpp>
#include <backend/renderer/vulkan/command_buffer.hpp>

#include <backend/renderer/vulkan/device.hpp>

#include <algorithm>

#include <stdexcept>

namespace pbrlib::vk
{
    CommandBuffer::CommandBuffer(const Device* ptr_device) :
        _ptr_device(ptr_device)
    { }

    CommandBuffer::CommandBuffer(CommandBuffer&& command_buffer)
    {
        std::swap(command_buffer.handle, handle);
    }

    CommandBuffer& CommandBuffer::operator = (CommandBuffer&& command_buffer)
    {
        std::swap(handle, command_buffer.handle);
        return *this;
    }

    void CommandBuffer::write(
        const WriteFunctionType&    callback,
        std::string_view            name,
        const pbrlib::math::vec3&   col
    )
    {
        if (!callback)
            throw std::invalid_argument("[Vulkan::CommandBuffer] Callback is empty");

        VkCommandBufferBeginInfo begin_info = { };
        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        VkDebugMarkerMarkerInfoEXT marker_info = { };
        marker_info.sType       = VK_STRUCTURE_TYPE_DEBUG_MARKER_MARKER_INFO_EXT;
        marker_info.pMarkerName = name.data();

        if (col.lengthSquared() > 0.0)
        {
            marker_info.color[0] = col.r;
            marker_info.color[1] = col.g;
            marker_info.color[2] = col.b;
            marker_info.color[3] = 1.0f;
        }

        const auto& functions = _ptr_device->vulkanFunctions();

        VK_CHECK(vkBeginCommandBuffer(handle, &begin_info));

        if (config::enable_vulkan_debug_marker)
            functions.vkCmdDebugMarkerBeginEXT(handle, &marker_info);
           
        callback(handle);

        if (config::enable_vulkan_debug_marker)
            functions.vkCmdDebugMarkerEndEXT(handle);

        vkEndCommandBuffer(handle);
    }
}