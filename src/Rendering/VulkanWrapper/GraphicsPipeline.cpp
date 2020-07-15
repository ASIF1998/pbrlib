//
//  GraphicsPipeline.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 24/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include "GraphicsPipeline.hpp"

#include "ShaderModule.hpp"

#include <numeric>

namespace pbrlib
{
    VertexInputState::VertexInputState(
        size_t num_vertex_biding_descriptions,
        size_t num_vertex_attribute_descriptions
    ) :
        VkPipelineVertexInputStateCreateInfo {
            .sType                              = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
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

    VertexInputState::VertexInputState(VertexInputState&& vertex_input_state) :
        VkPipelineVertexInputStateCreateInfo {
            .sType                              = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
            .vertexBindingDescriptionCount      = vertex_input_state.vertexBindingDescriptionCount,
            .vertexAttributeDescriptionCount    = vertex_input_state.vertexAttributeDescriptionCount
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

    VertexInputState::VertexInputState(const VertexInputState& vertex_input_state) :
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

    void VertexInputState::addVertexInputBindingDescription(uint32_t binding, uint32_t stride, VkVertexInputRate input_rate)
    {
        assert(_curent_vertex_biding_description < VkPipelineVertexInputStateCreateInfo::vertexBindingDescriptionCount);

        _ptr_vertex_biding_descriptions[_curent_vertex_biding_description] = {
            .binding    = binding,
            .stride     = stride,
            .inputRate  = input_rate
        };

        _curent_vertex_biding_description++;
    }

    void VertexInputState::addVertexInputAttributeDescription(
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

    const VkVertexInputBindingDescription* VertexInputState::getVertexInputBindingDescriptions() const noexcept
    {
        return _ptr_vertex_biding_descriptions;
    }


    const VkVertexInputAttributeDescription* VertexInputState::getVertexInputAttributeDescriptions() const noexcept
    {
        return _ptr_vertex_attribute_descriptions;
    }

    size_t VertexInputState::numVertexInputBindingDescription() const noexcept
    {
        return _curent_vertex_biding_description;
    }

    size_t VertexInputState::numVertexInputAttributeDescription() const noexcept
    {
        return _curent_vertex_attribute_description;
    }

    size_t VertexInputState::capacityVertexInputBindingDescription() const noexcept
    {
        return VkPipelineVertexInputStateCreateInfo::vertexBindingDescriptionCount;
    }

    size_t VertexInputState::capacityVertexInputAttributeDescription() const noexcept
    {
        return VkPipelineVertexInputStateCreateInfo::vertexAttributeDescriptionCount;
    }

    VertexInputState::~VertexInputState()
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

    MultisampleState::MultisampleState(const MultisampleState& multisample_state) noexcept
    {
        memcpy(this, &multisample_state, sizeof(MultisampleState));
        VkPipelineMultisampleStateCreateInfo::pSampleMask = &_sample_mask;
    }

    void MultisampleState::setSampleMask(VkSampleMask sample_mask) noexcept
    {
        _sample_mask = sample_mask;
    }

    void MultisampleState::setRasterizationSamples(VkSampleCountFlagBits num_samples) noexcept
    {
        VkPipelineMultisampleStateCreateInfo::rasterizationSamples = num_samples;
    }

    void MultisampleState::setMinSampleShading(float min_sample_shading) noexcept
    {
        assert(min_sample_shading >= 0.0f && min_sample_shading <= 1.0f);
        VkPipelineMultisampleStateCreateInfo::minSampleShading = min_sample_shading;
    }
    
    void MultisampleState::sampleShadingEnable(VkBool32 is_enable) noexcept
    {
        VkPipelineMultisampleStateCreateInfo::sampleShadingEnable = is_enable;
    }

    void MultisampleState::alphaToCoverageEnable(VkBool32 is_enable) noexcept
    {
        VkPipelineMultisampleStateCreateInfo::alphaToCoverageEnable = is_enable;
    }

    void MultisampleState::alphaToOneEnable(VkBool32 is_enable) noexcept
    {
        VkPipelineMultisampleStateCreateInfo::alphaToOneEnable = is_enable;
    }

    VkSampleMask MultisampleState::getSampleMask() const noexcept
    {
        return _sample_mask;
    }

    VkSampleCountFlagBits MultisampleState::getRasterizationSamples() const noexcept
    {
        return VkPipelineMultisampleStateCreateInfo::rasterizationSamples;
    }

    float MultisampleState::getMinSampleShading() const noexcept
    {
        return VkPipelineMultisampleStateCreateInfo::minSampleShading;
    }

    VkBool32 MultisampleState::sampleShadingEnable() const noexcept
    {
        return VkPipelineMultisampleStateCreateInfo::sampleShadingEnable;
    }

    VkBool32 MultisampleState::alphaToCoverageEnable() const noexcept
    {
        return VkPipelineMultisampleStateCreateInfo::alphaToCoverageEnable;
    }

    VkBool32 MultisampleState::alphaToOneEnable() const noexcept
    {
        return VkPipelineMultisampleStateCreateInfo::alphaToOneEnable;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    RasterizationState::RasterizationState() :
        VkPipelineRasterizationStateCreateInfo {
            .sType      = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
            .pNext      = nullptr,
            .flags      = 0,
            .lineWidth  = 1.0f
        }
    {}

    void RasterizationState::setPolygonMode(VkPolygonMode polygon_mode) noexcept
    {
        VkPipelineRasterizationStateCreateInfo::polygonMode = polygon_mode;
    }

    void RasterizationState::setCullMode(VkCullModeFlags cull_mode) noexcept
    {
        VkPipelineRasterizationStateCreateInfo::cullMode = cull_mode;
    }
    
    void RasterizationState::setFrontFace(VkFrontFace front_face) noexcept
    {
        VkPipelineRasterizationStateCreateInfo::frontFace = front_face;
    }

    void RasterizationState::setDepthBiasConstantFactor(float depth_bias_constant_factor) noexcept
    {
        VkPipelineRasterizationStateCreateInfo::depthBiasConstantFactor = depthBiasConstantFactor;
    }

    void RasterizationState::setDepthBiasClamp(float depth_bias_clamp) noexcept
    {
        VkPipelineRasterizationStateCreateInfo::depthBiasClamp = depth_bias_clamp;
    }

    void RasterizationState::setDepthBiasSlopeFactor(float depts_bias_slope_factor) noexcept
    {
        VkPipelineRasterizationStateCreateInfo::depthBiasSlopeFactor = depts_bias_slope_factor;
    }

    void RasterizationState::setLineWidth(float line_width) noexcept
    {
        VkPipelineRasterizationStateCreateInfo::lineWidth = line_width;
    }

    void RasterizationState::depthClampEnable(VkBool32 is_enable) noexcept
    {
        VkPipelineRasterizationStateCreateInfo::depthClampEnable = is_enable;
    }

    void RasterizationState::rasterizerDiscardEnable(VkBool32 is_enable) noexcept
    {
        VkPipelineRasterizationStateCreateInfo::rasterizerDiscardEnable = is_enable;
    }

    void RasterizationState::depthBiasEnable(VkBool32 is_enable) noexcept
    {
        VkPipelineRasterizationStateCreateInfo::depthBiasEnable = is_enable;
    }

    VkPolygonMode RasterizationState::getPolygonMode() const noexcept
    {
        return VkPipelineRasterizationStateCreateInfo::polygonMode;
    }

    VkCullModeFlags RasterizationState::getCullMode() const noexcept
    {
        return VkPipelineRasterizationStateCreateInfo::cullMode;
    }

    VkFrontFace RasterizationState::getFrontFace() const noexcept
    {
        return VkPipelineRasterizationStateCreateInfo::frontFace;
    }

    float RasterizationState::getDepthBiasConstantFactor() const noexcept
    {
        return VkPipelineRasterizationStateCreateInfo::depthBiasConstantFactor;
    }

    float RasterizationState::getDepthBiasClamp() const noexcept
    {
        return VkPipelineRasterizationStateCreateInfo::depthBiasClamp;
    }

    float RasterizationState::getDepthBiasSlopeFactor() const noexcept
    {
        return VkPipelineRasterizationStateCreateInfo::depthBiasSlopeFactor;
    }

    float RasterizationState::getLineWidth() const noexcept
    {
        return VkPipelineRasterizationStateCreateInfo::lineWidth;
    }

    VkBool32 RasterizationState::depthClampEnable() const noexcept
    {
        return VkPipelineRasterizationStateCreateInfo::depthClampEnable;
    }

    VkBool32 RasterizationState::rasterizerDiscardEnable() const noexcept
    {
        return VkPipelineRasterizationStateCreateInfo::rasterizerDiscardEnable;
    }

    VkBool32 RasterizationState::depthBiasEnable() const noexcept
    {
        return VkPipelineRasterizationStateCreateInfo::depthBiasEnable;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    DepthStencilState::DepthStencilState(VkBool32 depth_test_enable, VkBool32 stencil_test_enable) noexcept :
        VkPipelineDepthStencilStateCreateInfo{
            .sType              = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
            .pNext              = nullptr,
            .flags              = 0,
            .depthTestEnable    = depth_test_enable,
            .stencilTestEnable  = stencil_test_enable
        }
    {}

    void DepthStencilState::setDepthCompareOp(VkCompareOp depth_compare_op) noexcept
    {
        VkPipelineDepthStencilStateCreateInfo::depthCompareOp = depth_compare_op;
    }

    void DepthStencilState::setFront(VkStencilOpState front) noexcept
    {
        VkPipelineDepthStencilStateCreateInfo::front = front;
    }

    void DepthStencilState::setBack(VkStencilOpState back) noexcept
    {
        VkPipelineDepthStencilStateCreateInfo::back = back;
    }

    void DepthStencilState::setMinDepthBounds(float min_depth_bounds) noexcept
    {
        VkPipelineDepthStencilStateCreateInfo::minDepthBounds = min_depth_bounds;
    }

    void DepthStencilState::setMaxDepthBounds(float max_depth_bounds) noexcept
    {
        VkPipelineDepthStencilStateCreateInfo::maxDepthBounds = max_depth_bounds;
    }

    void DepthStencilState::depthTestEnable(VkBool32 is_enable) noexcept
    {
        VkPipelineDepthStencilStateCreateInfo::depthTestEnable = is_enable;
    }

    void DepthStencilState::depthWriteEnable(VkBool32 is_enable) noexcept
    {
        VkPipelineDepthStencilStateCreateInfo::depthWriteEnable = is_enable;
    }

    void DepthStencilState::depthBoundsTestEnable(VkBool32 is_enable) noexcept
    {
        VkPipelineDepthStencilStateCreateInfo::depthBoundsTestEnable = is_enable;
    }

    void DepthStencilState::stencilTestEnable(VkBool32 is_enable) noexcept
    {
        VkPipelineDepthStencilStateCreateInfo::stencilTestEnable = is_enable;
    }

    VkCompareOp DepthStencilState::getDepthCompareOp() noexcept
    {
        return VkPipelineDepthStencilStateCreateInfo::depthCompareOp;
    }

    VkStencilOpState DepthStencilState::getFront() noexcept
    {
        return VkPipelineDepthStencilStateCreateInfo::front;
    }

    VkStencilOpState DepthStencilState::getBack() noexcept
    {
        return VkPipelineDepthStencilStateCreateInfo::back;
    }

    float DepthStencilState::getMinDepthBounds() noexcept
    {
        return VkPipelineDepthStencilStateCreateInfo::minDepthBounds;
    }

    float DepthStencilState::getMaxDepthBounds() noexcept
    {
        return VkPipelineDepthStencilStateCreateInfo::maxDepthBounds;
    }

    VkBool32 DepthStencilState::depthTestEnable() noexcept
    {
        return VkPipelineDepthStencilStateCreateInfo::depthTestEnable;
    }

    VkBool32 DepthStencilState::depthWriteEnable() noexcept
    {
        return VkPipelineDepthStencilStateCreateInfo::depthWriteEnable;
    }

    VkBool32 DepthStencilState::depthBoundsTestEnable() noexcept
    {
        return VkPipelineDepthStencilStateCreateInfo::depthBoundsTestEnable;
    }

    VkBool32 DepthStencilState::stencilTestEnable() noexcept
    {
        return VkPipelineDepthStencilStateCreateInfo::stencilTestEnable;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ViewportState::ViewportState(size_t num_viewports, size_t num_scissors) :
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

    ViewportState::ViewportState(ViewportState&& viewport_state) :
        _ptr_viewports  (nullptr),
        _ptr_scissors   (nullptr)
    {
        swap(_ptr_viewports,    viewport_state._ptr_viewports);
        swap(_ptr_scissors,     viewport_state._ptr_scissors);

        _current_viewport   = viewport_state._current_viewport;
        _current_scissor    = viewport_state._current_scissor;

        memcpy(this, &viewport_state, sizeof(VkPipelineViewportStateCreateInfo));
    }

    ViewportState::ViewportState(const ViewportState& viewport_state) :
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

    ViewportState::~ViewportState()
    {
        if (_ptr_viewports) {
            delete[] _ptr_viewports;
        }

        if (_ptr_scissors) {
            delete[] _ptr_scissors;
        }
    }

    void ViewportState::addViewport(float x, float y, float width, float height, float min_depth, float max_depth)
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

    void ViewportState::addScissor(int32_t x_offset, int32_t y_offset, uint32_t width, uint32_t height)
    {
        assert(VkPipelineViewportStateCreateInfo::scissorCount > _current_scissor);

        _ptr_scissors[_current_scissor] = {
            .offset = {x_offset,    y_offset},
            .extent = {width,       height}
        };

        _current_scissor++;
    }

    const VkViewport* ViewportState::getViewports() const noexcept
    {
        return _ptr_viewports;
    }

    const VkRect2D* ViewportState::getScissors() const noexcept
    {
        return _ptr_scissors;
    }

    size_t ViewportState::numViewports() const noexcept
    {
        return _current_viewport;
    }

    size_t ViewportState::numScissors() const noexcept
    {
        return _current_scissor;
    }

    size_t ViewportState::capacityViewports() const noexcept
    {
        return VkPipelineViewportStateCreateInfo::viewportCount;
    }

    size_t ViewportState::capacityScissors() const noexcept
    {
        return VkPipelineViewportStateCreateInfo::scissorCount;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    InputAssemblyState::InputAssemblyState(VkPrimitiveTopology topology) :
        VkPipelineInputAssemblyStateCreateInfo {
            .sType      = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
            .pNext      = nullptr,
            .flags      = 0,
            .topology   = topology 
        }
    {}

    void InputAssemblyState::setTopology(VkPrimitiveTopology topology) noexcept
    {
        VkPipelineInputAssemblyStateCreateInfo::topology = topology;
    }

    void InputAssemblyState::primitiveRestartEnable(VkBool32 is_enable) noexcept
    {
        VkPipelineInputAssemblyStateCreateInfo::primitiveRestartEnable = is_enable;
    }

    VkPrimitiveTopology InputAssemblyState::getTopology() const noexcept
    {
        return VkPipelineInputAssemblyStateCreateInfo::topology;
    }

    VkBool32 InputAssemblyState::primitiveRestartEnable() const noexcept
    {
        return VkPipelineInputAssemblyStateCreateInfo::primitiveRestartEnable;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ColorBlendState::ColorBlendState(size_t num_attachments) noexcept :
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

    ColorBlendState::ColorBlendState(ColorBlendState&& color_blend_state) :
        _ptr_attachments    (nullptr),
        _current_attachment (color_blend_state._current_attachment)
    {
        swap(_ptr_attachments, color_blend_state._ptr_attachments);
        memcpy(this, &color_blend_state, sizeof(VkPipelineColorBlendStateCreateInfo));
    }


    ColorBlendState::ColorBlendState(const ColorBlendState& color_blend_state) :
        _ptr_attachments    (new VkPipelineColorBlendAttachmentState[color_blend_state.attachmentCount]),
        _current_attachment (color_blend_state._current_attachment)
    {
        memcpy(_ptr_attachments,    color_blend_state._ptr_attachments, sizeof(VkPipelineColorBlendAttachmentState) * _current_attachment);
        memcpy(this,                &color_blend_state,                 sizeof(VkPipelineColorBlendStateCreateInfo));
        VkPipelineColorBlendStateCreateInfo::pAttachments = _ptr_attachments;
    }

    ColorBlendState::~ColorBlendState()
    {
        if (_ptr_attachments) {
            delete[] _ptr_attachments;
        }
    }

    void ColorBlendState::addAttchament(
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

        _ptr_attachments[_current_attachment] = {
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

    void ColorBlendState::logicOpEnable(VkBool32 is_enable) noexcept
    {
        VkPipelineColorBlendStateCreateInfo::logicOpEnable = is_enable;
    }

    void ColorBlendState::setLogicOp(VkLogicOp logic_op) noexcept
    {
        VkPipelineColorBlendStateCreateInfo::logicOp= logic_op;
    }

    const VkPipelineColorBlendAttachmentState* ColorBlendState::getAttachments() const noexcept
    {
        return _ptr_attachments;
    }

    size_t ColorBlendState::numAttachments() const noexcept
    {
        return _current_attachment;
    }

    size_t ColorBlendState::capacityAttachments() const noexcept
    {
        return VkPipelineColorBlendStateCreateInfo::attachmentCount;
    }

    bool ColorBlendState::logicOpEnable() const noexcept
    {
        return VkPipelineColorBlendStateCreateInfo::logicOpEnable;
    }

    VkLogicOp ColorBlendState::getLogicOp() const noexcept
    {
        return VkPipelineColorBlendStateCreateInfo::logicOp;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    GraphicsPipelineState::GraphicsPipelineState(
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

    GraphicsPipelineState::GraphicsPipelineState(GraphicsPipelineState&& graphics_pipeline_state) :
        _vertex_input_state     (move(graphics_pipeline_state._vertex_input_state)),
        _multisample_state      (move(graphics_pipeline_state._multisample_state)),
        _rasterization_state    (move(graphics_pipeline_state._rasterization_state)),
        _depth_stencil_state    (move(graphics_pipeline_state._depth_stencil_state)),
        _viewport_state         (move(graphics_pipeline_state._viewport_state)),
        _input_assembly_state   (move(graphics_pipeline_state._input_assembly_state)),
        _color_blend_state      (move(graphics_pipeline_state._color_blend_state))
    {}

    GraphicsPipelineState::GraphicsPipelineState(const GraphicsPipelineState& graphics_pipeline_state) :
        _vertex_input_state     (graphics_pipeline_state._vertex_input_state),
        _multisample_state      (graphics_pipeline_state._multisample_state),
        _rasterization_state    (graphics_pipeline_state._rasterization_state),
        _depth_stencil_state    (graphics_pipeline_state._depth_stencil_state),
        _viewport_state         (graphics_pipeline_state._viewport_state),
        _input_assembly_state   (graphics_pipeline_state._input_assembly_state),
        _color_blend_state      (graphics_pipeline_state._color_blend_state)
    {}

    VertexInputState& GraphicsPipelineState::getVertexInputState() noexcept
    {
        return _vertex_input_state;
    }
    
    const VertexInputState& GraphicsPipelineState::getVertexInputState() const noexcept
    {
        return _vertex_input_state;
    }

    MultisampleState& GraphicsPipelineState::getMultisampleState() noexcept
    {
        return _multisample_state;
    }

    const MultisampleState& GraphicsPipelineState::getMultisampleState() const noexcept
    {
        return _multisample_state;
    }

    RasterizationState& GraphicsPipelineState::getRasterizationState() noexcept
    {
        return _rasterization_state;
    }

    const RasterizationState& GraphicsPipelineState::getRasterizationState() const noexcept
    {
        return _rasterization_state;
    }

    DepthStencilState& GraphicsPipelineState::getDepthStencilState() noexcept
    {
        return _depth_stencil_state;
    }

    const DepthStencilState& GraphicsPipelineState::getDepthStencilState() const noexcept
    {
        return _depth_stencil_state;
    }

    ViewportState& GraphicsPipelineState::getViewportState() noexcept
    {
        return _viewport_state;
    }

    const ViewportState& GraphicsPipelineState::getViewportState() const noexcept
    {
        return _viewport_state;
    }

    InputAssemblyState& GraphicsPipelineState::getInputAssemblyState() noexcept
    {
        return _input_assembly_state;
    }

    const InputAssemblyState& GraphicsPipelineState::getInputAssemblyState() const noexcept
    {
        return _input_assembly_state;
    }

    ColorBlendState& GraphicsPipelineState::getColorBlendState() noexcept
    {
        return _color_blend_state;
    }

    const ColorBlendState& GraphicsPipelineState::getColorBlendState() const noexcept
    {
        return _color_blend_state;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    GraphicsPipeline::GraphicsPipeline(
        const GraphicsPipelineState&            graphics_pipeline_state,
        const vector<ShaderModule>&             shaders,
        const PtrPipelineLayout&                ptr_pipeline_layout,
        const PtrRenderPass&                    ptr_render_pass,
        uint32_t                                subpass_index
    ) :
        _subpass_index          (subpass_index),
        _ptr_pipeline_layout    (ptr_pipeline_layout),
        _ptr_render_pass        (ptr_render_pass),
        _state                  (graphics_pipeline_state),
        _pipeline_handle        (VK_NULL_HANDLE),
        _pipeline_cache_handle  (VK_NULL_HANDLE)
    {
        _create(shaders);
    }
    
    GraphicsPipeline::GraphicsPipeline(
        GraphicsPipelineState&&                 graphics_pipeline_state,
        const vector<ShaderModule>&             shaders,
        const PtrPipelineLayout&                ptr_pipeline_layout,
        const PtrRenderPass&                    ptr_render_pass,
        uint32_t                                subpass_index
    ) :
        _subpass_index          (subpass_index),
        _ptr_pipeline_layout    (ptr_pipeline_layout),
        _ptr_render_pass        (ptr_render_pass),
        _state                  (move(graphics_pipeline_state)),
        _pipeline_handle        (VK_NULL_HANDLE),
        _pipeline_cache_handle  (VK_NULL_HANDLE)
    {
        _create(shaders);
    }

    GraphicsPipeline::GraphicsPipeline(GraphicsPipeline&& graphics_pipeline) :
        _subpass_index          (graphics_pipeline._subpass_index),
        _ptr_pipeline_layout    (move(graphics_pipeline._ptr_pipeline_layout)),
        _ptr_render_pass        (move(graphics_pipeline._ptr_render_pass)),
        _state                  (move(graphics_pipeline._state)),
        _pipeline_handle        (VK_NULL_HANDLE),
        _pipeline_cache_handle  (VK_NULL_HANDLE)
    {
        swap(_pipeline_handle,          graphics_pipeline._pipeline_handle);
        swap(_pipeline_cache_handle,    graphics_pipeline._pipeline_cache_handle);
    }

    GraphicsPipeline::~GraphicsPipeline()
    {
        auto& ptr_device = _ptr_render_pass->getDevice();

        if (_pipeline_handle != VK_NULL_HANDLE) {
            vkDestroyPipeline(ptr_device->getDeviceHandle(), _pipeline_handle, nullptr);
        }

        if (_pipeline_cache_handle != VK_NULL_HANDLE) {
            vkDestroyPipelineCache(ptr_device->getDeviceHandle(), _pipeline_cache_handle, nullptr);
        }
    }

    void GraphicsPipeline::_create(const vector<ShaderModule>& shaders)
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

       vector<VkPipelineShaderStageCreateInfo> pipeline_shader_stages_info(shaders.size());

       for (size_t i{0}; i < pipeline_shader_stages_info.size(); i++) {
           pipeline_shader_stages_info[i] = {
               .sType                  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
               .pNext                  = nullptr,
               .flags                  = 0,
               .stage                  = shaders[i].getShaderType(),
               .module                 = shaders[i].getShaderHandle(),
               .pName                  = "main",
               .pSpecializationInfo    = reinterpret_cast<const VkSpecializationInfo*>(&shaders[i].getSpecializationInfo())
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

    uint32_t GraphicsPipeline::getSubpassIndex() const noexcept
    {
        return _subpass_index;
    }

    PtrPipelineLayout& GraphicsPipeline::getPipelineLayout() noexcept
    {
        return _ptr_pipeline_layout;
    }

    const PtrPipelineLayout& GraphicsPipeline::getPipelineLayout() const noexcept
    {
        return _ptr_pipeline_layout;
    }

    GraphicsPipelineState& GraphicsPipeline::getGraphicsPipelineState() noexcept
    {
        return _state;
    }

    const GraphicsPipelineState& GraphicsPipeline::getGraphicsPipelineState() const noexcept
    {
        return _state;
    }

    VkPipeline GraphicsPipeline::getPipelineHandle() const noexcept
    {
        return _pipeline_handle;
    }

    VkPipelineCache GraphicsPipeline::getPipelineCacheHandle() const noexcept
    {
        return _pipeline_cache_handle;
    }

    PtrGraphicsPipeline GraphicsPipeline::make(
        const GraphicsPipelineState&            graphics_pipeline_state,
        const vector<ShaderModule>&             shaders,
        const PtrPipelineLayout&       ptr_pipeline_layout,
        const PtrRenderPass&           ptr_render_pass,
        uint32_t                                subpass_index
    )
    {
        return make_shared<GraphicsPipeline>(
            graphics_pipeline_state,
            shaders,
            ptr_pipeline_layout,
            ptr_render_pass,
            subpass_index
        );
    }

    PtrGraphicsPipeline GraphicsPipeline::make(
        GraphicsPipelineState&&                 graphics_pipeline_state,
        const vector<ShaderModule>&             shaders,
        const PtrPipelineLayout&       ptr_pipeline_layout,
        const PtrRenderPass&           ptr_render_pass,
        uint32_t                                subpass_index
    )
    {
        return make_shared<GraphicsPipeline>(
            move(graphics_pipeline_state),
            shaders,
            ptr_pipeline_layout,
            ptr_render_pass,
            subpass_index
        );
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    GraphicsPipeline::Builder::Builder(
        size_t num_vertex_biding_descriptions,
        size_t num_vertex_attribute_descriptions,
        size_t num_viewports,
        size_t num_scissors,
        size_t num_attachments
    ) :
        GraphicsPipelineState(
            num_vertex_biding_descriptions, 
            num_vertex_attribute_descriptions, 
            num_viewports, 
            num_scissors, 
            num_attachments
        ),
        _subpass_index(0)
    {}

    void GraphicsPipeline::Builder::setPipleineLayout(const PtrPipelineLayout& ptr_pipeline_layout)
    {
        _ptr_pipeline_layout = ptr_pipeline_layout;
    }

    void GraphicsPipeline::Builder::setRenderPass(const PtrRenderPass& ptr_render_pass)
    {
        _ptr_render_pass = ptr_render_pass;
    }
    
    void GraphicsPipeline::Builder::setSubpassIndex(uint32_t subpass_index) noexcept
    {
        _subpass_index = subpass_index;
    }

    void GraphicsPipeline::Builder::setShadersModules(vector<ShaderModule>&& shaders)
    {
        swap(_shaders, shaders);
    }

    void GraphicsPipeline::Builder::addShader(ShaderModule&& shader_module)
    {
        _shaders.push_back(move(shader_module));
    }

    GraphicsPipeline GraphicsPipeline::Builder::build() const
    {
        return GraphicsPipeline(
            *this,
            _shaders,
            _ptr_pipeline_layout,
            _ptr_render_pass,
            _subpass_index
        );
    }

    PtrGraphicsPipeline GraphicsPipeline::Builder::buildPtr() const
    {
        return make_shared<GraphicsPipeline>(
            *this,
            _shaders,
            _ptr_pipeline_layout,
            _ptr_render_pass,
            _subpass_index
        );
    }
}
