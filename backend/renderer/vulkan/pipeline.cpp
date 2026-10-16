#include <backend/renderer/vulkan/device.hpp>
#include <backend/renderer/vulkan/pipeline.hpp>
#include <backend/renderer/vulkan/check.hpp>
#include <backend/utils/paths.hpp>
#include <backend/logger/logger.hpp>

#include <ranges>

namespace pbrlib::backend::vk::utils
{
    static const auto pipelines_caches_directory = PBRLIB_ABS_PATH("pipelines-caches");

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

    auto cast(bool v) noexcept
    {
        return v ? VK_TRUE : VK_FALSE;
    }

    void logPipelineCreationInfo(
        const VkPipelineCreationFeedback&                   total_creation_info,
        std::span<const VkPipelineCreationFeedback>         stages_creation_info,
        std::span<const VkPipelineShaderStageCreateInfo>    stages
    )
    {
        const auto print_creation_feedback = [] (std::string_view title, const VkPipelineCreationFeedback& feedback)
        {
            std::string str_flags;
            if (feedback.flags) [[likely]]
            {
                auto add_flag = [&](const char* flag_name)
                {
                    if (!str_flags.empty())
                        str_flags += " | ";

                    str_flags += flag_name;
                };

                if (feedback.flags & VK_PIPELINE_CREATION_FEEDBACK_VALID_BIT)
                    add_flag("VALID");

                if (feedback.flags & VK_PIPELINE_CREATION_FEEDBACK_APPLICATION_PIPELINE_CACHE_HIT_BIT)
                    add_flag("APP_CACHE_HIT");

                if (feedback.flags & VK_PIPELINE_CREATION_FEEDBACK_BASE_PIPELINE_ACCELERATION_BIT)
                    add_flag("BASE_PIPELINE_ACCEL");
            }

            if (str_flags.empty()) [[unlikely]]
                str_flags = "NONE";

            log::info("[pipeline-creation-info] {}", title);
            log::info("[pipeline-creation-info] \t- compile time: {:.3f} ms", static_cast<double>(feedback.duration) / 1000000.0);
            log::info("[pipeline-creation-info] \t- flags: [{}]", str_flags);
        };

        const auto castToString = [] (VkShaderStageFlagBits current_stage)
        {
            constexpr std::array stages
            {
                std::make_pair(VK_SHADER_STAGE_VERTEX_BIT, "vertex"),
                std::make_pair(VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT, "tess-control"),
                std::make_pair(VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT, "tess-evaluation"),
                std::make_pair(VK_SHADER_STAGE_GEOMETRY_BIT, "geometry"),
                std::make_pair(VK_SHADER_STAGE_FRAGMENT_BIT, "fragment"),
                std::make_pair(VK_SHADER_STAGE_COMPUTE_BIT, "compute"),
                std::make_pair(VK_SHADER_STAGE_RAYGEN_BIT_KHR, "raygen"),
                std::make_pair(VK_SHADER_STAGE_ANY_HIT_BIT_KHR, "any-hit"),
                std::make_pair(VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR, "closest-hit"),
                std::make_pair(VK_SHADER_STAGE_MISS_BIT_KHR, "miss"),
                std::make_pair(VK_SHADER_STAGE_INTERSECTION_BIT_KHR, "intersection"),
                std::make_pair(VK_SHADER_STAGE_CALLABLE_BIT_KHR, "callable"),
                std::make_pair(VK_SHADER_STAGE_TASK_BIT_EXT, "task"),
                std::make_pair(VK_SHADER_STAGE_MESH_BIT_EXT, "mesh")
            };

            for (const auto [stage, name]: stages)
            {
                if (current_stage == stage)
                    return name;
            }

            throw exception::InvalidArgument(std::format("[pipeline-creation-info] undefined pipeline stage: {}", static_cast<uint32_t>(current_stage)));
        };

        print_creation_feedback("total info", total_creation_info);
        for (const auto& [stage_info, stage_creation_info]: std::views::zip(stages, stages_creation_info))
            print_creation_feedback(castToString(stage_info.stage), stage_creation_info);
    }
}

namespace pbrlib::backend::vk::builders
{
    GraphicsPipeline::GraphicsPipeline(Device& device) noexcept :
        _device (device)
    { }

