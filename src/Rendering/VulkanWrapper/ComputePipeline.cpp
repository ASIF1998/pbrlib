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

namespace pbrlib
{
    ComputePipeline::ComputePipeline(
        const ShaderModule&         shader_module,
        const PtrPipelineLayout&    ptr_pipeline_layout
    ) :
        _pipeline_handle        (VK_NULL_HANDLE),
        _ptr_pipeline_layout    (ptr_pipeline_layout)
    {
        /**
         * TODO: const VkSpecializationInfo*.
        */

        assert(shader_module.getShaderType() != VK_SHADER_STAGE_COMPUTE_BIT);

        VkComputePipelineCreateInfo compute_pipeline_info {
            .sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,
            .stage = {
                .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                .stage  = VK_SHADER_STAGE_COMPUTE_BIT,
                .module = shader_module.getShaderHandle(),
                .pName  = "main"
            },
            .layout     = _ptr_pipeline_layout->getPipelineLayoutHandle()
        };

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
    ) :
        _pipeline_handle        (VK_NULL_HANDLE),
        _ptr_pipeline_layout    (ptr_pipeline_layout)
    {
        /**
         * TODO: const VkSpecializationInfo*.
        */

        assert(ptr_shader_module->getShaderType() != VK_SHADER_STAGE_COMPUTE_BIT);

        VkComputePipelineCreateInfo compute_pipeline_info {
            .sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,
            .stage = {
                .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                .stage  = VK_SHADER_STAGE_COMPUTE_BIT,
                .module = ptr_shader_module->getShaderHandle(),
                .pName  = "main"
            },
            .layout = _ptr_pipeline_layout->getPipelineLayoutHandle()
        };

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

    ComputePipeline::~ComputePipeline()
    {
        vkDestroyPipeline(_ptr_pipeline_layout->getDevice()->getDeviceHandle(), _pipeline_handle, nullptr);
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
}