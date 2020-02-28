//
//  GraphicsPipeline.inl
//  PBRLib
//
//  Created by Асиф Мамедов on 24/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <numeric>

namespace pbrlib
{
    inline VertexInputState::VertexInputState(
        size_t num_vertex_biding_descriptions,
        size_t num_vertex_attribute_descriptions
    ) :
        VkPipelineVertexInputStateCreateInfo {
            .sType                              = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
            .pNext                              = nullptr,
            .flags                              = 0,
            .vertexBindingDescriptionCount      = static_cast<uint32_t>(num_vertex_biding_descriptions),
            .vertexAttributeDescriptionCount    = static_cast<uint32_t>(num_vertex_attribute_descriptions),
        },
        _ptr_vertex_biding_descriptions     (new VkVertexInputBindingDescription[num_vertex_biding_descriptions]),
        _ptr_vertex_attribute_descriptions  (new VkVertexInputAttributeDescription [num_vertex_attribute_descriptions]),
        _curent_vertex_biding_description   (0),
        _curent_vertex_attribute_description(0)
    {
        VkPipelineVertexInputStateCreateInfo::pVertexBindingDescriptions    = _ptr_vertex_biding_descriptions;
        VkPipelineVertexInputStateCreateInfo::pVertexAttributeDescriptions  = _ptr_vertex_attribute_descriptions;
    }

    inline VertexInputState::VertexInputState(VertexInputState&& vertex_input_state) :
        VkPipelineVertexInputStateCreateInfo {
            .sType                              = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
            .pNext                              = nullptr,
            .flags                              = 0,
            .vertexBindingDescriptionCount      = vertex_input_state.vertexBindingDescriptionCount,
            .pVertexBindingDescriptions         = nullptr,
            .vertexAttributeDescriptionCount    = vertex_input_state.vertexAttributeDescriptionCount,
            .pVertexAttributeDescriptions       = nullptr
        },
        _ptr_vertex_biding_descriptions     (nullptr),
        _ptr_vertex_attribute_descriptions  (nullptr),
        _curent_vertex_biding_description   (vertex_input_state._curent_vertex_biding_description),
        _curent_vertex_attribute_description(vertex_input_state._curent_vertex_attribute_description)
    {
        swap(VkPipelineVertexInputStateCreateInfo::pVertexBindingDescriptions,      vertex_input_state.pVertexBindingDescriptions);
        swap(VkPipelineVertexInputStateCreateInfo::pVertexAttributeDescriptions,    vertex_input_state.pVertexAttributeDescriptions);
        swap(_ptr_vertex_biding_descriptions,                                       vertex_input_state._ptr_vertex_biding_descriptions);
        swap(_ptr_vertex_attribute_descriptions,                                    vertex_input_state._ptr_vertex_attribute_descriptions);
        
        vertex_input_state.vertexBindingDescriptionCount    = 0;
        vertex_input_state.vertexAttributeDescriptionCount  = 0;
    }

    inline VertexInputState::VertexInputState(const VertexInputState& vertex_input_state) :
        VkPipelineVertexInputStateCreateInfo {
            .sType                              = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
            .pNext                              = nullptr,
            .flags                              = 0,
            .vertexBindingDescriptionCount      = vertex_input_state.vertexBindingDescriptionCount,
            .pVertexBindingDescriptions         = nullptr,
            .vertexAttributeDescriptionCount    = vertex_input_state.vertexAttributeDescriptionCount,
            .pVertexAttributeDescriptions       = nullptr
        },
        _ptr_vertex_biding_descriptions     (new VkVertexInputBindingDescription[vertex_input_state.vertexBindingDescriptionCount]),
        _ptr_vertex_attribute_descriptions  (new VkVertexInputAttributeDescription [vertex_input_state.vertexAttributeDescriptionCount]),
        _curent_vertex_biding_description   (vertex_input_state._curent_vertex_biding_description),
        _curent_vertex_attribute_description(vertex_input_state._curent_vertex_attribute_description)
    {
        memcpy(_ptr_vertex_biding_descriptions,
               vertex_input_state._ptr_vertex_biding_descriptions,
               vertex_input_state.vertexBindingDescriptionCount * sizeof(VkVertexInputBindingDescription));
        
        memcpy(_ptr_vertex_attribute_descriptions,
               vertex_input_state._ptr_vertex_attribute_descriptions,
               vertex_input_state.vertexAttributeDescriptionCount * sizeof(VkVertexInputAttributeDescription));
        
        VkPipelineVertexInputStateCreateInfo::pVertexBindingDescriptions    = _ptr_vertex_biding_descriptions;
        VkPipelineVertexInputStateCreateInfo::pVertexAttributeDescriptions  = _ptr_vertex_attribute_descriptions;
    }

