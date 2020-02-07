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

        inline ~ShaderModule();

        inline shared_ptr<Device>& getDevice() noexcept;
        inline const shared_ptr<Device>& getDevice() const noexcept;
        inline VkShaderStageFlagBits getShaderType() const noexcept;
        inline VkShaderModule getShaderHandle() const noexcept;

    private:
        shared_ptr<Device> _ptr_device;
        VkShaderStageFlagBits _shader_type;
        VkShaderModule _shader_handle;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inline ShaderModule::~ShaderModule()
    {
        vkDestroyShaderModule(_ptr_device->getDeviceHandle(), _shader_handle, nullptr);
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

    inline VkShaderModule ShaderModule::getShaderHandle() const noexcept
    {
        return _shader_handle;
    }
}

#endif /* ShaderModule_hpp */
