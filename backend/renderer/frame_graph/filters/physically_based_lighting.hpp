#pragma once

#include <backend/renderer/frame_graph/filters/filter.hpp>

namespace pbrlib::backend
{
    class PhysicallyBasedLighting final :
        public Filter
    {
        bool init(const RenderContext& context, uint32_t width, uint32_t height) override;

        void render(vk::CommandBuffer& command_buffer) override;

        VkPipelineStageFlags2 srcStage() const noexcept override;
        VkPipelineStageFlags2 dstStage() const noexcept override;

        std::pair<VkDescriptorSet, VkDescriptorSetLayout> resultDescriptorSet() const noexcept override;

    public:
        explicit PhysicallyBasedLighting(vk::Device& device, vk::Image& dst_image) noexcept;

    private:
        vk::PipelineLayoutHandle    _pipeline_layout_handle;
        vk::PipelineHandle          _pipeline_handle;
    };
}
