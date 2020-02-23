//
//  ShaderModule.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 07/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef ShaderModule_hpp
#define ShaderModule_hpp

#include "Device.hpp"

namespace pbrlib
{
    class ShaderModule
    {
    public:
        ShaderModule(
            const shared_ptr<Device>& ptr_device,
            VkShaderStageFlagBits shader_type,
            const uint32_t* ptr_shader_code,
            size_t shader_code_size
        );

        inline ShaderModule(ShaderModule&& shader_module);
        ShaderModule(const ShaderModule&) = delete;
        
        inline ~ShaderModule();

        ShaderModule& operator = (ShaderModule&&) = delete;
        ShaderModule& operator = (const ShaderModule&) = delete;

        inline shared_ptr<Device>& getDevice() noexcept;
        inline const shared_ptr<Device>& getDevice() const noexcept;
        inline VkShaderStageFlagBits getShaderType() const noexcept;
        inline const VkShaderModule& getShaderHandle() const noexcept;

    private:
        shared_ptr<Device> _ptr_device;
        VkShaderStageFlagBits _shader_type;
        VkShaderModule _shader_handle;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inline ShaderModule::ShaderModule(ShaderModule&& shader_module) :
        _ptr_device(shader_module._ptr_device),
        _shader_type(shader_module._shader_type),
        _shader_handle(VK_NULL_HANDLE)
    {
        swap(_shader_handle, shader_module._shader_handle);
    }

    inline ShaderModule::~ShaderModule()
    {
        if (_shader_handle != VK_NULL_HANDLE) {
            vkDestroyShaderModule(_ptr_device->getDeviceHandle(), _shader_handle, nullptr);
        }
    }

    inline shared_ptr<Device>& ShaderModule::getDevice() noexcept
    {
        return _ptr_device;
    }

    inline const shared_ptr<Device>& ShaderModule::getDevice() const noexcept
    {
        return _ptr_device;
    }

    inline VkShaderStageFlagBits ShaderModule::getShaderType() const noexcept
    {
        return _shader_type;
    }

    inline const VkShaderModule& ShaderModule::getShaderHandle() const noexcept
    {
        return _shader_handle;
    }
}

#endif /* ShaderModule_hpp */
