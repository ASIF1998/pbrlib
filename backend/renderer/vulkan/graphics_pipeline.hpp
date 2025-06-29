#pragma once

#include <vulkan/vulkan.h>

#include <vector>
#include <filesystem>
#include <limits>

namespace pbrlib::backend::vk
{
    class Device;
}

namespace pbrlib::backend::vk
{
    enum class PrimitiveType
    {
        eTriangle
    };

    enum class PolygonMode 
    {
        eFill,
        eLine,
        ePoint
    };

    enum class CullMode
    {
        eNone,
        eBack,
        eFront,
        eFrontAndBack
    };

    enum class FrontFace
    {
        eClockwise,
        eCounterClockwise
    };

    enum class SampleCount
    {
        e1,
        e2,
        e4, 
        e8, 
        e16,
        e32, 
        e64
    };

    class GraphicsPipelineBuilder final
    {
    public:
        explicit GraphicsPipelineBuilder(Device& device) noexcept;

        GraphicsPipelineBuilder(GraphicsPipelineBuilder&& builder)      = delete;
        GraphicsPipelineBuilder(const GraphicsPipelineBuilder& builder) = delete;

        ~GraphicsPipelineBuilder();

        GraphicsPipelineBuilder& operator = (GraphicsPipelineBuilder&& builder)         = delete;
        GraphicsPipelineBuilder& operator = (const GraphicsPipelineBuilder& builder)    = delete;

        GraphicsPipelineBuilder& addStage(const std::filesystem::path& shader, VkShaderStageFlagBits stage);
        GraphicsPipelineBuilder& addAttachmentsState(bool blendEnable);

        GraphicsPipelineBuilder& primitiveType(PrimitiveType primitive_type)    noexcept;
        GraphicsPipelineBuilder& polygonMode(PolygonMode polygon_mode)          noexcept;
        GraphicsPipelineBuilder& cullMode(CullMode cull_mode)                   noexcept;
        GraphicsPipelineBuilder& frontFace(FrontFace front_face)                noexcept;
        GraphicsPipelineBuilder& sampleCount(SampleCount count)                 noexcept;

        GraphicsPipelineBuilder& depthStencilTest(bool is_enable) noexcept;

        GraphicsPipelineBuilder& pipelineLayoutHandle(VkPipelineLayout layout_handle)   noexcept;
        GraphicsPipelineBuilder& renderPassHandle(VkRenderPass render_pass_handle)      noexcept;

        GraphicsPipelineBuilder& subpass(uint32_t subpass_index) noexcept;

        [[nodiscard]] VkPipeline build();

    private:
        Device& _device;

        PrimitiveType   _primitive_type = PrimitiveType::eTriangle;
        PolygonMode     _polygon_mode   = PolygonMode::eFill;
        CullMode        _cull_mode      = CullMode::eBack;
        FrontFace       _front_face     = FrontFace::eClockwise;
        
        SampleCount _sample_count = SampleCount::e1;

        bool _enable_depth_stencil_test = false;

        VkPipelineLayout    _pipeline_layout_handle = VK_NULL_HANDLE;
        VkRenderPass        _render_pass_handle     = VK_NULL_HANDLE;

        uint32_t _subpass = std::numeric_limits<uint32_t>::max();

        std::vector<VkPipelineShaderStageCreateInfo>        _stages;
        std::vector<VkPipelineColorBlendAttachmentState>    _attachments_state;
    };
}