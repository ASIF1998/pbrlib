#include <backend/renderer/vulkan/device.hpp>

#include <backend/renderer/vulkan/graphics_pipeline.hpp>
#include <backend/renderer/vulkan/shader_compiler.hpp>

#include <backend/utils/vulkan.hpp>

#include <pbrlib/exceptions.hpp>

#include <array>

namespace pbrlib::backend::vk::utils
{
    VkPrimitiveTopology cast(PrimitiveType type) noexcept
    {
        switch (type)
        {
            case PrimitiveType::eTriangle:
                return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        }

        return VK_PRIMITIVE_TOPOLOGY_MAX_ENUM;
    }

    VkPolygonMode cast(PolygonMode mode) noexcept
    {
        switch (mode)
        {
            case PolygonMode::eFill:
                return VK_POLYGON_MODE_FILL;
            case PolygonMode::eLine:
                return VK_POLYGON_MODE_LINE;
            case PolygonMode::ePoint:
                return VK_POLYGON_MODE_POINT;
        }

        return VK_POLYGON_MODE_MAX_ENUM;
    }

    VkCullModeFlags cast(CullMode mode) noexcept
    {
        switch (mode)
        {
            case CullMode::eNone:
                return VK_CULL_MODE_NONE;
            case CullMode::eBack:
                return VK_CULL_MODE_BACK_BIT;
            case CullMode::eFront:
                return VK_CULL_MODE_FRONT_BIT;
            case CullMode::eFrontAndBack:
                return VK_CULL_MODE_FRONT_AND_BACK;
        }

        return VK_CULL_MODE_FLAG_BITS_MAX_ENUM;
    }

    VkFrontFace cast(FrontFace front_face) noexcept
    {
        switch (front_face)
        {
            case FrontFace::eClockwise:
                return VK_FRONT_FACE_CLOCKWISE;
            case FrontFace::eCounterClockwise:
                return VK_FRONT_FACE_COUNTER_CLOCKWISE;
        };

        return VK_FRONT_FACE_MAX_ENUM;
    }

    VkSampleCountFlagBits cast(SampleCount count) noexcept
    {
        switch (count)
        {
            case SampleCount::e1:
                return VK_SAMPLE_COUNT_1_BIT;
            case SampleCount::e2:
                return VK_SAMPLE_COUNT_2_BIT;
            case SampleCount::e4:
                return VK_SAMPLE_COUNT_4_BIT;
            case SampleCount::e8:
                return VK_SAMPLE_COUNT_8_BIT;
            case SampleCount::e16:
                return VK_SAMPLE_COUNT_16_BIT;
            case SampleCount::e32:
                return VK_SAMPLE_COUNT_32_BIT;
            case SampleCount::e64:
                return VK_SAMPLE_COUNT_64_BIT;
        };

        return VK_SAMPLE_COUNT_FLAG_BITS_MAX_ENUM;
    }

    auto cast(bool v)
    {
        return v ? VK_TRUE : VK_FALSE;
    }
}

namespace pbrlib::backend::vk::builders
{
    GraphicsPipeline::GraphicsPipeline(Device& device) noexcept :
        _device (device)
    { }

    GraphicsPipeline::~GraphicsPipeline()
    {
        for (const auto& stage: _stages)
            vkDestroyShaderModule(_device.device(), stage.module, nullptr);
    }

    GraphicsPipeline& GraphicsPipeline::addStage(const std::filesystem::path& shader, VkShaderStageFlagBits stage, const shader::SpecializationInfoBase* ptr_spec_info)
    {
        VkPipelineShaderStageCreateInfo pipeline_stage =
        {
            .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .stage  = stage,
            .module = vk::shader::compile(_device, shader),
            .pName  = "main"
        };

        if (ptr_spec_info)
        {
            const auto entries  = ptr_spec_info->entries();
            const auto data     = ptr_spec_info->data();

            _specialization_infos.emplace_back(
                static_cast<uint32_t>(entries.size()), entries.data(), 
                static_cast<uint32_t>(data.size()), data.data()
            );

            pipeline_stage.pSpecializationInfo = &_specialization_infos.back();
        }

        _stages.push_back(pipeline_stage);

        return *this;
    }

    GraphicsPipeline& GraphicsPipeline::addAttachmentsState(bool blendEnable)
    {
        const VkPipelineColorBlendAttachmentState state =
        {
            .blendEnable    = utils::cast(blendEnable),
            .colorWriteMask =
                    VK_COLOR_COMPONENT_R_BIT
                |   VK_COLOR_COMPONENT_G_BIT
                |   VK_COLOR_COMPONENT_B_BIT
                |   VK_COLOR_COMPONENT_A_BIT
        };

        _attachments_state.push_back(state);

        return *this;
    }

    GraphicsPipeline& GraphicsPipeline::primitiveType(PrimitiveType primitive_type) noexcept
    {
        _primitive_type = primitive_type;
        return *this;
    }

    GraphicsPipeline& GraphicsPipeline::polygonMode(PolygonMode polygon_mode) noexcept
    {
        _polygon_mode = polygon_mode;
        return *this;
    }

