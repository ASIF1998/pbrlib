#include <backend/renderer/frame_graph/filters/physically_based_lighting.hpp>

namespace pbrlib::backend
{
    PhysicallyBasedLighting::PhysicallyBasedLighting(vk::Device& device, vk::Image& dst_image) noexcept :
        Filter("physically-based-lighting", device, dst_image)
    { }

    bool PhysicallyBasedLighting::init(const RenderContext& context, uint32_t width, uint32_t height)
    {
        /// @todo
        return true;
    }

    void PhysicallyBasedLighting::render(vk::CommandBuffer& command_buffer)
    {
        /// @todo
    }

    VkPipelineStageFlags2 PhysicallyBasedLighting::srcStage() const noexcept
    {
        /// @todo
        return VK_PIPELINE_STAGE_2_NONE;
    }

    VkPipelineStageFlags2 PhysicallyBasedLighting::dstStage() const noexcept
    {
        /// @todo
        return VK_PIPELINE_STAGE_2_NONE;
    }

    std::pair<VkDescriptorSet, VkDescriptorSetLayout> PhysicallyBasedLighting::resultDescriptorSet() const noexcept
    {
        /// @todo
        return std::make_pair(VK_NULL_HANDLE, VK_NULL_HANDLE);
    }
}
