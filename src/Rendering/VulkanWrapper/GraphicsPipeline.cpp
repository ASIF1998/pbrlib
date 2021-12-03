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

#include <cassert>

namespace pbrlib
{
    VertexInputState::VertexInputState(
        size_t num_vertex_biding_descriptions,
        size_t num_vertex_attribute_descriptions
    ) :
        _ptr_vertex_biding_descriptions     (new VkVertexInputBindingDescription[num_vertex_biding_descriptions]),
        _ptr_vertex_attribute_descriptions  (new VkVertexInputAttributeDescription [num_vertex_attribute_descriptions]),
        _curent_vertex_biding_description   (0),
        _curent_vertex_attribute_description(0)
    {
        _pipeline_vertex_input_state_ci = { };
        _pipeline_vertex_input_state_ci.sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        _pipeline_vertex_input_state_ci.vertexBindingDescriptionCount   = static_cast<uint32_t>(num_vertex_biding_descriptions);
        _pipeline_vertex_input_state_ci.vertexAttributeDescriptionCount = static_cast<uint32_t>(num_vertex_attribute_descriptions);
        _pipeline_vertex_input_state_ci.pVertexBindingDescriptions      = _ptr_vertex_biding_descriptions;
        _pipeline_vertex_input_state_ci.pVertexAttributeDescriptions    = _ptr_vertex_attribute_descriptions;
    }

    VertexInputState::VertexInputState(VertexInputState&& vertex_input_state) :
        _ptr_vertex_biding_descriptions         (nullptr),
        _ptr_vertex_attribute_descriptions      (nullptr),
        _curent_vertex_biding_description       (vertex_input_state._curent_vertex_biding_description),
        _curent_vertex_attribute_description    (vertex_input_state._curent_vertex_attribute_description)
    {
        _pipeline_vertex_input_state_ci = { };
        _pipeline_vertex_input_state_ci.sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        _pipeline_vertex_input_state_ci.vertexBindingDescriptionCount   = vertex_input_state._pipeline_vertex_input_state_ci.vertexBindingDescriptionCount;
        _pipeline_vertex_input_state_ci.vertexAttributeDescriptionCount = vertex_input_state._pipeline_vertex_input_state_ci.vertexAttributeDescriptionCount;

        swap(_pipeline_vertex_input_state_ci.pVertexBindingDescriptions, vertex_input_state._pipeline_vertex_input_state_ci.pVertexBindingDescriptions);
        swap(_pipeline_vertex_input_state_ci.pVertexAttributeDescriptions, vertex_input_state._pipeline_vertex_input_state_ci.pVertexAttributeDescriptions);
        swap(_ptr_vertex_biding_descriptions, vertex_input_state._ptr_vertex_biding_descriptions);
        swap(_ptr_vertex_attribute_descriptions, vertex_input_state._ptr_vertex_attribute_descriptions);
        
        vertex_input_state._pipeline_vertex_input_state_ci.vertexBindingDescriptionCount    = 0;
        vertex_input_state._pipeline_vertex_input_state_ci.vertexAttributeDescriptionCount  = 0;
    }

    VertexInputState::VertexInputState(const VertexInputState& vertex_input_state) :
        _ptr_vertex_biding_descriptions         (new VkVertexInputBindingDescription[vertex_input_state._pipeline_vertex_input_state_ci.vertexBindingDescriptionCount]),
        _ptr_vertex_attribute_descriptions      (new VkVertexInputAttributeDescription [vertex_input_state._pipeline_vertex_input_state_ci.vertexAttributeDescriptionCount]),
        _curent_vertex_biding_description       (vertex_input_state._curent_vertex_biding_description),
        _curent_vertex_attribute_description    (vertex_input_state._curent_vertex_attribute_description)
    {
        _pipeline_vertex_input_state_ci = { };
        _pipeline_vertex_input_state_ci.sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        _pipeline_vertex_input_state_ci.vertexBindingDescriptionCount   = vertex_input_state._pipeline_vertex_input_state_ci.vertexBindingDescriptionCount;
        _pipeline_vertex_input_state_ci.vertexAttributeDescriptionCount = vertex_input_state._pipeline_vertex_input_state_ci.vertexAttributeDescriptionCount;

        memcpy(_ptr_vertex_biding_descriptions,
               vertex_input_state._ptr_vertex_biding_descriptions,
               vertex_input_state._pipeline_vertex_input_state_ci.vertexBindingDescriptionCount * sizeof(VkVertexInputBindingDescription));
        
        memcpy(_ptr_vertex_attribute_descriptions,
               vertex_input_state._ptr_vertex_attribute_descriptions,
               vertex_input_state._pipeline_vertex_input_state_ci.vertexAttributeDescriptionCount * sizeof(VkVertexInputAttributeDescription));
        
        _pipeline_vertex_input_state_ci.pVertexBindingDescriptions      = _ptr_vertex_biding_descriptions;
        _pipeline_vertex_input_state_ci.pVertexAttributeDescriptions    = _ptr_vertex_attribute_descriptions;
    }

