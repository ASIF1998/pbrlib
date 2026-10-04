#pragma once

#include <backend/renderer/vulkan/shader_compilers/shader_compiler.hpp>

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
    enum class PrimitiveType :
        uint8_t
    {
        eTriangle
    };

    enum class PolygonMode :
        uint8_t
    {
        eFill,
        eLine,
        ePoint
    };

    enum class CullMode :
        uint8_t
    {
        eNone,
        eBack,
        eFront,
        eFrontAndBack
    };

    enum class FrontFace :
        uint8_t
    {
        eClockwise,
        eCounterClockwise
    };

    enum class SampleCount :
        uint8_t
    {
        e1,
        e2,
        e4,
        e8,
        e16,
        e32,
        e64
    };

    struct PipelineCache final
    {
        vk::PipelineCacheHandle handle;
        bool                    has_on_disk = false;
        std::string             filename;
    };

    class Pipeline final
    {
    public:
        explicit Pipeline(Device& device, PipelineHandle&& pipeline_handle)                                 noexcept;
        explicit Pipeline(Device& device, PipelineHandle&& pipeline_handle, PipelineCache&& pipeline_cache) noexcept;

        Pipeline(Pipeline&& pipeline) noexcept;

        ~Pipeline();

        Pipeline& operator = (Pipeline&& pipeline) noexcept;

        [[nodiscard]] VkPipeline handle() const noexcept;

    private:
        Device& _device;

        PipelineHandle                  _pipeline_handle;
        std::optional<PipelineCache>    _pipeline_cache;
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

        GraphicsPipeline& operator = (GraphicsPipeline&& builder)       = delete;
        GraphicsPipeline& operator = (const GraphicsPipeline& builder)  = delete;

        GraphicsPipeline& addStage(const std::filesystem::path& shader, VkShaderStageFlagBits stage, const shader::SpecializationInfoBase* ptr_spec_info = nullptr);
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

        GraphicsPipeline& addDefine(const vk::shader::Define& define);

        [[nodiscard]] Pipeline build();

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
        std::vector<VkSpecializationInfo>                   _specialization_infos;

        std::vector<vk::ShaderModuleHandle> _shaders;
        std::vector<std::string>            _shaders_names;

        std::vector<backend::vk::shader::Define> _defines;
    };

    class ComputePipeline final
    {
    public:
        explicit ComputePipeline(Device& device) noexcept;

        ComputePipeline(ComputePipeline&& builder)      = delete;
        ComputePipeline(const ComputePipeline& builder) = delete;

        ComputePipeline& operator = (ComputePipeline&& builder)         = delete;
        ComputePipeline& operator = (const ComputePipeline& builder)    = delete;

        ComputePipeline& shader                 (const std::filesystem::path& shader_name);
        ComputePipeline& addDefine              (const vk::shader::Define& define);
        ComputePipeline& specializationInfo     (const shader::SpecializationInfoBase& spec_info)   noexcept;
        ComputePipeline& pipelineLayoutHandle   (VkPipelineLayout layout_handle)                    noexcept;

        [[nodiscard]] Pipeline build();

    private:
        Device& _device;

        std::filesystem::path _shader_name;

        VkPipelineLayout _pipeline_layout_handle = VK_NULL_HANDLE;

        VkSpecializationInfo _specialization_info = { };

        std::vector<backend::vk::shader::Define> _defines;
    };
}
