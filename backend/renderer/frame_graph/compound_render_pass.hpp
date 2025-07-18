#pragma once

#include <backend/renderer/frame_graph/render_pass.hpp>

#include <vector>
#include <memory>

namespace pbrlib::backend
{
    template<typename T>
    concept IsRenderPass = requires(T render_pass)
    {
        dynamic_cast<RenderPass*>(&render_pass);
    };

    class CompoundRenderPass :
        public RenderPass
    {
        bool init(const RenderContext& context, uint32_t width, uint32_t height)    override;
        bool rebuild(uint32_t width, uint32_t height)                               override;
        void render(vk::CommandBuffer& command_buffer)                              override;
        void draw(vk::CommandBuffer& command_buffer)                                override;
        
        VkPipelineStageFlags2 srcStage() const noexcept override;
        VkPipelineStageFlags2 dstStage() const noexcept override;

        std::pair<VkDescriptorSet, VkDescriptorSetLayout> resultDescriptorSet() const noexcept override;

    public:
        explicit CompoundRenderPass(vk::Device& device) noexcept;

        template<IsRenderPass T>
        void add(std::unique_ptr<T>&& subpass)
        {
            _subpasses.push_back(std::move(subpass));
        }

    private:
        std::vector<std::unique_ptr<RenderPass>> _subpasses;
    };
}   