    void VertexInputState::addVertexInputBindingDescription(uint32_t binding, uint32_t stride, VkVertexInputRate input_rate)
    {
        assert(_curent_vertex_biding_description < _pipeline_vertex_input_state_ci.vertexBindingDescriptionCount);

        _ptr_vertex_biding_descriptions[_curent_vertex_biding_description] = { };
        _ptr_vertex_biding_descriptions[_curent_vertex_biding_description].binding      = binding;
        _ptr_vertex_biding_descriptions[_curent_vertex_biding_description].stride       = stride;
        _ptr_vertex_biding_descriptions[_curent_vertex_biding_description].inputRate    = input_rate;

        _curent_vertex_biding_description++;
    }

    void VertexInputState::addVertexInputAttributeDescription(
        uint32_t location,
        uint32_t binding,
        VkFormat format,
        uint32_t offset
    )
    {
        assert(_curent_vertex_attribute_description < _pipeline_vertex_input_state_ci.vertexAttributeDescriptionCount);

        _ptr_vertex_attribute_descriptions[_curent_vertex_attribute_description] = { };
        _ptr_vertex_attribute_descriptions[_curent_vertex_attribute_description].location   = location;
        _ptr_vertex_attribute_descriptions[_curent_vertex_attribute_description].binding    = binding;
        _ptr_vertex_attribute_descriptions[_curent_vertex_attribute_description].format     = format;
        _ptr_vertex_attribute_descriptions[_curent_vertex_attribute_description].offset     = offset;

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
        return _pipeline_vertex_input_state_ci.vertexBindingDescriptionCount;
    }

    size_t VertexInputState::capacityVertexInputAttributeDescription() const noexcept
    {
        return _pipeline_vertex_input_state_ci.vertexAttributeDescriptionCount;
    }

    const VkPipelineVertexInputStateCreateInfo& VertexInputState::getPipelineVertexInputStateCreateInfo() const noexcept
    {
        return _pipeline_vertex_input_state_ci;
    }