    GraphicsPipeline& GraphicsPipeline::cullMode(CullMode cull_mode) noexcept
    {
        _cull_mode = cull_mode;
        return *this;
    }

    GraphicsPipeline& GraphicsPipeline::frontFace(FrontFace front_face) noexcept
    {
        _front_face = front_face;
        return *this;
    }

    GraphicsPipeline& GraphicsPipeline::sampleCount(SampleCount count) noexcept
    {
        _sample_count = count;
        return *this;   
    }

    GraphicsPipeline& GraphicsPipeline::depthStencilTest(bool is_enable) noexcept
    {
        _enable_depth_stencil_test = is_enable;
        return *this;
    }

    GraphicsPipeline& GraphicsPipeline::pipelineLayoutHandle(VkPipelineLayout layout_handle) noexcept
    {
        _pipeline_layout_handle = layout_handle;
        return *this;
    }

    GraphicsPipeline& GraphicsPipeline::renderPassHandle(VkRenderPass render_pass_handle) noexcept 
    {
        _render_pass_handle = render_pass_handle;
        return *this;
    }

    GraphicsPipeline& GraphicsPipeline::subpass(uint32_t subpass_index) noexcept
    {
        _subpass = subpass_index;
        return *this;
    }

    VkPipeline GraphicsPipeline::build()
    {
        if (_pipeline_layout_handle == VK_NULL_HANDLE)
            throw exception::InvalidState("[vk-graphics-pipeline-builder] pipeline layout handle is null");
        
        if (_render_pass_handle == VK_NULL_HANDLE)
            throw exception::InvalidState("[vk-graphics-pipeline-builder] render pass handle is null");

        if (_subpass == std::numeric_limits<uint32_t>::max())
            throw exception::InvalidState("[vk-graphics-pipeline-builder] subpass index didn't set");

        constexpr VkPipelineVertexInputStateCreateInfo vertex_input_state =
        {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO
        };

        const VkPipelineInputAssemblyStateCreateInfo input_assembly_state =
        {
            .sType      = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
            .topology   = utils::cast(_primitive_type)
        };

        constexpr VkPipelineViewportStateCreateInfo viewport_state =
        {
            .sType          = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
            .viewportCount  = 1,
            .scissorCount   = 1
        };

        const VkPipelineRasterizationStateCreateInfo rasterization_state =
        {
            .sType                      = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
            .depthClampEnable           = VK_FALSE,
            .rasterizerDiscardEnable    = VK_FALSE,
            .polygonMode                = utils::cast(_polygon_mode),
            .cullMode                   = utils::cast(_cull_mode),
            .frontFace                  = utils::cast(_front_face),
            .depthBiasEnable            = VK_FALSE,
            .lineWidth                  = 1.0f
        };

        const VkPipelineMultisampleStateCreateInfo multisample_state =
        {
            .sType                  = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
            .rasterizationSamples   = utils::cast(_sample_count)
        };

        constexpr VkPipelineDepthStencilStateCreateInfo depth_stencil_state =
        {
            .sType              = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
            .depthTestEnable    = VK_TRUE,
            .depthWriteEnable   = VK_TRUE,
            .depthCompareOp     = VK_COMPARE_OP_LESS,
            .stencilTestEnable  = VK_FALSE,
            .minDepthBounds     = 0.0f,
            .maxDepthBounds     = 1.0f
        };

        const VkPipelineColorBlendStateCreateInfo color_blend_state =
        {
            .sType              = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
            .logicOpEnable      = VK_FALSE,
            .logicOp            = VK_LOGIC_OP_COPY,
            .attachmentCount    = static_cast<uint32_t>(_attachments_state.size()),
            .pAttachments       = _attachments_state.data(),
            .blendConstants     = { }
        };

        constexpr std::array dynamic_states =
        {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR
        };

        const VkPipelineDynamicStateCreateInfo dynamic_state =
        {
            .sType              = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
            .dynamicStateCount  = static_cast<uint32_t>(dynamic_states.size()),
            .pDynamicStates     = dynamic_states.data()
        };

        VkGraphicsPipelineCreateInfo pipeline_create_info =
        {
            .sType                  = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
            .stageCount             = static_cast<uint32_t>(_stages.size()),
            .pStages                = _stages.data(),
            .pVertexInputState      = &vertex_input_state,
            .pInputAssemblyState    = &input_assembly_state,
            .pViewportState         = &viewport_state,
            .pRasterizationState    = &rasterization_state,
            .pMultisampleState      = &multisample_state,
            .pColorBlendState       = &color_blend_state,
            .pDynamicState          = &dynamic_state,
            .layout                 = _pipeline_layout_handle,
            .renderPass             = _render_pass_handle,
            .subpass                = _subpass
        };

        if (_enable_depth_stencil_test)
            pipeline_create_info.pDepthStencilState = &depth_stencil_state;

        VkPipeline pipeline_handle = VK_NULL_HANDLE;

        VK_CHECK(vkCreateGraphicsPipelines(
            _device.device(),
            VK_NULL_HANDLE,
            1, &pipeline_create_info,
            nullptr,
            &pipeline_handle
        ));

        return pipeline_handle;
    }
}