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
}

namespace pbrlib::backend::vk::builders
{
    class GraphicsPipeline final
    {
    public:
        explicit GraphicsPipeline(Device& device) noexcept;

        GraphicsPipeline(GraphicsPipeline&& builder)        = delete;
        GraphicsPipeline(const GraphicsPipeline& builder)   = delete;

        ~GraphicsPipeline();

        GraphicsPipeline& operator = (GraphicsPipeline&& builder)       = delete;
        GraphicsPipeline& operator = (const GraphicsPipeline& builder)  = delete;

        GraphicsPipeline& addStage(const std::filesystem::path& shader, VkShaderStageFlagBits stage);
        GraphicsPipeline& addAttachmentsState(bool blendEnable);

        GraphicsPipeline& primitiveType(PrimitiveType primitive_type)   noexcept;
        GraphicsPipeline& polygonMode(PolygonMode polygon_mode)         noexcept;
        GraphicsPipeline& cullMode(CullMode cull_mode)                  noexcept;
        GraphicsPipeline& frontFace(FrontFace front_face)               noexcept;
        GraphicsPipeline& sampleCount(SampleCount count)                noexcept;

        GraphicsPipeline& depthStencilTest(bool is_enable) noexcept;

        GraphicsPipeline& pipelineLayoutHandle(VkPipelineLayout layout_handle)  noexcept;
        GraphicsPipeline& renderPassHandle(VkRenderPass render_pass_handle)     noexcept;

        GraphicsPipeline& subpass(uint32_t subpass_index) noexcept;

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