    VkPipelineVertexInputStateCreateInfo& VertexInputState::getPipelineVertexInputStateCreateInfo() noexcept
    {
        return _pipeline_vertex_input_state_ci;
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
        _sample_mask(numeric_limits<decltype(_sample_mask)>::max())
    {
        _pipeline_multisample_state_ci = { };
        _pipeline_multisample_state_ci.sType                    = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        _pipeline_multisample_state_ci.rasterizationSamples     = num_samples;
        _pipeline_multisample_state_ci.sampleShadingEnable      = sample_shading_enable;
        _pipeline_multisample_state_ci.minSampleShading         = min_sample_shading;
        _pipeline_multisample_state_ci.pSampleMask              = &_sample_mask;
        _pipeline_multisample_state_ci.alphaToCoverageEnable    = alpha_to_coverage_enable;
        _pipeline_multisample_state_ci.alphaToOneEnable         = alpha_to_one_enable;
    }

    MultisampleState::MultisampleState(const MultisampleState& multisample_state) noexcept
    {
        memcpy(this, &multisample_state, sizeof(MultisampleState));
        _pipeline_multisample_state_ci.pSampleMask = &_sample_mask;
    }

    void MultisampleState::setSampleMask(VkSampleMask sample_mask) noexcept
    {
        _sample_mask = sample_mask;
    }

    void MultisampleState::setRasterizationSamples(VkSampleCountFlagBits num_samples) noexcept
    {
        _pipeline_multisample_state_ci.rasterizationSamples = num_samples;
    }

    void MultisampleState::setMinSampleShading(float min_sample_shading) noexcept
    {
        assert(min_sample_shading >= 0.0f && min_sample_shading <= 1.0f);
        _pipeline_multisample_state_ci.minSampleShading = min_sample_shading;
    }
    
    const VkPipelineMultisampleStateCreateInfo& MultisampleState::getPipelineMultisampleStateCreateInfo() const noexcept
    {
        return _pipeline_multisample_state_ci;
    }

    VkPipelineMultisampleStateCreateInfo& MultisampleState::getPipelineMultisampleStateCreateInfo() noexcept
    {
        return _pipeline_multisample_state_ci;
    }

    void MultisampleState::sampleShadingEnable(VkBool32 is_enable) noexcept
    {
        _pipeline_multisample_state_ci.sampleShadingEnable = is_enable;
    }

    void MultisampleState::alphaToCoverageEnable(VkBool32 is_enable) noexcept
    {
        _pipeline_multisample_state_ci.alphaToCoverageEnable = is_enable;
    }

    void MultisampleState::alphaToOneEnable(VkBool32 is_enable) noexcept
    {
        _pipeline_multisample_state_ci.alphaToOneEnable = is_enable;
    }

    VkSampleMask MultisampleState::getSampleMask() const noexcept
    {
        return _sample_mask;
    }

    VkSampleCountFlagBits MultisampleState::getRasterizationSamples() const noexcept
    {
        return _pipeline_multisample_state_ci.rasterizationSamples;
    }

    float MultisampleState::getMinSampleShading() const noexcept
    {
        return _pipeline_multisample_state_ci.minSampleShading;
    }

    VkBool32 MultisampleState::sampleShadingEnable() const noexcept
    {
        return _pipeline_multisample_state_ci.sampleShadingEnable;
    }

    VkBool32 MultisampleState::alphaToCoverageEnable() const noexcept
    {
        return _pipeline_multisample_state_ci.alphaToCoverageEnable;
    }

    VkBool32 MultisampleState::alphaToOneEnable() const noexcept
    {
        return _pipeline_multisample_state_ci.alphaToOneEnable;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    RasterizationState::RasterizationState() 
    {
        _pipeline_rasterization_state_ci = { };
        _pipeline_rasterization_state_ci.sType      = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        _pipeline_rasterization_state_ci.lineWidth  = 1.0f;
    }

    void RasterizationState::setPolygonMode(VkPolygonMode polygon_mode) noexcept
    {
        _pipeline_rasterization_state_ci.polygonMode = polygon_mode;
    }

    void RasterizationState::setCullMode(VkCullModeFlags cull_mode) noexcept
    {
        _pipeline_rasterization_state_ci.cullMode = cull_mode;
    }
    
    void RasterizationState::setFrontFace(VkFrontFace front_face) noexcept
    {
        _pipeline_rasterization_state_ci.frontFace = front_face;
    }

    void RasterizationState::setDepthBiasConstantFactor(float depth_bias_constant_factor) noexcept
    {
        _pipeline_rasterization_state_ci.depthBiasConstantFactor = depth_bias_constant_factor;
    }

    void RasterizationState::setDepthBiasClamp(float depth_bias_clamp) noexcept
    {
        _pipeline_rasterization_state_ci.depthBiasClamp = depth_bias_clamp;
    }

    void RasterizationState::setDepthBiasSlopeFactor(float depts_bias_slope_factor) noexcept
    {
        _pipeline_rasterization_state_ci.depthBiasSlopeFactor = depts_bias_slope_factor;
    }

    void RasterizationState::setLineWidth(float line_width) noexcept
    {
        _pipeline_rasterization_state_ci.lineWidth = line_width;
    }

    void RasterizationState::depthClampEnable(VkBool32 is_enable) noexcept
    {
        _pipeline_rasterization_state_ci.depthClampEnable = is_enable;
    }

    void RasterizationState::rasterizerDiscardEnable(VkBool32 is_enable) noexcept
    {
        _pipeline_rasterization_state_ci.rasterizerDiscardEnable = is_enable;
    }

    void RasterizationState::depthBiasEnable(VkBool32 is_enable) noexcept
    {
        _pipeline_rasterization_state_ci.depthBiasEnable = is_enable;
    }

    const VkPipelineRasterizationStateCreateInfo& RasterizationState::getPipelineRasterizationStateCreateInfo() const noexcept
    {
        return _pipeline_rasterization_state_ci;
    }

    VkPipelineRasterizationStateCreateInfo& RasterizationState::getPipelineRasterizationStateCreateInfo() noexcept
    {
        return _pipeline_rasterization_state_ci;
    }

    VkPolygonMode RasterizationState::getPolygonMode() const noexcept
    {
        return _pipeline_rasterization_state_ci.polygonMode;
    }

    VkCullModeFlags RasterizationState::getCullMode() const noexcept
    {
        return _pipeline_rasterization_state_ci.cullMode;
    }

    VkFrontFace RasterizationState::getFrontFace() const noexcept
    {
        return _pipeline_rasterization_state_ci.frontFace;
    }

    float RasterizationState::getDepthBiasConstantFactor() const noexcept
    {
        return _pipeline_rasterization_state_ci.depthBiasConstantFactor;
    }

    float RasterizationState::getDepthBiasClamp() const noexcept
    {
        return _pipeline_rasterization_state_ci.depthBiasClamp;
    }

    float RasterizationState::getDepthBiasSlopeFactor() const noexcept
    {
        return _pipeline_rasterization_state_ci.depthBiasSlopeFactor;
    }

    float RasterizationState::getLineWidth() const noexcept
    {
        return _pipeline_rasterization_state_ci.lineWidth;
    }

    VkBool32 RasterizationState::depthClampEnable() const noexcept
    {
        return _pipeline_rasterization_state_ci.depthClampEnable;
    }

    VkBool32 RasterizationState::rasterizerDiscardEnable() const noexcept
    {
        return _pipeline_rasterization_state_ci.rasterizerDiscardEnable;
    }

    VkBool32 RasterizationState::depthBiasEnable() const noexcept
    {
        return _pipeline_rasterization_state_ci.depthBiasEnable;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    DepthStencilState::DepthStencilState(VkBool32 depth_test_enable, VkBool32 stencil_test_enable) noexcept
    {
        _pipeline_depth_stencil_static_ci = { };
        _pipeline_depth_stencil_static_ci.sType             = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        _pipeline_depth_stencil_static_ci.depthTestEnable   = depth_test_enable;
        _pipeline_depth_stencil_static_ci.stencilTestEnable = stencil_test_enable;
    }

    void DepthStencilState::setDepthCompareOp(VkCompareOp depth_compare_op) noexcept
    {
        _pipeline_depth_stencil_static_ci.depthCompareOp = depth_compare_op;
    }

    void DepthStencilState::setFront(VkStencilOpState front) noexcept
    {
        _pipeline_depth_stencil_static_ci.front = front;
    }

    void DepthStencilState::setBack(VkStencilOpState back) noexcept
    {
        _pipeline_depth_stencil_static_ci.back = back;
    }

    void DepthStencilState::setMinDepthBounds(float min_depth_bounds) noexcept
    {
        _pipeline_depth_stencil_static_ci.minDepthBounds = min_depth_bounds;
    }

    void DepthStencilState::setMaxDepthBounds(float max_depth_bounds) noexcept
    {
        _pipeline_depth_stencil_static_ci.maxDepthBounds = max_depth_bounds;
    }

    void DepthStencilState::depthTestEnable(VkBool32 is_enable) noexcept
    {
        _pipeline_depth_stencil_static_ci.depthTestEnable = is_enable;
    }

    void DepthStencilState::depthWriteEnable(VkBool32 is_enable) noexcept
    {
        _pipeline_depth_stencil_static_ci.depthWriteEnable = is_enable;
    }

    void DepthStencilState::depthBoundsTestEnable(VkBool32 is_enable) noexcept
    {
        _pipeline_depth_stencil_static_ci.depthBoundsTestEnable = is_enable;
    }

    void DepthStencilState::stencilTestEnable(VkBool32 is_enable) noexcept
    {
        _pipeline_depth_stencil_static_ci.stencilTestEnable = is_enable;
    }

    const VkPipelineDepthStencilStateCreateInfo& DepthStencilState::getPipelineDepthStencilStateCreateInfo() const noexcept
    {
        return _pipeline_depth_stencil_static_ci;
    }

    VkPipelineDepthStencilStateCreateInfo& DepthStencilState::getPipelineDepthStencilStateCreateInfo() noexcept
    {
        return _pipeline_depth_stencil_static_ci;
    }

    VkCompareOp DepthStencilState::getDepthCompareOp() const noexcept
    {
        return _pipeline_depth_stencil_static_ci.depthCompareOp;
    }

    VkStencilOpState DepthStencilState::getFront() const noexcept
    {
        return _pipeline_depth_stencil_static_ci.front;
    }

    VkStencilOpState DepthStencilState::getBack() const noexcept
    {
        return _pipeline_depth_stencil_static_ci.back;
    }

    float DepthStencilState::getMinDepthBounds() const noexcept
    {
        return _pipeline_depth_stencil_static_ci.minDepthBounds;
    }

    float DepthStencilState::getMaxDepthBounds() const noexcept
    {
        return _pipeline_depth_stencil_static_ci.maxDepthBounds;
    }

    VkBool32 DepthStencilState::depthTestEnable() const noexcept
    {
        return _pipeline_depth_stencil_static_ci.depthTestEnable;
    }

    VkBool32 DepthStencilState::depthWriteEnable() const noexcept
    {
        return _pipeline_depth_stencil_static_ci.depthWriteEnable;
    }

    VkBool32 DepthStencilState::depthBoundsTestEnable() const noexcept
    {
        return _pipeline_depth_stencil_static_ci.depthBoundsTestEnable;
    }

    VkBool32 DepthStencilState::stencilTestEnable() const noexcept
    {
        return _pipeline_depth_stencil_static_ci.stencilTestEnable;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ViewportState::ViewportState(size_t num_viewports, size_t num_scissors) :
        _current_viewport   (0),
        _current_scissor    (0)
    {
        assert(num_viewports > 0 && num_scissors > 0);
        _ptr_viewports = new VkViewport [num_viewports];
        _ptr_scissors  = new VkRect2D [num_scissors];

        _pipeline_viewport_state_ci = { };
        _pipeline_viewport_state_ci.sType           = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        _pipeline_viewport_state_ci.viewportCount   = static_cast<uint32_t>(num_viewports);
        _pipeline_viewport_state_ci.scissorCount    = static_cast<uint32_t>(num_scissors); 
        _pipeline_viewport_state_ci.pViewports      = _ptr_viewports;
        _pipeline_viewport_state_ci.pScissors       = _ptr_scissors;
    }

    ViewportState::ViewportState(ViewportState&& viewport_state) :
        _ptr_viewports  (nullptr),
        _ptr_scissors   (nullptr)
    {
        swap(_ptr_viewports, viewport_state._ptr_viewports);
        swap(_ptr_scissors, viewport_state._ptr_scissors);

        _current_viewport   = viewport_state._current_viewport;
        _current_scissor    = viewport_state._current_scissor;

        memcpy(&_pipeline_viewport_state_ci, &viewport_state._pipeline_viewport_state_ci, sizeof(VkPipelineViewportStateCreateInfo));
    }

    ViewportState::ViewportState(const ViewportState& viewport_state) :
        _ptr_viewports      (new VkViewport [viewport_state._pipeline_viewport_state_ci.viewportCount]),
        _ptr_scissors       (new VkRect2D   [viewport_state._pipeline_viewport_state_ci.scissorCount]),
        _current_viewport   (viewport_state._current_viewport),
        _current_scissor    (viewport_state._current_scissor)
    {
        _pipeline_viewport_state_ci = { };
        _pipeline_viewport_state_ci.sType           = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        _pipeline_viewport_state_ci.viewportCount   = viewport_state._pipeline_viewport_state_ci.viewportCount;
        _pipeline_viewport_state_ci.scissorCount    = viewport_state._pipeline_viewport_state_ci.scissorCount;
        _pipeline_viewport_state_ci.pViewports      = _ptr_viewports;
        _pipeline_viewport_state_ci.pScissors       = _ptr_scissors;

        memcpy(_ptr_viewports, viewport_state._ptr_viewports, sizeof(VkViewport) * _current_viewport);
        memcpy(_ptr_scissors, viewport_state._ptr_scissors, sizeof(VkRect2D) * _current_scissor);
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
        assert(_pipeline_viewport_state_ci.viewportCount > _current_viewport);
        assert(min_depth >= 0.0f && max_depth <= 1.0f);

        _ptr_viewports[_current_viewport] = { };
        _ptr_viewports[_current_viewport].x         = x;
        _ptr_viewports[_current_viewport].y         = y;
        _ptr_viewports[_current_viewport].width     = width;
        _ptr_viewports[_current_viewport].height    = height;
        _ptr_viewports[_current_viewport].minDepth  = min_depth;
        _ptr_viewports[_current_viewport].maxDepth  = max_depth;

        _current_viewport++;
    }

    void ViewportState::addScissor(int32_t x_offset, int32_t y_offset, uint32_t width, uint32_t height)
    {
        assert(_pipeline_viewport_state_ci.scissorCount > _current_scissor);

        _ptr_scissors[_current_scissor] = { };
        _ptr_scissors[_current_scissor].offset = {x_offset, y_offset};
        _ptr_scissors[_current_scissor].extent = {width, height};

        _current_scissor++;
    }

    const VkPipelineViewportStateCreateInfo& ViewportState::getPipelineViewportStateCreateInfo() const noexcept
    {
        return _pipeline_viewport_state_ci;
    }

    VkPipelineViewportStateCreateInfo& ViewportState::getPipelineViewportStateCreateInfo() noexcept
    {
        return _pipeline_viewport_state_ci;
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
        return _pipeline_viewport_state_ci.viewportCount;
    }

    size_t ViewportState::capacityScissors() const noexcept
    {
        return _pipeline_viewport_state_ci.scissorCount;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    InputAssemblyState::InputAssemblyState(VkPrimitiveTopology topology)
    {
        _pipeline_input_assembly_state_ci = { };
        _pipeline_input_assembly_state_ci.sType     = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        _pipeline_input_assembly_state_ci.topology  = topology;
    }

    void InputAssemblyState::setTopology(VkPrimitiveTopology topology) noexcept
    {
        _pipeline_input_assembly_state_ci.topology = topology;
    }

    void InputAssemblyState::primitiveRestartEnable(VkBool32 is_enable) noexcept
    {
        _pipeline_input_assembly_state_ci.primitiveRestartEnable = is_enable;
    }

    const VkPipelineInputAssemblyStateCreateInfo& InputAssemblyState::getPipelineInputAssemblyStateCreateInfo() const noexcept
    {
        return _pipeline_input_assembly_state_ci;
    }

    VkPipelineInputAssemblyStateCreateInfo& InputAssemblyState::getPipelineInputAssemblyStateCreateInfo() noexcept
    {
        return _pipeline_input_assembly_state_ci;
    }

    VkPrimitiveTopology InputAssemblyState::getTopology() const noexcept
    {
        return _pipeline_input_assembly_state_ci.topology;
    }

    VkBool32 InputAssemblyState::primitiveRestartEnable() const noexcept
    {
        return _pipeline_input_assembly_state_ci.primitiveRestartEnable;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ColorBlendState::ColorBlendState(size_t num_attachments) noexcept
    {
        _current_attachment = 0;

        _pipeline_color_blend_state_ci = { };
        _pipeline_color_blend_state_ci.sType            = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        _pipeline_color_blend_state_ci.attachmentCount  = static_cast<uint32_t>(num_attachments);

        _ptr_attachments                            = new VkPipelineColorBlendAttachmentState[num_attachments];
        _pipeline_color_blend_state_ci.pAttachments = _ptr_attachments;
    }

    ColorBlendState::ColorBlendState(ColorBlendState&& color_blend_state) :
        _ptr_attachments    (nullptr),
        _current_attachment (color_blend_state._current_attachment)
    {
        swap(_ptr_attachments, color_blend_state._ptr_attachments);
        memcpy(&_pipeline_color_blend_state_ci,  &color_blend_state._pipeline_color_blend_state_ci, sizeof(VkPipelineColorBlendStateCreateInfo));
    }


    ColorBlendState::ColorBlendState(const ColorBlendState& color_blend_state) :
        _ptr_attachments    (new VkPipelineColorBlendAttachmentState[color_blend_state._pipeline_color_blend_state_ci.attachmentCount]),
        _current_attachment (color_blend_state._current_attachment)
    {
        memcpy(_ptr_attachments, color_blend_state._ptr_attachments, sizeof(VkPipelineColorBlendAttachmentState) * _current_attachment);
        memcpy(&_pipeline_color_blend_state_ci, &color_blend_state._pipeline_color_blend_state_ci, sizeof(VkPipelineColorBlendStateCreateInfo));
        _pipeline_color_blend_state_ci.pAttachments = _ptr_attachments;
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
        assert(_pipeline_color_blend_state_ci.attachmentCount > _current_attachment);

        _ptr_attachments[_current_attachment] = { };
        _ptr_attachments[_current_attachment].blendEnable           = blend_enable;
        _ptr_attachments[_current_attachment].srcColorBlendFactor   = src_color_blend_factor;
        _ptr_attachments[_current_attachment].dstColorBlendFactor   = dst_color_blend_factor;
        _ptr_attachments[_current_attachment].colorBlendOp          = color_blend_op;
        _ptr_attachments[_current_attachment].srcAlphaBlendFactor   = src_alpha_blend_factor;
        _ptr_attachments[_current_attachment].dstAlphaBlendFactor   = dst_alpha_blend_factor;
        _ptr_attachments[_current_attachment].alphaBlendOp          = alpha_blend_op;
        _ptr_attachments[_current_attachment].colorWriteMask        = color_write_mask;

        _current_attachment++;
    }

    void ColorBlendState::logicOpEnable(VkBool32 is_enable) noexcept
    {
        _pipeline_color_blend_state_ci.logicOpEnable = is_enable;
    }

    void ColorBlendState::setLogicOp(VkLogicOp logic_op) noexcept
    {
        _pipeline_color_blend_state_ci.logicOp = logic_op;
    }

    const VkPipelineColorBlendStateCreateInfo& ColorBlendState::getPipelineColorBlendStateCreateInfo() const noexcept
    {
        return _pipeline_color_blend_state_ci;
    }

    VkPipelineColorBlendStateCreateInfo& ColorBlendState::getPipelineColorBlendStateCreateInfo() noexcept
    {
        return _pipeline_color_blend_state_ci;
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
        return _pipeline_color_blend_state_ci.attachmentCount;
    }

    bool ColorBlendState::logicOpEnable() const noexcept
    {
        return _pipeline_color_blend_state_ci.logicOpEnable;
    }

    VkLogicOp ColorBlendState::getLogicOp() const noexcept
    {
        return _pipeline_color_blend_state_ci.logicOp;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    GraphicsPipelineStates::GraphicsPipelineStates(
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

    GraphicsPipelineStates::GraphicsPipelineStates(GraphicsPipelineStates&& graphics_pipeline_states) :
        _vertex_input_state     (move(graphics_pipeline_states._vertex_input_state)),
        _multisample_state      (move(graphics_pipeline_states._multisample_state)),
        _rasterization_state    (move(graphics_pipeline_states._rasterization_state)),
        _depth_stencil_state    (move(graphics_pipeline_states._depth_stencil_state)),
        _viewport_state         (move(graphics_pipeline_states._viewport_state)),
        _input_assembly_state   (move(graphics_pipeline_states._input_assembly_state)),
        _color_blend_state      (move(graphics_pipeline_states._color_blend_state))
    {}

    GraphicsPipelineStates::GraphicsPipelineStates(const GraphicsPipelineStates& graphics_pipeline_states) :
        _vertex_input_state     (graphics_pipeline_states._vertex_input_state),
        _multisample_state      (graphics_pipeline_states._multisample_state),
        _rasterization_state    (graphics_pipeline_states._rasterization_state),
        _depth_stencil_state    (graphics_pipeline_states._depth_stencil_state),
        _viewport_state         (graphics_pipeline_states._viewport_state),
        _input_assembly_state   (graphics_pipeline_states._input_assembly_state),
        _color_blend_state      (graphics_pipeline_states._color_blend_state)
    {}

    VertexInputState& GraphicsPipelineStates::getVertexInputState() noexcept
    {
        return _vertex_input_state;
    }
    
    const VertexInputState& GraphicsPipelineStates::getVertexInputState() const noexcept
    {
        return _vertex_input_state;
    }

    MultisampleState& GraphicsPipelineStates::getMultisampleState() noexcept
    {
        return _multisample_state;
    }

    const MultisampleState& GraphicsPipelineStates::getMultisampleState() const noexcept
    {
        return _multisample_state;
    }

    RasterizationState& GraphicsPipelineStates::getRasterizationState() noexcept
    {
        return _rasterization_state;
    }

    const RasterizationState& GraphicsPipelineStates::getRasterizationState() const noexcept
    {
        return _rasterization_state;
    }

    DepthStencilState& GraphicsPipelineStates::getDepthStencilState() noexcept
    {
        return _depth_stencil_state;
    }

    const DepthStencilState& GraphicsPipelineStates::getDepthStencilState() const noexcept
    {
        return _depth_stencil_state;
    }

    ViewportState& GraphicsPipelineStates::getViewportState() noexcept
    {
        return _viewport_state;
    }

    const ViewportState& GraphicsPipelineStates::getViewportState() const noexcept
    {
        return _viewport_state;
    }

    InputAssemblyState& GraphicsPipelineStates::getInputAssemblyState() noexcept
    {
        return _input_assembly_state;
    }

    const InputAssemblyState& GraphicsPipelineStates::getInputAssemblyState() const noexcept
    {
        return _input_assembly_state;
    }

    ColorBlendState& GraphicsPipelineStates::getColorBlendState() noexcept
    {
        return _color_blend_state;
    }

    const ColorBlendState& GraphicsPipelineStates::getColorBlendState() const noexcept
    {
        return _color_blend_state;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    GraphicsPipeline::GraphicsPipeline(
        const GraphicsPipelineStates&           graphics_pipeline_states,
        const vector<ShaderModule>&             shaders,
        const PtrPipelineLayout&                ptr_pipeline_layout,
        const PtrRenderPass&                    ptr_render_pass,
        uint32_t                                subpass_index
    ) :
        _subpass_index          (subpass_index),
        _ptr_pipeline_layout    (ptr_pipeline_layout),
        _ptr_render_pass        (ptr_render_pass),
        _states                 (graphics_pipeline_states),
        _pipeline_handle        (VK_NULL_HANDLE),
        _pipeline_cache_handle  (VK_NULL_HANDLE)
    {
        _create(shaders);
    }
    
    GraphicsPipeline::GraphicsPipeline(
        GraphicsPipelineStates&&                graphics_pipeline_states,
        const vector<ShaderModule>&             shaders,
        const PtrPipelineLayout&                ptr_pipeline_layout,
        const PtrRenderPass&                    ptr_render_pass,
        uint32_t                                subpass_index
    ) :
        _subpass_index          (subpass_index),
        _ptr_pipeline_layout    (ptr_pipeline_layout),
        _ptr_render_pass        (ptr_render_pass),
        _states                 (move(graphics_pipeline_states)),
        _pipeline_handle        (VK_NULL_HANDLE),
        _pipeline_cache_handle  (VK_NULL_HANDLE)
    {
        _create(shaders);
    }

    GraphicsPipeline::GraphicsPipeline(GraphicsPipeline&& graphics_pipeline) :
        _subpass_index          (graphics_pipeline._subpass_index),
        _ptr_pipeline_layout    (move(graphics_pipeline._ptr_pipeline_layout)),
        _ptr_render_pass        (move(graphics_pipeline._ptr_render_pass)),
        _states                 (move(graphics_pipeline._states)),
        _pipeline_handle        (VK_NULL_HANDLE),
        _pipeline_cache_handle  (VK_NULL_HANDLE)
    {
        swap(_pipeline_handle, graphics_pipeline._pipeline_handle);
        swap(_pipeline_cache_handle, graphics_pipeline._pipeline_cache_handle);
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

        VkPipelineCacheCreateInfo pipeline_cache_info = { };
        pipeline_cache_info.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;

        assert(vkCreatePipelineCache(
            ptr_device->getDeviceHandle(),
            &pipeline_cache_info,
            nullptr,
            &_pipeline_cache_handle
        ) == VK_SUCCESS);

        assert(_pipeline_cache_handle != VK_NULL_HANDLE);

       vector<VkPipelineShaderStageCreateInfo> pipeline_shader_stages_info(shaders.size());

       for (size_t i{0}; i < pipeline_shader_stages_info.size(); i++) {
            pipeline_shader_stages_info[i] = { };
            pipeline_shader_stages_info[i].sType                = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            pipeline_shader_stages_info[i].stage                = shaders[i].getShaderType();
            pipeline_shader_stages_info[i].module               = shaders[i].getShaderHandle();
            pipeline_shader_stages_info[i].pName                = "main";
            pipeline_shader_stages_info[i].pSpecializationInfo  = &shaders[i].getSpecializationInfo().getSpecializationInfo();
       };

        VkGraphicsPipelineCreateInfo graphics_pipeline_info = { };
        graphics_pipeline_info.sType                = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        graphics_pipeline_info.stageCount           = static_cast<uint32_t>(pipeline_shader_stages_info.size());
        graphics_pipeline_info.pStages              = pipeline_shader_stages_info.data();
        graphics_pipeline_info.pVertexInputState    = &_states.getVertexInputState().getPipelineVertexInputStateCreateInfo();
        graphics_pipeline_info.pInputAssemblyState  = reinterpret_cast<VkPipelineInputAssemblyStateCreateInfo*> (&_states.getInputAssemblyState());
        graphics_pipeline_info.pTessellationState   = nullptr;
        graphics_pipeline_info.pViewportState       = &_states.getViewportState().getPipelineViewportStateCreateInfo();
        graphics_pipeline_info.pRasterizationState  = &_states.getRasterizationState().getPipelineRasterizationStateCreateInfo();
        graphics_pipeline_info.pMultisampleState    = &_states.getMultisampleState().getPipelineMultisampleStateCreateInfo();
        graphics_pipeline_info.pDepthStencilState   = &_states.getDepthStencilState().getPipelineDepthStencilStateCreateInfo();
        graphics_pipeline_info.pColorBlendState     = &_states.getColorBlendState().getPipelineColorBlendStateCreateInfo();
        graphics_pipeline_info.pDynamicState        = nullptr;
        graphics_pipeline_info.layout               = _ptr_pipeline_layout->getPipelineLayoutHandle();
        graphics_pipeline_info.renderPass           = _ptr_render_pass->getRenderPassHandle();
        graphics_pipeline_info.subpass              = _subpass_index;

        assert(vkCreateGraphicsPipelines(
            ptr_device->getDeviceHandle(),
            _pipeline_cache_handle,
            1,
            &graphics_pipeline_info,
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

    GraphicsPipelineStates& GraphicsPipeline::getGraphicsPipelineStates() noexcept
    {
        return _states;
    }

    const GraphicsPipelineStates& GraphicsPipeline::getGraphicsPipelineStates() const noexcept
    {
        return _states;
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
        const GraphicsPipelineStates&           graphics_pipeline_states,
        const vector<ShaderModule>&             shaders,
        const PtrPipelineLayout&                ptr_pipeline_layout,
        const PtrRenderPass&                    ptr_render_pass,
        uint32_t                                subpass_index
    )
    {
        return make_shared<GraphicsPipeline>(
            graphics_pipeline_states,
            shaders,
            ptr_pipeline_layout,
            ptr_render_pass,
            subpass_index
        );
    }

    PtrGraphicsPipeline GraphicsPipeline::make(
        GraphicsPipelineStates&&                graphics_pipeline_states,
        const vector<ShaderModule>&             shaders,
        const PtrPipelineLayout&                ptr_pipeline_layout,
        const PtrRenderPass&                    ptr_render_pass,
        uint32_t                                subpass_index
    )
    {
        return make_shared<GraphicsPipeline>(
            move(graphics_pipeline_states),
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
        GraphicsPipelineStates(
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
