#pragma once

#include <backend/renderer/frame_graph/filters/filter.hpp>
#include <backend/renderer/vulkan/unique_handler.hpp>

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

        [[nodiscard]] Settings&         settings() noexcept;
        [[nodiscard]] const Settings&   settings() const noexcept;

    private:
        vk::PipelineLayoutHandle    _pipeline_layout_handle;
        vk::PipelineHandle          _pipeline_handle;

        vk::DescriptorSetLayoutHandle   _descriptor_set_layout_handle;
        vk::DescriptorSetHandle         _descriptor_set_handle;

        vk::SamplerHandle _sampler_handle;

        std::string _output_image_name;

        Settings _settings;
    };
}