    inline void VertexInputState::addVertexInputBindingDescription(uint32_t binding, uint32_t stride, VkVertexInputRate input_rate)
    {
        assert(_curent_vertex_biding_description < VkPipelineVertexInputStateCreateInfo::vertexBindingDescriptionCount);

        _ptr_vertex_biding_descriptions[_curent_vertex_biding_description] = {
            .binding    = binding,
            .stride     = stride,
            .inputRate  = input_rate
        };

        _curent_vertex_biding_description++;
    }

    inline void VertexInputState::addVertexInputAttributeDescription(
        uint32_t location,
        uint32_t binding,
        VkFormat format,
        uint32_t offset
    )
    {
        assert(_curent_vertex_attribute_description < VkPipelineVertexInputStateCreateInfo::vertexAttributeDescriptionCount);

        _ptr_vertex_attribute_descriptions[_curent_vertex_attribute_description] = {
            .location   = location,
            .binding    = binding,
            .format     = format,
            .offset     = offset
        };

        _curent_vertex_attribute_description++;
    }

    inline const VkVertexInputBindingDescription* VertexInputState::getVertexInputBindingDescriptions() const noexcept
    {
        return _ptr_vertex_biding_descriptions;
    }


    inline const VkVertexInputAttributeDescription* VertexInputState::getVertexInputAttributeDescriptions() const noexcept
    {
        return _ptr_vertex_attribute_descriptions;
    }

    inline size_t VertexInputState::numVertexInputBindingDescription() const noexcept
    {
        return _curent_vertex_biding_description;
    }

    inline size_t VertexInputState::numVertexInputAttributeDescription() const noexcept
    {
        return _curent_vertex_attribute_description;
    }

    inline size_t VertexInputState::capacityVertexInputBindingDescription() const noexcept
    {
        return VkPipelineVertexInputStateCreateInfo::vertexBindingDescriptionCount;
    }

    inline size_t VertexInputState::capacityVertexInputAttributeDescription() const noexcept
    {
        return VkPipelineVertexInputStateCreateInfo::vertexAttributeDescriptionCount;
    }

