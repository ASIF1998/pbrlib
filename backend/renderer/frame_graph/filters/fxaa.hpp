#pragma once

#include <backend/renderer/frame_graph/filters/filter.hpp>
#include <backend/renderer/vulkan/unique_handler.hpp>

namespace pbrlib::backend
{
    class FXAA;

    template<>
    struct AttachmentsTraits<FXAA> final
    {
        static constexpr auto metadata()
        {
            constexpr auto usage_flags = 
                VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT 
            |   VK_IMAGE_USAGE_SAMPLED_BIT 
            |   VK_IMAGE_USAGE_TRANSFER_SRC_BIT
            |   VK_IMAGE_USAGE_STORAGE_BIT;

            constexpr std::array metadata
            {
                AttachmentMetadata(result, VK_FORMAT_A2R10G10B10_UNORM_PACK32, usage_flags),
            };

            return metadata;
        };

        constexpr static auto result = "fxaa";
    };
}

namespace pbrlib::backend
{
    class FXAA final :
        public Filter
    {
        bool init(const RenderContext& context, uint32_t width, uint32_t height) override;
        
        void render(vk::CommandBuffer& command_buffer) override;
        
        VkPipelineStageFlags2 srcStage() const noexcept override;
        VkPipelineStageFlags2 dstStage() const noexcept override;

        std::pair<VkDescriptorSet, VkDescriptorSetLayout> resultDescriptorSet() const noexcept override;

        bool createPipeline();

    public:
        explicit FXAA(vk::Device& device);

    private:
        vk::PipelineLayoutHandle    _pipeline_layout_handle;
        vk::PipelineHandle          _pipeline_handle;

        vk::DescriptorSetLayoutHandle   _descriptor_set_layout_handle;
        vk::DescriptorSetHandle         _descriptor_set_handle;

        vk::SamplerHandle _sampler_handle;
    };
}