#pragma once

#include <backend/renderer/vulkan/pipeline_layout.hpp>
#include <backend/renderer/vulkan/buffer.hpp>
#include <backend/renderer/frame_graph/render_pass.hpp>
#include <backend/renderer/vulkan/descriptor_group.hpp>

#include <cstdint>
#include <pbrlib/math/vec2.hpp>
#include <pbrlib/math/matrix4x4.hpp>
#include <pbrlib/event_system.hpp>

#include <optional>
#include <array>

namespace pbrlib::backend
{
    class BilateralBlur;
}

namespace pbrlib::backend
{
    class SSAO;

    template<>
    struct AttachmentsTraits<SSAO> final
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
                AttachmentMetadata(ssao, VK_FORMAT_R16_SFLOAT, usage_flags),
                AttachmentMetadata(blur, VK_FORMAT_R16_SFLOAT, usage_flags)
            };

            return metadata;
        };

        constexpr static auto ssao = "ssao-result";
        constexpr static auto blur = "ssao-blur";
    };
}

namespace pbrlib::backend
{
    class SSAO final :
        public RenderPass,
        public pbrlib::EventSystem
    {
        struct alignas(16) Params final
        {
            float               radius          = 0.05f;
            uint32_t            sample_count    = 0;
            pbrlib::math::vec2  noise_scale;
        };

        bool init(const RenderContext& context, uint32_t width, uint32_t height) override;

        bool createPipeline(uint32_t width, uint32_t height);

        void render(vk::CommandBuffer& command_buffer) override;

        VkPipelineStageFlags2 srcStage() const noexcept override;
        VkPipelineStageFlags2 dstStage() const noexcept override;

        const vk::DescriptorGroup* resultDescriptorGroup() const noexcept override;

        void bindResultDescriptorSet();

        void createSSAODescriptorSet();

        void createParamsBuffer();
        void createSamplesBuffer();

    public:
        static constexpr uint32_t gbuffer_set_id = 0;

        explicit SSAO(vk::Device& device, BilateralBlur* ptr_blur);

    private:
        vk::PipelineLayoutHandle    _pipeline_layout_handle;
        vk::PipelineHandle          _pipeline_handle;

        std::optional<vk::DescriptorGroup> _result_descriptor_group;

        vk::SamplerHandle _result_image_sampler;

        std::optional<vk::DescriptorGroup> _ssao_descriptor_group;

        Params                      _params;
        std::optional<vk::Buffer>   _params_buffer;

        std::optional<vk::Buffer> _samples_buffer;

        BilateralBlur* _ptr_blur = nullptr;

        static constexpr auto final_attachments_layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    };
}
