#include <backend/renderer/frame_graph/filters/fxaa.hpp>

namespace pbrlib::backend
{
    FXAA::FXAA(vk::Device& device) :
        Filter (device)
    { }

    bool FXAA::init(const RenderContext& context, uint32_t width, uint32_t height)
    {
        return true;
    }

    void FXAA::render(vk::CommandBuffer& command_buffer)
    {

    }

    VkPipelineStageFlags2 FXAA::srcStage() const noexcept
    {
        return VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT;
    }

    VkPipelineStageFlags2 FXAA::dstStage() const noexcept
    {
        return VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT;
    }

    std::pair<VkDescriptorSet, VkDescriptorSetLayout> FXAA::resultDescriptorSet() const noexcept
    {
        return std::make_pair(VK_NULL_HANDLE, VK_NULL_HANDLE);
    }
}