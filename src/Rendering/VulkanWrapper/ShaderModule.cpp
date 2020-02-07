//
//  ShaderModule.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 07/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include "ShaderModule.hpp"

namespace pbrlib
{
    ShaderModule::ShaderModule(
        const shared_ptr<Device>& ptr_device,
        VkShaderStageFlagBits shader_type,
        const uint32_t* ptr_shader_code,
        size_t shader_code_size
    ) :
        _ptr_device(ptr_device),
        _shader_type(shader_type),
        _shader_handle(VK_NULL_HANDLE)
    {
        VkShaderModuleCreateInfo shader_info {
            .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .codeSize = shader_code_size,
            .pCode = ptr_shader_code
        };

        assert(vkCreateShaderModule(_ptr_device->getDeviceHandle(), &shader_info, nullptr, &_shader_handle) == VK_SUCCESS);
        assert(_shader_handle != VK_NULL_HANDLE);
    }
}