//
//  ComputePipeline.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 16/07/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include "ComputePipeline.hpp"

#include "PipelineLayout.hpp"

#include "ShaderModule.hpp"

#include <cassert>
#include <stdexcept>

namespace pbrlib
{
    ComputePipeline::ComputePipeline(
        const ShaderModule&         shader_module,
        const PtrPipelineLayout&    ptr_pipeline_layout
    ) :
        _pipeline_handle        (VK_NULL_HANDLE),
        _ptr_pipeline_layout    (ptr_pipeline_layout)
    {
        assert(shader_module.getShaderType() == VK_SHADER_STAGE_COMPUTE_BIT);

        VkComputePipelineCreateInfo compute_pipeline_info = { };
        compute_pipeline_info.sType                     = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
        compute_pipeline_info.stage.sType               = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        compute_pipeline_info.stage.stage               = VK_SHADER_STAGE_COMPUTE_BIT;
        compute_pipeline_info.stage.module              = shader_module.getShaderHandle();
        compute_pipeline_info.stage.pName               = "main";
        compute_pipeline_info.stage.pSpecializationInfo = &shader_module.getSpecializationInfo().getSpecializationInfo();
        compute_pipeline_info.layout                    = _ptr_pipeline_layout->getPipelineLayoutHandle();

        assert(vkCreateComputePipelines(
            _ptr_pipeline_layout->getDevice()->getDeviceHandle(), 
            VK_NULL_HANDLE, 
            1, 
            &compute_pipeline_info, 
            nullptr, 
            &_pipeline_handle
        ) == VK_SUCCESS);

        assert(_pipeline_handle != VK_NULL_HANDLE);
    }

    ComputePipeline::ComputePipeline(
        const PtrShaderModule&      ptr_shader_module,
        const PtrPipelineLayout&    ptr_pipeline_layout
    )
    {
        if (!ptr_shader_module) {
            throw invalid_argument("ptr_shader_module is null");
        } 

        ComputePipeline(*ptr_shader_module, ptr_pipeline_layout);
    }

    ComputePipeline::ComputePipeline(ComputePipeline&& pipeline) :
        _pipeline_handle(VK_NULL_HANDLE)
    {
        swap(_pipeline_handle, pipeline._pipeline_handle);
        swap(_ptr_pipeline_layout, pipeline._ptr_pipeline_layout);
    }

    ComputePipeline::~ComputePipeline()
    {
        if (_pipeline_handle != VK_NULL_HANDLE) {
            vkDestroyPipeline(_ptr_pipeline_layout->getDevice()->getDeviceHandle(), _pipeline_handle, nullptr);
        }
    }

    VkPipeline ComputePipeline::getPipelineHandle() const noexcept
    {
        return _pipeline_handle;
    }

    PtrPipelineLayout& ComputePipeline::getPipelineLayout() noexcept
    {
        return _ptr_pipeline_layout;
    }

    const PtrPipelineLayout& ComputePipeline::getPipelineLayout() const noexcept
    {
        return _ptr_pipeline_layout;
    }

    PtrComputePipeline ComputePipeline::make(
        const ShaderModule&         shader_module,
        const PtrPipelineLayout&    ptr_pipeline_layout
    )
    {
        return make_shared<ComputePipeline>(shader_module, ptr_pipeline_layout);
    }
}
