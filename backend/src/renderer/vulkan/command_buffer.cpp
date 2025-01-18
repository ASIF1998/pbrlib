#include <backend/renderer/vulkan/command_buffer.hpp>
#include <backend/utils/vulkan.hpp>

#include <algorithm>

#include <stdexcept>

namespace pbrlib::vk
{
    CommandBuffer::CommandBuffer(CommandBuffer&& command_buffer)
    {
        std::swap(command_buffer.handle, handle);
    }

    CommandBuffer& CommandBuffer::operator = (CommandBuffer&& command_buffer)
    {
        std::swap(handle, command_buffer.handle);
        return *this;
    }

    void CommandBuffer::write(const std::function<void (VkCommandBuffer command_buffer)>& callback)
    {
        if (!callback)
            throw std::invalid_argument("[Vulkan::CommandBuffer] Callback is empty");

        VkCommandBufferBeginInfo begin_info = { };
        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        VK_CHECK(vkBeginCommandBuffer(handle, &begin_info));
        callback(handle);
        vkEndCommandBuffer(handle);
    }
}