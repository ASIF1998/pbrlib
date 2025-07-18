#pragma once

#include <backend/renderer/vulkan/pipeline_layout.hpp>
#include <backend/renderer/vulkan/buffer.hpp>

#include <backend/renderer/frame_graph/render_pass.hpp> 

#include <pbrlib/math/vec2.hpp>
#include <pbrlib/math/matrix4x4.hpp>

#include <optional>

namespace pbrlib::backend
{
    struct SSAOOutputAttachmentsNames final
    {
        constexpr static auto result = "ssao-result";
    };

    struct SSAOInputAttachmentNames final
    {
        constexpr static auto pos_uv            = "ssao-gbuffer-pos-uv";
        constexpr static auto normal_tangent    = "ssao-gbuffer-normal-tangent";
        constexpr static auto depth_buffer      = "ssao-gbuffer-depth-buffer";
    };

    struct SSAOInputSetsId
    {
        enum
        {
            eGBuffer
        };
    };
    
    class SSAO final :
        public RenderPass
    {
        struct alignas(16) Params
        {
            float               radius          = 0.5;
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
        void createResultDescriptorSet();

        void createSSAODescriptorSet();

        void createParamsBuffer();
        void createSamplesBuffer();

    public:
        explicit SSAO(vk::Device& device);
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

        static constexpr auto final_attachments_layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    };
}