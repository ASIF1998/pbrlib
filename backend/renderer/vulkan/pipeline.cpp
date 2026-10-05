#include <backend/renderer/vulkan/device.hpp>
#include <backend/renderer/vulkan/pipeline.hpp>
#include <backend/renderer/vulkan/check.hpp>
#include <backend/utils/paths.hpp>
#include <backend/logger/logger.hpp>

namespace pbrlib::backend::vk::utils
{
    /// @link https://zeux.io/2019/07/17/serializing-pipeline-cache/
    struct PipelineCachePrefixHeader final
    {
        uint32_t    magic               = 0x5042524c;
        uint32_t    vendor_id           = 0;
        uint32_t    device_id           = 0;
        uint32_t    driver_version      = 0;
        uint64_t    size                = 0;
        uint8_t     uuid [VK_UUID_SIZE] = { };
    };

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

    bool checkPipelineCacheHeader(const Device& device, const utils::PipelineCachePrefixHeader& header) noexcept
    {
        if (header.magic != utils::PipelineCachePrefixHeader().magic) [[unlikely]]
            return false;

        const auto& gpu_properties = device.gpuProperties().properties;

        if (header.vendor_id != gpu_properties.vendorID) [[unlikely]]
            return false;

        if (header.device_id != gpu_properties.deviceID) [[unlikely]]
            return false;

        if (header.driver_version != gpu_properties.driverVersion) [[unlikely]]
            return false;

        if (memcmp(header.uuid, gpu_properties.pipelineCacheUUID, VK_UUID_SIZE)) [[unlikely]]
            return false;

        return true;
    }

    std::optional<PipelineCache> createPipelineCache(Device& device, std::span<const std::string> shaders)
    {
        size_t final_hash = 0;

        std::hash<std::string_view> hasher;
        for (const std::string_view shader_name: shaders)
        {
            const auto hash = static_cast<uint32_t>(hasher(shader_name));
            final_hash ^= hash + 0x9e3779b9 + (final_hash  << 6) + (final_hash >> 2);
        }

        if (!final_hash) [[unlikely]]
            return std::nullopt;

        PipelineCache pipeline_cache
        {
            .filename = utils::pipelines_caches_directory / (std::to_string(final_hash) + ".pbrlib-cache")
        };

        VkPipelineCacheCreateInfo pipeline_cache_create_info
        {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO,
            .flags = VK_PIPELINE_CACHE_CREATE_INTERNALLY_SYNCHRONIZED_MERGE_BIT_KHR
        };

#   ifndef PBRLIB_ENABLE_DEVELOPER_MODE
        std::ifstream       file (pipeline_cache.filename, std::ios::binary);
        std::vector<char>   cache;
        if (file) [[likely]]
        {
            utils::PipelineCachePrefixHeader header;
            file.read(reinterpret_cast<char*>(&header), static_cast<std::streamsize>(sizeof(utils::PipelineCachePrefixHeader)));

            if (checkPipelineCacheHeader(device, header)) [[likely]]
            {
                cache.resize(header.size);
                if (file.read(cache.data(), header.size)) [[likely]]
                {
                    /// @note some vulkan drivers have a bug where passing a non-null pInitialData
                    /// with initialDataSize == 0 causes pipeline cache creation to fail
                    if (cache.size() > 0) [[likely]]
                    {
                        pipeline_cache.has_on_disk = true;

                        pipeline_cache_create_info.pInitialData     = cache.data();
                        pipeline_cache_create_info.initialDataSize  = cache.size();
                    }
                    else
                        backend::log::warning("[pipeline-cache] failed to read cache file: {}", pipeline_cache.filename);
                }
                else
                    backend::log::warning("[pipeline-cache] failed to read cache file: {}", pipeline_cache.filename);
            }
            else
                backend::log::warning("[pipeline-cache] invalid pipeline cache header, file: {}", pipeline_cache.filename);
        }
#   endif

        VK_CHECK(vkCreatePipelineCache(device.device(), &pipeline_cache_create_info, nullptr, &pipeline_cache.handle.handle()));

        return pipeline_cache;
    }
}

namespace pbrlib::backend::vk
{
    Pipeline::Pipeline(Device& device, PipelineHandle&& pipeline_handle) noexcept :
        _device             (device),
        _pipeline_handle    (std::move(pipeline_handle))
    { }

    Pipeline::Pipeline(Device& device, PipelineHandle&& pipeline_handle, PipelineCache&& pipeline_cache) noexcept :
        _device             (device),
        _pipeline_handle    (std::move(pipeline_handle)),
        _pipeline_cache     (std::move(pipeline_cache))
    { }

