#pragma once

#include <backend/renderer/frame_graph/render_pass.hpp>

#include <vector>
#include <memory>

namespace pbrlib::backend
{
    template<typename T>
    concept IsRenderPass = std::is_base_of_v<RenderPass, T>;

    class CompoundRenderPass :
        public RenderPass
    {
        bool init(const RenderContext& context, uint32_t width, uint32_t height)    override;
        void render(vk::CommandBuffer& command_buffer)                              override;
        void draw(vk::CommandBuffer& command_buffer)                                override;
        
        VkPipelineStageFlags2 srcStage() const noexcept override;
        VkPipelineStageFlags2 dstStage() const noexcept override;

        std::pair<VkDescriptorSet, VkDescriptorSetLayout> resultDescriptorSet() const noexcept override;

    public:
        explicit CompoundRenderPass(vk::Device& device) noexcept;

        template<IsRenderPass T>
        void add(std::unique_ptr<T>&& ptr_subpass)
        {
            _subpasses.push_back(std::move(ptr_subpass));
        }

    private:
        std::vector<std::unique_ptr<RenderPass>> _subpasses;

        VkDescriptorSet         _descriptor_set_handle          = VK_NULL_HANDLE;
        VkDescriptorSetLayout   _descriptor_set_layout_handle   = VK_NULL_HANDLE;
    };
}   