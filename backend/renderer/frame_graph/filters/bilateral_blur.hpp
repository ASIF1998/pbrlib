#pragma once

#include <backend/renderer/frame_graph/filters/filter.hpp>

namespace pbrlib::backend
{
    struct BilateralBlurSetsId final
    {
        enum
        {
            eBlurImage
        };
    };

    class BilateralBlur final :
        public Filter
    {
        bool init(const RenderContext& context, uint32_t width, uint32_t height)    override;
        bool rebuild(uint32_t width, uint32_t height)                               override;

        void render(vk::CommandBuffer& command_buffer) override;

        VkPipelineStageFlags2 srcStage() const noexcept override;
        VkPipelineStageFlags2 dstStage() const noexcept override;

        [[nodiscard]] 
        std::pair<VkDescriptorSet, VkDescriptorSetLayout> resultDescriptorSet() const noexcept override;
        
    public:
        struct alignas(16) Settings
        {   
            uint32_t    sample_count    = 8;
            float       sigma_s         = static_cast<float>(sample_count) * 0.25f;
            float       sigma_l         = 0.1f;
        };

        explicit BilateralBlur(vk::Device& device, std::string_view output_image_name, const Settings& settings);

        ~BilateralBlur();

        [[nodiscard]] Settings&         settings() noexcept;
        [[nodiscard]] const Settings&   settings() const noexcept;

    private:
        VkPipeline          _pipeline_handle        = VK_NULL_HANDLE;
        VkPipelineLayout    _pipeline_layout_handle = VK_NULL_HANDLE;

        VkDescriptorSet         _descriptor_set_handle          = VK_NULL_HANDLE;
        VkDescriptorSetLayout   _descriptor_set_layout_handle   = VK_NULL_HANDLE;

        VkSampler _sampler_handle = VK_NULL_HANDLE;

        std::string _output_image_name;

        Settings _settings;
    };
}