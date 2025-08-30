#pragma once

#include <backend/renderer/vulkan/pipeline_layout.hpp>
#include <backend/renderer/vulkan/buffer.hpp>

#include <backend/renderer/frame_graph/render_pass.hpp>

#include <pbrlib/math/vec2.hpp>
#include <pbrlib/math/matrix4x4.hpp>

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
            |   VK_IMAGE_USAGE_STORAGE_BIT;

            /// @todo VK_FORMAT_R16_SFLOAT

            constexpr std::array metadata
            {
                AttachmentMetadata(ssao, VK_FORMAT_R32G32B32A32_SFLOAT, usage_flags),
                AttachmentMetadata(blur, VK_FORMAT_R32G32B32A32_SFLOAT, usage_flags)
            };

            return metadata;
        };

        constexpr static auto ssao = "ssao-result";
        constexpr static auto blur = "ssao-blur";
    };

    template<>
    struct InputDescriptorSetTraits<SSAO> final
    { 
        constexpr static uint8_t gbuffer = 0;
    };
}

namespace pbrlib::backend
{
    class SSAO final :
        public RenderPass
    {
        struct alignas(16) Params final
        {
            float               radius          = 0.5f;
            uint32_t            sample_count    = 0;
            pbrlib::math::vec2  noise_scale;
        };

        bool init(const RenderContext& context, uint32_t width, uint32_t height)    override;
        bool rebuild(uint32_t width, uint32_t height)                               override;

        void render(vk::CommandBuffer& command_buffer) override;

        VkPipelineStageFlags2 srcStage() const noexcept override;
        VkPipelineStageFlags2 dstStage() const noexcept override;

        std::pair<VkDescriptorSet, VkDescriptorSetLayout> resultDescriptorSet() const noexcept override;

        void createSampler();
        void bindResultDescriptorSet();

        void createSSAODescriptorSet();

        void createParamsBuffer();
        void createSamplesBuffer();

        void update(const Config& config) override;

    public:
        explicit SSAO(vk::Device& device, BilateralBlur* ptr_blur);
        ~SSAO();

    private:
        VkPipeline          _pipeline_handle        = VK_NULL_HANDLE;
        VkPipelineLayout    _pipeline_layout_handle = VK_NULL_HANDLE;

        VkDescriptorSet         _result_image_desc_set          = VK_NULL_HANDLE;
        VkDescriptorSetLayout   _result_image_desc_set_layout   = VK_NULL_HANDLE;

        VkSampler _result_image_sampler = VK_NULL_HANDLE;

        VkDescriptorSet         _ssao_desc_set          = VK_NULL_HANDLE;
        VkDescriptorSetLayout   _ssao_desc_set_layout   = VK_NULL_HANDLE;

        Params                      _params;
        std::optional<vk::Buffer>   _params_buffer;
        
        std::optional<vk::Buffer> _samples_buffer;

        BilateralBlur* _ptr_blur = nullptr;

        static constexpr auto final_attachments_layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    };
}