    Pipeline::Pipeline(Pipeline&& pipeline) noexcept :
        _device(pipeline._device)
    {
        std::swap(_pipeline_cache, pipeline._pipeline_cache);
        std::swap(_pipeline_handle, pipeline._pipeline_handle);
    }

    Pipeline& Pipeline::operator = (Pipeline&& pipeline) noexcept
    {
        std::swap(_pipeline_cache, pipeline._pipeline_cache);
        std::swap(_pipeline_handle, pipeline._pipeline_handle);

        return *this;
    }

    Pipeline::~Pipeline()
    {
#   ifndef PBRLIB_ENABLE_DEVELOPER_MODE
        if (!_pipeline_cache || _pipeline_cache->has_on_disk || _pipeline_cache->handle == VK_NULL_HANDLE) [[likely]]
            return ;

        std::error_code create_directory_error_code;
        if (!std::filesystem::exists(utils::pipelines_caches_directory, create_directory_error_code)) [[unlikely]]
        {
            std::filesystem::create_directory(utils::pipelines_caches_directory, create_directory_error_code);

            if (create_directory_error_code) [[unlikely]]
            {
                backend::log::warning("[pipeline] failed to create pipeline cache directory: {}", create_directory_error_code.message());
                return ;
            }
        }

        size_t size = 0;
        if (vkGetPipelineCacheData(_device.device(), _pipeline_cache->handle, &size, nullptr) != VK_SUCCESS) [[unlikely]]
        {
            backend::log::warning("[pipeline] failed save pipeline cache: {}", _pipeline_cache->filename);
            return;
        }

        std::vector<char> data (size);
        if (vkGetPipelineCacheData(_device.device(), _pipeline_cache->handle, &size, data.data()) != VK_SUCCESS) [[unlikely]]
        {
            backend::log::warning("[pipeline] failed save pipeline cache: {}", _pipeline_cache->filename);
            return;
        }

        const auto& gpu_properties = _device.gpuProperties().properties;

        utils::PipelineCachePrefixHeader header
        {
            .size           = size,
            .vendor_id      = gpu_properties.vendorID,
            .device_id      = gpu_properties.deviceID,
            .driver_version = gpu_properties.driverVersion,
        };
        memcpy(header.uuid, gpu_properties.pipelineCacheUUID, VK_UUID_SIZE);

        std::ofstream file (_pipeline_cache->filename, std::ios::binary);
        if (file) [[likely]]
        {
            const auto ptr_header   = reinterpret_cast<const char*>(&header);
            const auto header_size  = static_cast<std::streamsize>(sizeof(utils::PipelineCachePrefixHeader));
            file.write(ptr_header, header_size);
            file.write(data.data(), static_cast<std::streamsize>(data.size()));
        }
        else
            backend::log::warning("[pipeline] failed save pipeline cache: {}", _pipeline_cache->filename);
#   endif
    }

    VkPipeline Pipeline::handle() const noexcept
    {
        return _pipeline_handle.handle();
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

    Pipeline GraphicsPipeline::build()
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

        VkPipelineCache pipeline_cache_handle = VK_NULL_HANDLE;
        if (pipeline_cache && pipeline_cache->handle != VK_NULL_HANDLE) [[likely]]
            pipeline_cache_handle = pipeline_cache->handle;

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
            pipeline_cache_handle,
            1, &pipeline_create_info,
            nullptr,
            &pipeline_handle
        ));

        if (pipeline_cache) [[likely]]
            return Pipeline(_device, PipelineHandle(pipeline_handle), std::move(pipeline_cache.value()));

        return Pipeline(_device, PipelineHandle(pipeline_handle));
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

    Pipeline ComputePipeline::build()
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

        VkPipelineCache pipeline_cache_handle = VK_NULL_HANDLE;
        if (pipeline_cache && pipeline_cache->handle) [[likely]]
            pipeline_cache_handle = pipeline_cache->handle;

        const VkComputePipelineCreateInfo pipeline_info
        {
            .sType  = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,
            .stage  = stage,
            .layout = _pipeline_layout_handle
        };

        VkPipeline pipeline_handle = VK_NULL_HANDLE;

        VK_CHECK(vkCreateComputePipelines(
            _device.device(),
            pipeline_cache_handle,
            1, &pipeline_info,
            nullptr,
            &pipeline_handle
        ));

        if (pipeline_cache) [[likely]]
            return Pipeline(_device, PipelineHandle(pipeline_handle), std::move(pipeline_cache.value()));

        return Pipeline(_device, PipelineHandle(pipeline_handle));
    }
}