    inline VertexInputState::~VertexInputState()
    {
        if (_ptr_vertex_biding_descriptions) {
            delete[] _ptr_vertex_biding_descriptions;
        }

        if (_ptr_vertex_attribute_descriptions) {
            delete[] _ptr_vertex_attribute_descriptions;
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    MultisampleState::MultisampleState(
        VkSampleCountFlagBits   num_samples,
        VkBool32                sample_shading_enable,
        float                   min_sample_shading,
        VkBool32                alpha_to_coverage_enable,
        VkBool32                alpha_to_one_enable
    ) :
        VkPipelineMultisampleStateCreateInfo {
            .sType                  = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
            .pNext                  = nullptr,
            .flags                  = 0,
            .rasterizationSamples   = num_samples,
            .sampleShadingEnable    = sample_shading_enable,
            .minSampleShading       = min_sample_shading,
            .pSampleMask            = &_sample_mask,
            .alphaToCoverageEnable  = alpha_to_coverage_enable,
            .alphaToOneEnable       = alpha_to_one_enable
        },
        _sample_mask(numeric_limits<decltype(_sample_mask)>::max())
    {}

    inline MultisampleState::MultisampleState(const MultisampleState& multisample_state) noexcept
    {
        memcpy(this, &multisample_state, sizeof(MultisampleState));
        VkPipelineMultisampleStateCreateInfo::pSampleMask = &_sample_mask;
    }

    inline void MultisampleState::setSampleMask(VkSampleMask sample_mask) noexcept
    {
        _sample_mask = sample_mask;
    }

    inline void MultisampleState::setRasterizationSamples(VkSampleCountFlagBits num_samples) noexcept
    {
        VkPipelineMultisampleStateCreateInfo::rasterizationSamples = num_samples;
    }

    inline void MultisampleState::setMinSampleShading(float min_sample_shading) noexcept
    {
        assert(min_sample_shading >= 0.0f && min_sample_shading <= 1.0f);
        VkPipelineMultisampleStateCreateInfo::minSampleShading = min_sample_shading;
    }
    
    inline void MultisampleState::sampleShadingEnable(VkBool32 is_enable) noexcept
    {
        VkPipelineMultisampleStateCreateInfo::sampleShadingEnable = is_enable;
    }

    inline void MultisampleState::alphaToCoverageEnable(VkBool32 is_enable) noexcept
    {
        VkPipelineMultisampleStateCreateInfo::alphaToCoverageEnable = is_enable;
    }

    inline void MultisampleState::alphaToOneEnable(VkBool32 is_enable) noexcept
    {
        VkPipelineMultisampleStateCreateInfo::alphaToOneEnable = is_enable;
    }

    inline VkSampleMask MultisampleState::getSampleMask() const noexcept
    {
        return _sample_mask;
    }

    inline VkSampleCountFlagBits MultisampleState::getRasterizationSamples() const noexcept
    {
        return VkPipelineMultisampleStateCreateInfo::rasterizationSamples;
    }

    inline float MultisampleState::getMinSampleShading() const noexcept
    {
        return VkPipelineMultisampleStateCreateInfo::minSampleShading;
    }

    inline VkBool32 MultisampleState::sampleShadingEnable() const noexcept
    {
        return VkPipelineMultisampleStateCreateInfo::sampleShadingEnable;
    }

    inline VkBool32 MultisampleState::alphaToCoverageEnable() const noexcept
    {
        return VkPipelineMultisampleStateCreateInfo::alphaToCoverageEnable;
    }

    inline VkBool32 MultisampleState::alphaToOneEnable() const noexcept
    {
        return VkPipelineMultisampleStateCreateInfo::alphaToOneEnable;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inline RasterizationState::RasterizationState() :
        VkPipelineRasterizationStateCreateInfo {
            .sType      = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
            .pNext      = nullptr,
            .flags      = 0,
            .lineWidth  = 1.0f
        }
    {}

    inline void RasterizationState::setPolygonMode(VkPolygonMode polygon_mode) noexcept
    {
        VkPipelineRasterizationStateCreateInfo::polygonMode = polygon_mode;
    }

    inline void RasterizationState::setCullMode(VkCullModeFlags cull_mode) noexcept
    {
        VkPipelineRasterizationStateCreateInfo::cullMode = cull_mode;
    }
    
    inline void RasterizationState::setFrontFace(VkFrontFace front_face) noexcept
    {
        VkPipelineRasterizationStateCreateInfo::frontFace = front_face;
    }

    inline void RasterizationState::setDepthBiasConstantFactor(float depth_bias_constant_factor) noexcept
    {
        VkPipelineRasterizationStateCreateInfo::depthBiasConstantFactor = depthBiasConstantFactor;
    }

    inline void RasterizationState::setDepthBiasClamp(float depth_bias_clamp) noexcept
    {
        VkPipelineRasterizationStateCreateInfo::depthBiasClamp = depth_bias_clamp;
    }

    inline void RasterizationState::setDepthBiasSlopeFactor(float depts_bias_slope_factor) noexcept
    {
        VkPipelineRasterizationStateCreateInfo::depthBiasSlopeFactor = depts_bias_slope_factor;
    }

    inline void RasterizationState::setLineWidth(float line_width) noexcept
    {
        VkPipelineRasterizationStateCreateInfo::lineWidth = line_width;
    }

    inline void RasterizationState::depthClampEnable(VkBool32 is_enable) noexcept
    {
        VkPipelineRasterizationStateCreateInfo::depthClampEnable = is_enable;
    }

    inline void RasterizationState::rasterizerDiscardEnable(VkBool32 is_enable) noexcept
    {
        VkPipelineRasterizationStateCreateInfo::rasterizerDiscardEnable = is_enable;
    }

    inline void RasterizationState::depthBiasEnable(VkBool32 is_enable) noexcept
    {
        VkPipelineRasterizationStateCreateInfo::depthBiasEnable = is_enable;
    }

    inline VkPolygonMode RasterizationState::getPolygonMode() const noexcept
    {
        return VkPipelineRasterizationStateCreateInfo::polygonMode;
    }

    inline VkCullModeFlags RasterizationState::getCullMode() const noexcept
    {
        return VkPipelineRasterizationStateCreateInfo::cullMode;
    }

    inline VkFrontFace RasterizationState::getFrontFace() const noexcept
    {
        return VkPipelineRasterizationStateCreateInfo::frontFace;
    }

    inline float RasterizationState::getDepthBiasConstantFactor() const noexcept
    {
        return VkPipelineRasterizationStateCreateInfo::depthBiasConstantFactor;
    }

    inline float RasterizationState::getDepthBiasClamp() const noexcept
    {
        return VkPipelineRasterizationStateCreateInfo::depthBiasClamp;
    }

    inline float RasterizationState::getDepthBiasSlopeFactor() const noexcept
    {
        return VkPipelineRasterizationStateCreateInfo::depthBiasSlopeFactor;
    }

    inline float RasterizationState::getLineWidth() const noexcept
    {
        return VkPipelineRasterizationStateCreateInfo::lineWidth;
    }

    inline VkBool32 RasterizationState::depthClampEnable() const noexcept
    {
        return VkPipelineRasterizationStateCreateInfo::depthClampEnable;
    }

    inline VkBool32 RasterizationState::rasterizerDiscardEnable() const noexcept
    {
        return VkPipelineRasterizationStateCreateInfo::rasterizerDiscardEnable;
    }

    inline VkBool32 RasterizationState::depthBiasEnable() const noexcept
    {
        return VkPipelineRasterizationStateCreateInfo::depthBiasEnable;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inline DepthStencilState::DepthStencilState(VkBool32 depth_test_enable, VkBool32 stencil_test_enable) noexcept :
        VkPipelineDepthStencilStateCreateInfo{
            .sType              = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
            .pNext              = nullptr,
            .flags              = 0,
            .depthTestEnable    = depth_test_enable,
            .stencilTestEnable  = stencil_test_enable
        }
    {}

    inline void DepthStencilState::setDepthCompareOp(VkCompareOp depth_compare_op) noexcept
    {
        VkPipelineDepthStencilStateCreateInfo::depthCompareOp = depth_compare_op;
    }

    inline void DepthStencilState::setFront(VkStencilOpState front) noexcept
    {
        VkPipelineDepthStencilStateCreateInfo::front = front;
    }

    inline void DepthStencilState::setBack(VkStencilOpState back) noexcept
    {
        VkPipelineDepthStencilStateCreateInfo::back = back;
    }

    inline void DepthStencilState::setMinDepthBounds(float min_depth_bounds) noexcept
    {
        VkPipelineDepthStencilStateCreateInfo::minDepthBounds = min_depth_bounds;
    }

    inline void DepthStencilState::setMaxDepthBounds(float max_depth_bounds) noexcept
    {
        VkPipelineDepthStencilStateCreateInfo::maxDepthBounds = max_depth_bounds;
    }

    inline void DepthStencilState::depthTestEnable(VkBool32 is_enable) noexcept
    {
        VkPipelineDepthStencilStateCreateInfo::depthTestEnable = is_enable;
    }

    inline void DepthStencilState::depthWriteEnable(VkBool32 is_enable) noexcept
    {
        VkPipelineDepthStencilStateCreateInfo::depthWriteEnable = is_enable;
    }

    inline void DepthStencilState::depthBoundsTestEnable(VkBool32 is_enable) noexcept
    {
        VkPipelineDepthStencilStateCreateInfo::depthBoundsTestEnable = is_enable;
    }

    inline void DepthStencilState::stencilTestEnable(VkBool32 is_enable) noexcept
    {
        VkPipelineDepthStencilStateCreateInfo::stencilTestEnable = is_enable;
    }

    inline VkCompareOp DepthStencilState::getDepthCompareOp() noexcept
    {
        return VkPipelineDepthStencilStateCreateInfo::depthCompareOp;
    }

    inline VkStencilOpState DepthStencilState::getFront() noexcept
    {
        return VkPipelineDepthStencilStateCreateInfo::front;
    }

    inline VkStencilOpState DepthStencilState::getBack() noexcept
    {
        return VkPipelineDepthStencilStateCreateInfo::back;
    }

    inline float DepthStencilState::getMinDepthBounds() noexcept
    {
        return VkPipelineDepthStencilStateCreateInfo::minDepthBounds;
    }

    inline float DepthStencilState::getMaxDepthBounds() noexcept
    {
        return VkPipelineDepthStencilStateCreateInfo::maxDepthBounds;
    }

    inline VkBool32 DepthStencilState::depthTestEnable() noexcept
    {
        return VkPipelineDepthStencilStateCreateInfo::depthTestEnable;
    }

    inline VkBool32 DepthStencilState::depthWriteEnable() noexcept
    {
        return VkPipelineDepthStencilStateCreateInfo::depthWriteEnable;
    }

    inline VkBool32 DepthStencilState::depthBoundsTestEnable() noexcept
    {
        return VkPipelineDepthStencilStateCreateInfo::depthBoundsTestEnable;
    }

    inline VkBool32 DepthStencilState::stencilTestEnable() noexcept
    {
        return VkPipelineDepthStencilStateCreateInfo::stencilTestEnable;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inline ViewportState::ViewportState(size_t num_viewports, size_t num_scissors) :
        VkPipelineViewportStateCreateInfo {
            .sType          = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
            .pNext          = nullptr,
            .flags          = 0,
            .viewportCount  = static_cast<uint32_t>(num_viewports),
            .scissorCount   = static_cast<uint32_t>(num_scissors)
        },
        _current_viewport   (0),
        _current_scissor    (0)
    {
        assert(num_viewports > 0 && num_scissors > 0);
        VkPipelineViewportStateCreateInfo::pViewports   = _ptr_viewports    = new VkViewport    [num_viewports];
        VkPipelineViewportStateCreateInfo::pScissors    = _ptr_scissors     = new VkRect2D      [num_scissors];
    }

    inline ViewportState::ViewportState(ViewportState&& viewport_state) :
        _ptr_viewports  (nullptr),
        _ptr_scissors   (nullptr)
    {
        swap(_ptr_viewports,    viewport_state._ptr_viewports);
        swap(_ptr_scissors,     viewport_state._ptr_scissors);

        _current_viewport   = viewport_state._current_viewport;
        _current_scissor    = viewport_state._current_scissor;

        memcpy(this, &viewport_state, sizeof(VkPipelineViewportStateCreateInfo));
    }

    inline ViewportState::ViewportState(const ViewportState& viewport_state) :
        VkPipelineViewportStateCreateInfo {
            .sType          = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
            .pNext          = nullptr,
            .flags          = 0,
            .viewportCount  = viewport_state.viewportCount,
            .scissorCount   = viewport_state.scissorCount
        },
        _ptr_viewports      (new VkViewport [viewport_state.viewportCount]),
        _ptr_scissors       (new VkRect2D   [viewport_state.scissorCount]),
        _current_viewport   (viewport_state._current_viewport),
        _current_scissor    (viewport_state._current_scissor)
    {
        VkPipelineViewportStateCreateInfo::pViewports   = _ptr_viewports;
        VkPipelineViewportStateCreateInfo::pScissors    = _ptr_scissors;

        memcpy(_ptr_viewports,  viewport_state._ptr_viewports,  sizeof(VkViewport) * _current_viewport);
        memcpy(_ptr_scissors,   viewport_state._ptr_scissors,   sizeof(VkRect2D) * _current_scissor);
    }

    inline ViewportState::~ViewportState()
    {
        if (_ptr_viewports) {
            delete[] _ptr_viewports;
        }

        if (_ptr_scissors) {
            delete[] _ptr_scissors;
        }
    }

    inline void ViewportState::addViewport(float x, float y, float width, float height, float min_depth, float max_depth)
    {
        assert(VkPipelineViewportStateCreateInfo::viewportCount > _current_viewport);
        assert(min_depth >= 0.0f && max_depth <= 1.0f);

        _ptr_viewports[_current_viewport] = {
            .x          = x,
            .y          = y,
            .width      = width,
            .height     = height,
            .minDepth   = min_depth,
            .maxDepth   = max_depth
        };

        _current_viewport++;
    }

    inline void ViewportState::addScissor(int32_t x_offset, int32_t y_offset, uint32_t width, uint32_t height)
    {
        assert(VkPipelineViewportStateCreateInfo::scissorCount > _current_scissor);

        _ptr_scissors[_current_scissor] = {
            .offset = {x_offset,    y_offset},
            .extent = {width,       height}
        };

        _current_scissor++;
    }

    inline const VkViewport* ViewportState::getViewports() const noexcept
    {
        return _ptr_viewports;
    }

    inline const VkRect2D* ViewportState::getScissors() const noexcept
    {
        return _ptr_scissors;
    }

    inline size_t ViewportState::numViewports() const noexcept
    {
        return _current_viewport;
    }

    inline size_t ViewportState::numScissors() const noexcept
    {
        return _current_scissor;
    }

    inline size_t ViewportState::capacityViewports() const noexcept
    {
        return VkPipelineViewportStateCreateInfo::viewportCount;
    }

    inline size_t ViewportState::capacityScissors() const noexcept
    {
        return VkPipelineViewportStateCreateInfo::scissorCount;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inline InputAssemblyState::InputAssemblyState(VkPrimitiveTopology topology) :
        VkPipelineInputAssemblyStateCreateInfo {
            .sType      = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
            .pNext      = nullptr,
            .flags      = 0,
            .topology   = topology 
        }
    {}

    inline void InputAssemblyState::setTopology(VkPrimitiveTopology topology) noexcept
    {
        VkPipelineInputAssemblyStateCreateInfo::topology = topology;
    }

    inline void InputAssemblyState::primitiveRestartEnable(VkBool32 is_enable) noexcept
    {
        VkPipelineInputAssemblyStateCreateInfo::primitiveRestartEnable = is_enable;
    }

    inline VkPrimitiveTopology InputAssemblyState::getTopology() const noexcept
    {
        return VkPipelineInputAssemblyStateCreateInfo::topology;
    }

    inline VkBool32 InputAssemblyState::primitiveRestartEnable() const noexcept
    {
        return VkPipelineInputAssemblyStateCreateInfo::primitiveRestartEnable;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inline ColorBlendState::ColorBlendState(size_t num_attachments) noexcept :
        VkPipelineColorBlendStateCreateInfo {
            .sType              = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
            .pNext              = nullptr,
            .flags              = 0,
            .attachmentCount    = static_cast<uint32_t>(num_attachments)
        },
        _ptr_attachments    (new VkPipelineColorBlendAttachmentState[num_attachments]),
        _current_attachment (0)
    {
        VkPipelineColorBlendStateCreateInfo::pAttachments = _ptr_attachments;
    }

    inline ColorBlendState::ColorBlendState(ColorBlendState&& color_blend_state) :
        _ptr_attachments    (nullptr),
        _current_attachment (color_blend_state._current_attachment)
    {
        swap(_ptr_attachments, color_blend_state._ptr_attachments);
        memcpy(this, &color_blend_state, sizeof(VkPipelineColorBlendStateCreateInfo));
    }


    inline ColorBlendState::ColorBlendState(const ColorBlendState& color_blend_state) :
        _ptr_attachments    (new VkPipelineColorBlendAttachmentState[color_blend_state.attachmentCount]),
        _current_attachment (color_blend_state._current_attachment)
    {
        memcpy(_ptr_attachments,    color_blend_state._ptr_attachments, sizeof(VkPipelineColorBlendAttachmentState) * _current_attachment);
        memcpy(this,                &color_blend_state,                 sizeof(VkPipelineColorBlendStateCreateInfo));
        VkPipelineColorBlendStateCreateInfo::pAttachments = _ptr_attachments;
    }

    inline ColorBlendState::~ColorBlendState()
    {
        if (_ptr_attachments) {
            delete[] _ptr_attachments;
        }
    }

    inline void ColorBlendState::addAttchament(
        VkBool32                blend_enable,
        VkBlendFactor           src_color_blend_factor,
        VkBlendFactor           dst_color_blend_factor,
        VkBlendOp               color_blend_op,
        VkBlendFactor           src_alpha_blend_factor,
        VkBlendFactor           dst_alpha_blend_factor,
        VkBlendOp               alpha_blend_op,
        VkColorComponentFlags   color_write_mask
    )
    {
        assert(VkPipelineColorBlendStateCreateInfo::attachmentCount > _current_attachment);

        _ptr_attachments[-_current_attachment] = {
            .blendEnable            = blend_enable,
            .srcColorBlendFactor    = src_color_blend_factor,
            .dstColorBlendFactor    = dst_color_blend_factor,
            .colorBlendOp           = color_blend_op,
            .srcAlphaBlendFactor    = src_alpha_blend_factor,
            .dstAlphaBlendFactor    = dst_alpha_blend_factor,
            .alphaBlendOp           = alpha_blend_op,
            .colorWriteMask         = color_write_mask
        };

        _current_attachment++;
    }

    inline const VkPipelineColorBlendAttachmentState* ColorBlendState::getAttachments() const noexcept
    {
        return _ptr_attachments;
    }

    inline size_t ColorBlendState::numAttachments() const noexcept
    {
        return _current_attachment;
    }

    inline size_t ColorBlendState::capacityAttachments() const noexcept
    {
        return VkPipelineColorBlendStateCreateInfo::attachmentCount;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inline GraphicsPipelineState::GraphicsPipelineState(
        size_t num_vertex_biding_descriptions,
        size_t num_vertex_attribute_descriptions,
        size_t num_viewports,
        size_t num_scissors,
        size_t num_attachments
    ) :
        _vertex_input_state (num_vertex_biding_descriptions, num_vertex_attribute_descriptions),
        _viewport_state     (num_viewports, num_scissors),
        _color_blend_state  (num_attachments)
    {}

    inline GraphicsPipelineState::GraphicsPipelineState(GraphicsPipelineState&& graphics_pipeline_state) :
        _vertex_input_state     (move(graphics_pipeline_state._vertex_input_state)),
        _multisample_state      (move(graphics_pipeline_state._multisample_state)),
        _rasterization_state    (move(graphics_pipeline_state._rasterization_state)),
        _depth_stencil_state    (move(graphics_pipeline_state._depth_stencil_state)),
        _viewport_state         (move(graphics_pipeline_state._viewport_state)),
        _input_assembly_state   (move(graphics_pipeline_state._input_assembly_state)),
        _color_blend_state      (move(graphics_pipeline_state._color_blend_state))
    {}

    inline GraphicsPipelineState::GraphicsPipelineState(const GraphicsPipelineState& graphics_pipeline_state) :
        _vertex_input_state     (graphics_pipeline_state._vertex_input_state),
        _multisample_state      (graphics_pipeline_state._multisample_state),
        _rasterization_state    (graphics_pipeline_state._rasterization_state),
        _depth_stencil_state    (graphics_pipeline_state._depth_stencil_state),
        _viewport_state         (graphics_pipeline_state._viewport_state),
        _input_assembly_state   (graphics_pipeline_state._input_assembly_state),
        _color_blend_state      (graphics_pipeline_state._color_blend_state)
    {}

    inline VertexInputState& GraphicsPipelineState::getVertexInputState() noexcept
    {
        return _vertex_input_state;
    }
    
    inline const VertexInputState& GraphicsPipelineState::getVertexInputState() const noexcept
    {
        return _vertex_input_state;
    }

    inline MultisampleState& GraphicsPipelineState::getMultisampleState() noexcept
    {
        return _multisample_state;
    }

    inline const MultisampleState& GraphicsPipelineState::getMultisampleState() const noexcept
    {
        return _multisample_state;
    }

    inline RasterizationState& GraphicsPipelineState::getRasterizationState() noexcept
    {
        return _rasterization_state;
    }

    inline const RasterizationState& GraphicsPipelineState::getRasterizationState() const noexcept
    {
        return _rasterization_state;
    }

    inline DepthStencilState& GraphicsPipelineState::getDepthStencilState() noexcept
    {
        return _depth_stencil_state;
    }

    inline const DepthStencilState& GraphicsPipelineState::getDepthStencilState() const noexcept
    {
        return _depth_stencil_state;
    }

    inline ViewportState& GraphicsPipelineState::getViewportState() noexcept
    {
        return _viewport_state;
    }

    inline const ViewportState& GraphicsPipelineState::getViewportState() const noexcept
    {
        return _viewport_state;
    }

    inline InputAssemblyState& GraphicsPipelineState::getInputAssemblyState() noexcept
    {
        return _input_assembly_state;
    }

    inline const InputAssemblyState& GraphicsPipelineState::getInputAssemblyState() const noexcept
    {
        return _input_assembly_state;
    }

    inline ColorBlendState& GraphicsPipelineState::getColorBlendState() noexcept
    {
        return _color_blend_state;
    }

    inline const ColorBlendState& GraphicsPipelineState::getColorBlendState() const noexcept
    {
        return _color_blend_state;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inline GraphicsPipeline::GraphicsPipeline(
        const GraphicsPipelineState&            graphics_pipeline_state,
        const vector<shared_ptr<ShaderModule>>& shaders,
        const shared_ptr<PipelineLayout>&       ptr_pipeline_layout,
        const shared_ptr<RenderPass>&           ptr_render_pass,
        uint32_t                                subpass_index
    ) :
        _subpass_index          (subpass_index),
        _ptr_pipeline_layout    (ptr_pipeline_layout),
        _ptr_render_pass        (ptr_render_pass),
        _state                  (graphics_pipeline_state),
        _pipeline_handle        (VK_NULL_HANDLE),
        _pipeline_cache_handle  (VK_NULL_HANDLE),
        _shaders                (shaders)
    {
        _create_graphics_pipeline();
    }
    
    inline GraphicsPipeline::GraphicsPipeline(
        const GraphicsPipelineState&        graphics_pipeline_state,
        vector<shared_ptr<ShaderModule>>&&  shaders,
        const shared_ptr<PipelineLayout>&   ptr_pipeline_layout,
        const shared_ptr<RenderPass>&       ptr_render_pass,
        uint32_t                            subpass_index
    ) :
        _subpass_index          (subpass_index),
        _ptr_pipeline_layout    (ptr_pipeline_layout),
        _ptr_render_pass        (ptr_render_pass),
        _state                  (graphics_pipeline_state),
        _pipeline_handle        (VK_NULL_HANDLE),
        _pipeline_cache_handle  (VK_NULL_HANDLE),
        _shaders                (move(shaders))
    {
        _create_graphics_pipeline();
    }

    inline GraphicsPipeline::GraphicsPipeline(
        GraphicsPipelineState&&                 graphics_pipeline_state,
        const vector<shared_ptr<ShaderModule>>& shaders,
        const shared_ptr<PipelineLayout>&       ptr_pipeline_layout,
        const shared_ptr<RenderPass>&           ptr_render_pass,
        uint32_t                                subpass_index
    ) :
        _subpass_index          (subpass_index),
        _ptr_pipeline_layout    (ptr_pipeline_layout),
        _ptr_render_pass        (ptr_render_pass),
        _state                  (move(graphics_pipeline_state)),
        _pipeline_handle        (VK_NULL_HANDLE),
        _pipeline_cache_handle  (VK_NULL_HANDLE),
        _shaders                (shaders)
    {
        _create_graphics_pipeline();
    }

    inline GraphicsPipeline::GraphicsPipeline(
        GraphicsPipelineState&&             graphics_pipeline_state,
        vector<shared_ptr<ShaderModule>>&&  shaders,
        const shared_ptr<PipelineLayout>&   ptr_pipeline_layout,
        const shared_ptr<RenderPass>&       ptr_render_pass,
        uint32_t                            subpass_index
    ) :
        _subpass_index          (subpass_index),
        _ptr_pipeline_layout    (ptr_pipeline_layout),
        _ptr_render_pass        (ptr_render_pass),
        _state                  (move(graphics_pipeline_state)),
        _pipeline_handle        (VK_NULL_HANDLE),
        _pipeline_cache_handle  (VK_NULL_HANDLE),
        _shaders                (move(shaders))
    {
        _create_graphics_pipeline();
    }

    inline GraphicsPipeline::GraphicsPipeline(GraphicsPipeline&& graphics_pipeline) :
        _subpass_index          (graphics_pipeline._subpass_index),
        _ptr_pipeline_layout    (move(graphics_pipeline._ptr_pipeline_layout)),
        _ptr_render_pass        (move(graphics_pipeline._ptr_render_pass)),
        _state                  (move(graphics_pipeline._state)),
        _pipeline_handle        (VK_NULL_HANDLE),
        _pipeline_cache_handle  (VK_NULL_HANDLE),
        _shaders                (move(graphics_pipeline._shaders))
    {
        swap(_pipeline_handle,          graphics_pipeline._pipeline_handle);
        swap(_pipeline_cache_handle,    graphics_pipeline._pipeline_cache_handle);
    }

    inline GraphicsPipeline::~GraphicsPipeline()
    {
        auto& ptr_device = _ptr_render_pass->getDevice();

        if (_pipeline_handle != VK_NULL_HANDLE) {
            vkDestroyPipeline(ptr_device->getDeviceHandle(), _pipeline_handle, nullptr);
        }

        if (_pipeline_cache_handle != VK_NULL_HANDLE) {
            vkDestroyPipelineCache(ptr_device->getDeviceHandle(), _pipeline_cache_handle, nullptr);
        }
    }

    void GraphicsPipeline::_create_graphics_pipeline()
    {
        auto& ptr_device = _ptr_render_pass->getDevice();

        VkPipelineCacheCreateInfo pipeline_cache_info {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO
        };

        assert(vkCreatePipelineCache(
            ptr_device->getDeviceHandle(),
            &pipeline_cache_info,
            nullptr,
            &_pipeline_cache_handle
        ) == VK_SUCCESS);

        assert(_pipeline_cache_handle != VK_NULL_HANDLE);

        vector<VkPipelineShaderStageCreateInfo> pipeline_shader_stages_info(_shaders.size());

        for (size_t i{0}; i < pipeline_shader_stages_info.size(); i++) {
            pipeline_shader_stages_info[i] = {
                .sType                  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                .pNext                  = nullptr,
                .flags                  = 0,
                .stage                  = _shaders[i]->getShaderType(),
                .module                 = _shaders[i]->getShaderHandle(),
                .pName                  = "main",
                .pSpecializationInfo    = reinterpret_cast<VkSpecializationInfo*>(&_shaders[i]->getSpecializationInfo())
            };
        };

        VkGraphicsPipelineCreateInfo grahics_pipeline_info {
            .sType                  = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
            .pNext                  = nullptr,
            .flags                  = 0,
            .stageCount             = static_cast<uint32_t>(pipeline_shader_stages_info.size()),
            .pStages                = pipeline_shader_stages_info.data(),
            .pVertexInputState      = reinterpret_cast<VkPipelineVertexInputStateCreateInfo*>   (&_state.getVertexInputState()),
            .pInputAssemblyState    = reinterpret_cast<VkPipelineInputAssemblyStateCreateInfo*> (&_state.getInputAssemblyState()),
            .pTessellationState     = nullptr,
            .pViewportState         = reinterpret_cast<VkPipelineViewportStateCreateInfo*>      (&_state.getViewportState()),
            .pRasterizationState    = reinterpret_cast<VkPipelineRasterizationStateCreateInfo*> (&_state.getRasterizationState()),
            .pMultisampleState      = reinterpret_cast<VkPipelineMultisampleStateCreateInfo*>   (&_state.getMultisampleState()),
            .pDepthStencilState     = reinterpret_cast<VkPipelineDepthStencilStateCreateInfo*>  (&_state.getDepthStencilState()),
            .pColorBlendState       = reinterpret_cast<VkPipelineColorBlendStateCreateInfo*>    (&_state.getColorBlendState()),
            .pDynamicState          = nullptr,
            .layout                 = _ptr_pipeline_layout->getPipelineLayoutHandle(),
            .renderPass             = _ptr_render_pass->getRenderPassHandle(),
            .subpass                = _subpass_index
        };

        assert(vkCreateGraphicsPipelines(
            ptr_device->getDeviceHandle(),
            _pipeline_cache_handle,
            1,
            &grahics_pipeline_info,
            nullptr,
            &_pipeline_handle
        ) == VK_SUCCESS);
        
        assert(_pipeline_handle != VK_NULL_HANDLE);
    }

    inline uint32_t GraphicsPipeline::getSubpassIndex() const noexcept
    {
        return _subpass_index;
    }

    inline shared_ptr<PipelineLayout>& GraphicsPipeline::getPipelineLayout() noexcept
    {
        return _ptr_pipeline_layout;
    }

    inline const shared_ptr<PipelineLayout>& GraphicsPipeline::getPipelineLayout() const noexcept
    {
        return _ptr_pipeline_layout;
    }

    inline GraphicsPipelineState& GraphicsPipeline::getGraphicsPipelineState() noexcept
    {
        return _state;
    }

    inline const GraphicsPipelineState& GraphicsPipeline::getGraphicsPipelineState() const noexcept
    {
        return _state;
    }

    inline VkPipeline GraphicsPipeline::getPipelineHandle() const noexcept
    {
        return _pipeline_handle;
    }

    inline VkPipelineCache GraphicsPipeline::getPipelineCacheHandle() const noexcept
    {
        return _pipeline_cache_handle;
    }

    inline const vector<shared_ptr<ShaderModule>>& GraphicsPipeline::getShaderModules() const noexcept
    {
        return _shaders;
    }
}