    GraphicsPipeline& GraphicsPipeline::addStage(const std::filesystem::path& shader, VkShaderStageFlagBits stage, const shader::SpecializationInfoBase* ptr_spec_info)
    {
        const auto root_directory = PBRLIB_ABS_PATH("backend/shaders");

        _shaders.emplace_back(shader::compile(_device, shader, root_directory, _defines));

        VkPipelineShaderStageCreateInfo pipeline_stage =
        {
            .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .stage  = stage,
            .module = _shaders.back(),
            .pName  = "main"
        };

        if (ptr_spec_info)
        {
            const auto entries  = ptr_spec_info->entries();
            const auto data     = ptr_spec_info->data();

            _specialization_infos.emplace_back (
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

    GraphicsPipeline& GraphicsPipeline::addDefine(const vk::shader::Define& define)
    {
        _defines.push_back(define);
        return *this;
    }

    PipelineHandle GraphicsPipeline::build()
    {
        if (_pipeline_layout_handle == VK_NULL_HANDLE) [[unlikely]]
            throw exception::InvalidState("[vk-graphics-pipeline-builder] pipeline layout handle is null");

        if (_render_pass_handle == VK_NULL_HANDLE) [[unlikely]]
            throw exception::InvalidState("[vk-graphics-pipeline-builder] render pass handle is null");

        if (_subpass == std::numeric_limits<uint32_t>::max()) [[unlikely]]
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

#if defined(PBRLIB_ENABLE_DEVELOPER_MODE) || defined(PBRLIB_RUN_FROM_CI)
        VkPipelineCreationFeedback              pipeline_creation_feedback = {};
        std::vector<VkPipelineCreationFeedback> stages_creation_info (_stages.size(), VkPipelineCreationFeedback{});

        const VkPipelineCreationFeedbackCreateInfo pipeline_creation_feedback_create_info
        {
            .sType                              = VK_STRUCTURE_TYPE_PIPELINE_CREATION_FEEDBACK_CREATE_INFO,
            .pPipelineCreationFeedback          = &pipeline_creation_feedback,
            .pipelineStageCreationFeedbackCount = static_cast<uint32_t>(stages_creation_info.size()),
            .pPipelineStageCreationFeedbacks    = stages_creation_info.data()
        };
#endif

        VkGraphicsPipelineCreateInfo pipeline_create_info =
        {
            .sType                  = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
#if defined(PBRLIB_ENABLE_DEVELOPER_MODE) || defined(PBRLIB_RUN_FROM_CI)
            .pNext                  = &pipeline_creation_feedback_create_info,
#endif
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

        PipelineHandle pipeline_handle;

        VK_CHECK(vkCreateGraphicsPipelines(
            _device.device(),
            _device.globalPipelineCache(),
            1, &pipeline_create_info,
            nullptr,
            &pipeline_handle.handle()
        ));

#if defined(PBRLIB_ENABLE_DEVELOPER_MODE) || defined(PBRLIB_RUN_FROM_CI)
        utils::logPipelineCreationInfo(pipeline_creation_feedback, stages_creation_info, _stages);
#endif

        return pipeline_handle;
    }
}

namespace pbrlib::backend::vk::builders
{
    ComputePipeline::ComputePipeline(Device& device) noexcept :
        _device (device)
    { }

    ComputePipeline& ComputePipeline::shader(const std::filesystem::path& shader_name)
    {
        _shader_name = shader_name;
        return *this;
    }

    ComputePipeline& ComputePipeline::specializationInfo(const shader::SpecializationInfoBase& spec_info) noexcept
    {
        const auto entries  = spec_info.entries();
        const auto data     = spec_info.data();

        _specialization_info =
        {
            .mapEntryCount  = static_cast<uint32_t>(entries.size()),
            .pMapEntries    = entries.data(),
            .dataSize       = static_cast<uint32_t>(data.size()),
            .pData          = data.data()
        };

        return *this;
    }

    ComputePipeline& ComputePipeline::addDefine(const vk::shader::Define& define)
    {
        _defines.push_back(define);
        return *this;
    }

    ComputePipeline& ComputePipeline::pipelineLayoutHandle(VkPipelineLayout layout_handle) noexcept
    {
        _pipeline_layout_handle = layout_handle;
        return *this;
    }

    vk::PipelineHandle ComputePipeline::build()
    {
        if (_pipeline_layout_handle == VK_NULL_HANDLE) [[unlikely]]
            throw exception::InvalidState("[vk-compute-pipeline-builder] pipeline layout handle is null");

        const auto shader_module = shader::compile(_device, _shader_name, PBRLIB_ABS_PATH("backend/shaders"), _defines);

        const VkPipelineShaderStageCreateInfo stage
        {
            .sType                  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .stage                  = VK_SHADER_STAGE_COMPUTE_BIT,
            .module                 = shader_module,
            .pName                  = "main",
            .pSpecializationInfo    = &_specialization_info
        };

#if defined(PBRLIB_ENABLE_DEVELOPER_MODE) || defined(PBRLIB_RUN_FROM_CI)
        VkPipelineCreationFeedback pipeline_creation_feedback  = {};
        VkPipelineCreationFeedback stage_creation_feedback     = {};

        const VkPipelineCreationFeedbackCreateInfo pipeline_creation_feedback_create_info
        {
            .sType                              = VK_STRUCTURE_TYPE_PIPELINE_CREATION_FEEDBACK_CREATE_INFO,
            .pPipelineCreationFeedback          = &pipeline_creation_feedback,
            .pipelineStageCreationFeedbackCount = 1,
            .pPipelineStageCreationFeedbacks    = &stage_creation_feedback
        };
#endif

        const VkComputePipelineCreateInfo pipeline_info
        {
            .sType  = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,
#if defined(PBRLIB_ENABLE_DEVELOPER_MODE) || defined(PBRLIB_RUN_FROM_CI)
            .pNext  = &pipeline_creation_feedback_create_info,
#endif
            .stage  = stage,
            .layout = _pipeline_layout_handle
        };

        PipelineHandle pipeline_handle;

        VK_CHECK(vkCreateComputePipelines(
            _device.device(),
            _device.globalPipelineCache(),
            1, &pipeline_info,
            nullptr,
            &pipeline_handle.handle()
        ));

#if defined(PBRLIB_ENABLE_DEVELOPER_MODE) || defined(PBRLIB_RUN_FROM_CI)
        const std::array stage_feedbacks    = {stage_creation_feedback};
        const std::array stages             = {stage};
        utils::logPipelineCreationInfo(pipeline_creation_feedback, stage_feedbacks, stages);
#endif

        return pipeline_handle;
    }
}
