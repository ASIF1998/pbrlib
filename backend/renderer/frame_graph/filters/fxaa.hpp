#pragma once

#include <backend/renderer/frame_graph/filters/filter.hpp>
#include <backend/renderer/vulkan/unique_handler.hpp>

#include <array>

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
                AttachmentMetadata(result, VK_FORMAT_R16G16B16A16_SFLOAT, usage_flags),
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
        struct Config final
        {
            float span_max      = 8.0;
            float reduce_min    = 1.0 / 128.0;
            float reduce_mul    = 1.0 / 8.0;
        };

        bool init(const RenderContext& context, uint32_t width, uint32_t height) override;

        void render(vk::CommandBuffer& command_buffer) override;

        VkPipelineStageFlags2 srcStage() const noexcept override;
        VkPipelineStageFlags2 dstStage() const noexcept override;

        std::pair<VkDescriptorSet, VkDescriptorSetLayout> resultDescriptorSet() const noexcept override;

        bool createPipeline();

    public:
        explicit FXAA(vk::Device& device, vk::Image& dst_image);

    private:
        vk::PipelineLayoutHandle    _pipeline_layout_handle;
        vk::PipelineHandle          _pipeline_handle;

        Config _config;
    };
}
