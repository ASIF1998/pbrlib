#include <backend/renderer/vulkan/device.hpp>
#include <backend/renderer/vulkan/pipeline.hpp>
#include <backend/renderer/vulkan/check.hpp>
#include <backend/utils/paths.hpp>
#include <backend/logger/logger.hpp>

#include <fstream>

namespace pbrlib::backend::vk::utils
{
    struct PipelineCache final
    {
        vk::PipelineCacheHandle handle;
        bool                    has_on_disk = false;
        std::string             filename;
    };

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

    PipelineCache createPipelineCache(Device& device, std::span<const std::string> shaders)
    {
        PipelineCache pipeline_cache { };

        size_t final_hash = 0;

        std::hash<std::string_view> hasher;
        for (const std::string_view shader_name: shaders)
        {
            const auto hash = static_cast<uint32_t>(hasher(shader_name));
            final_hash ^= hash + 0x9e3779b9 + (final_hash  << 6) + (final_hash >> 2);
        }

        if (!final_hash) [[unlikely]]
            return pipeline_cache;

        const auto pipelines_caches_directory = PBRLIB_ABS_PATH("pipelines-caches");
        if (!std::filesystem::exists(pipelines_caches_directory)) [[unlikely]]
            std::filesystem::create_directory(pipelines_caches_directory);

        pipeline_cache.filename = pipelines_caches_directory / (std::to_string(final_hash) + ".pbrlib-pipe-cache");

        std::ifstream file (pipeline_cache.filename, std::ios::binary);
        if (!file) [[unlikely]]
            return pipeline_cache;

        file.seekg(0, file.end);
        const auto size = file.tellg();
        file.seekg(0, file.beg);

        std::vector<char> cache (size);
        file.read(cache.data(), size);

        const VkPipelineCacheCreateInfo pipeline_cache_create_info
        {
            .sType              = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO,
            .flags              = VK_PIPELINE_CACHE_CREATE_INTERNALLY_SYNCHRONIZED_MERGE_BIT_KHR,
            .initialDataSize    = cache.size(),
            .pInitialData       = cache.data()
        };

        VkPipelineCache vk_pipeline_cache = VK_NULL_HANDLE;

        VK_CHECK(vkCreatePipelineCache(device.device(), &pipeline_cache_create_info, nullptr, &vk_pipeline_cache));

        pipeline_cache.handle       = vk::PipelineCacheHandle(vk_pipeline_cache);
        pipeline_cache.has_on_disk  = true;

        return pipeline_cache;
    }

    void savePipelineCache(Device& device, const PipelineCache& pipeline_cache)
    {
        if (pipeline_cache.has_on_disk) [[likely]]
            return ;

        size_t size = 0;
        VK_CHECK(vkGetPipelineCacheData(device.device(), pipeline_cache.handle, &size, nullptr));

        std::vector<char> data (size);
        VK_CHECK(vkGetPipelineCacheData(device.device(), pipeline_cache.handle, &size, data.data()));

        std::ofstream file (pipeline_cache.filename, std::ios::binary);
        if (!file) [[unlikely]]
        {
            backend::log::error("[pipeline-cache] failed save cache: {}", pipeline_cache.filename);
            return;
        }

        file.write(data.data(), data.size());
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
        _shaders_names.emplace_back(shader.string());

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

        auto pipeline_cache = utils::createPipelineCache(_device, _shaders_names);

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
            pipeline_cache.handle,
            1, &pipeline_create_info,
            nullptr,
            &pipeline_handle
        ));

        utils::savePipelineCache(_device, pipeline_cache);

        return PipelineHandle(pipeline_handle);
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

    PipelineHandle ComputePipeline::build()
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

        auto pipeline_cache = utils::createPipelineCache(_device, std::span<const std::string>({_shader_name}));

        const VkComputePipelineCreateInfo pipeline_info
        {
            .sType  = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,
            .stage  = stage,
            .layout = _pipeline_layout_handle
        };

        VkPipeline pipeline_handle = VK_NULL_HANDLE;

        VK_CHECK(vkCreateComputePipelines(
            _device.device(),
            VK_NULL_HANDLE,
            1, &pipeline_info,
            nullptr,
            &pipeline_handle
        ));

        utils::savePipelineCache(_device, pipeline_cache);

        return PipelineHandle(pipeline_handle);
    }
}
