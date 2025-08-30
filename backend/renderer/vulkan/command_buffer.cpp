#include <backend/renderer/vulkan/config.hpp>

#include <backend/utils/vulkan.hpp>
#include <backend/renderer/vulkan/command_buffer.hpp>

#include <backend/renderer/vulkan/device.hpp>

#include <pbrlib/exceptions.hpp>

#include <algorithm>

namespace pbrlib::backend::vk
{
    CommandBuffer::CommandBuffer(const Device& device, VkCommandPool command_pool_handle) :
        _device                 (device),
        _command_pool_handle    (command_pool_handle)
    { 
        const VkCommandBufferAllocateInfo alloc_info = 
        { 
            .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .commandPool        = _command_pool_handle,
            .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount = 1
        };

        level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        
        VK_CHECK(vkAllocateCommandBuffers(_device.device(), &alloc_info, &handle));
    }

    CommandBuffer::CommandBuffer(CommandBuffer&& command_buffer) noexcept :
        level                   (command_buffer.level),
        _device                 (command_buffer._device),
        _is_recording_started   (command_buffer._is_recording_started)
    {
        std::swap(command_buffer.handle, handle);
        std::swap(command_buffer._command_pool_handle, _command_pool_handle);
    }

    CommandBuffer::~CommandBuffer()
    {
        vkFreeCommandBuffers(
            _device.device(),
            _command_pool_handle, 
            1, &handle
        );
    }

    CommandBuffer& CommandBuffer::operator = (CommandBuffer&& command_buffer) noexcept
    {
        level                   = command_buffer.level;
        _is_recording_started   = command_buffer._is_recording_started;

        std::swap(handle, command_buffer.handle);
        std::swap(_command_pool_handle, command_buffer._command_pool_handle);

        return *this;
    }

    void CommandBuffer::write (
        const WriteFunctionType&    callback,
        std::string_view            name,
        const pbrlib::math::vec3&   col
    )
    {
        if (!callback)
            throw exception::InvalidArgument("[vulkan::command-buffer] callback is empty");

        if (!_is_recording_started)
        {
            constexpr VkCommandBufferBeginInfo begin_info 
            { 
                .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
                .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
            };

            VK_CHECK(vkBeginCommandBuffer(handle, &begin_info));

            _is_recording_started = true;
        }

        VkDebugMarkerMarkerInfoEXT marker_info 
        { 
            .sType       = VK_STRUCTURE_TYPE_DEBUG_MARKER_MARKER_INFO_EXT,
            .pMarkerName = name.data()
        };

        if (col.lengthSquared() > 0.0)
        {
            marker_info.color[0] = col.r;
            marker_info.color[1] = col.g;
            marker_info.color[2] = col.b;
            marker_info.color[3] = 1.0f;
        }

        const auto& functions = _device.vulkanFunctions();

        if (functions.vkCmdDebugMarkerBeginEXT)
            functions.vkCmdDebugMarkerBeginEXT(handle, &marker_info);
           
        callback(handle);

        if (functions.vkCmdDebugMarkerEndEXT)
            functions.vkCmdDebugMarkerEndEXT(handle);
    }
}