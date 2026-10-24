#pragma once

#include <backend/renderer/frame_graph/render_pass.hpp>
#include <backend/renderer/vulkan/unique_handler.hpp>

#include <pbrlib/event_system.hpp>

namespace pbrlib::backend
{
    class PhysicallyBasedLighting;

    template<>
    struct AttachmentsTraits<PhysicallyBasedLighting>
    {
        static constexpr auto metadata()
        {
            constexpr auto usage_flags =
                VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT
            |   VK_IMAGE_USAGE_SAMPLED_BIT
            |   VK_IMAGE_USAGE_TRANSFER_SRC_BIT
            |   VK_IMAGE_USAGE_TRANSFER_DST_BIT
            |   VK_IMAGE_USAGE_STORAGE_BIT;

            constexpr std::array metadata
            {
                AttachmentMetadata(dst_image, VK_FORMAT_R32G32B32A32_SFLOAT, usage_flags),
            };

            return metadata;
        }

        constexpr static auto dst_image = "pbl-dst-image";
    };
}

namespace pbrlib::backend
{
    class PhysicallyBasedLighting final :
        public RenderPass,
        public EventSystem
    {
        bool init(const RenderContext& context, uint32_t width, uint32_t height)    override;
        void render(vk::CommandBuffer& command_buffer)                              override;
        void sync(Transition& transition)                                           override;

        VkPipelineStageFlags2 srcStage() const noexcept override;
        VkPipelineStageFlags2 dstStage() const noexcept override;

        vk::DescriptorGroup*        resultDescriptorGroup() noexcept override;
        const vk::DescriptorGroup*  resultDescriptorGroup() const noexcept override;

        bool createPipeline();

    public:
        explicit PhysicallyBasedLighting(vk::Device& device, vk::Image& dst_image) noexcept;

    private:
        vk::PipelineLayoutHandle    _pipeline_layout_handle;
        vk::PipelineHandle          _pipeline_handle;
    };
}
