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
    SpecializationInfo::SpecializationInfo(
        size_t size_data,
        size_t num_map_entires
    ) :
        VkSpecializationInfo {
            .mapEntryCount = static_cast<uint32_t>(num_map_entires),
            .dataSize = size_data
        },
        _ptr_data               (new uint8_t                    [size_data]),
        _ptr_map_enties         (new VkSpecializationMapEntry   [num_map_entires]),
        _current_byte_in_data   (0),
        _current_map_entry      (0)
    {
        VkSpecializationInfo::pData         = static_cast<void*>(_ptr_data);
        VkSpecializationInfo::pMapEntries   = _ptr_map_enties;
    }

    SpecializationInfo::SpecializationInfo(SpecializationInfo&& specialization_info) :
        _ptr_data       (nullptr),
        _ptr_map_enties (nullptr)
    {
        swap(_ptr_data,         specialization_info._ptr_data);
        swap(_ptr_map_enties,   specialization_info._ptr_map_enties);

        memcpy(this, &specialization_info, sizeof(VkSpecializationInfo));
    }

    SpecializationInfo::SpecializationInfo(const SpecializationInfo& specialization_info) : 
        VkSpecializationInfo {
            .mapEntryCount = static_cast<uint32_t>(specialization_info.mapEntryCount),
            .dataSize = specialization_info.dataSize
        },
        _ptr_data               (new uint8_t                    [specialization_info.dataSize]),
        _ptr_map_enties         (new VkSpecializationMapEntry   [specialization_info.mapEntryCount]),
        _current_byte_in_data   (specialization_info._current_byte_in_data),
        _current_map_entry      (specialization_info._current_map_entry)
    {
        VkSpecializationInfo::pData         = static_cast<void*>(_ptr_data);
        VkSpecializationInfo::pMapEntries   = _ptr_map_enties;
    }

    SpecializationInfo::~SpecializationInfo()
    {
        if (_ptr_data) {
            delete[] _ptr_data;
        }

        if (_ptr_map_enties) {
            delete[] _ptr_map_enties;
        }
    }

    void SpecializationInfo::addMapEntry(const void* ptr_data, size_t data_size, uint32_t constant_id)
    {
        assert(VkSpecializationInfo::dataSize >= _current_byte_in_data + data_size &&
               VkSpecializationInfo::mapEntryCount > _current_map_entry);

        _ptr_map_enties[_current_map_entry] = {
            .constantID = constant_id,
            .offset     = static_cast<uint32_t>(_current_byte_in_data),
            .size       = data_size
        };

        memcpy(_ptr_data + _current_byte_in_data, ptr_data, data_size);

        _current_byte_in_data += data_size;
        _current_map_entry++;
    }

    uint8_t* SpecializationInfo::getData() noexcept
    {
        return _ptr_data;
    }

    const uint8_t* SpecializationInfo::getData() const noexcept
    {
        return _ptr_data;
    }

    VkSpecializationMapEntry* SpecializationInfo::getSpecializationMapEntries() noexcept
    {
        return _ptr_map_enties;
    }

    const VkSpecializationMapEntry* SpecializationInfo::getSpecializationMapEntries() const noexcept
    {
        return _ptr_map_enties;
    }

    size_t SpecializationInfo::getDataSize() const noexcept
    {
        return _current_byte_in_data;
    }

    size_t SpecializationInfo::getSpecializationMapEntriesNum() const noexcept
    {
        return _current_map_entry;
    }

    size_t SpecializationInfo::capacityData() const noexcept
    {
        return VkSpecializationInfo::dataSize;
    }

    size_t SpecializationInfo::capacitySpecializationMapEntries() const noexcept
    {
        return VkSpecializationInfo::mapEntryCount;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ShaderModule::ShaderModule(
        const PtrDevice&            ptr_device,
        VkShaderStageFlagBits       shader_type,
        const uint32_t*             ptr_shader_code,
        size_t                      shader_code_size,
        size_t                      specialization_info_size_data,
        size_t                      specialization_info_num_map_entires
    ) :
        _ptr_device             (ptr_device),
        _shader_type            (shader_type),
        _specialization_info    (specialization_info_size_data, specialization_info_num_map_entires),
        _shader_handle          (VK_NULL_HANDLE)
    {
        VkShaderModuleCreateInfo shader_info {
            .sType      = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            .pNext      = nullptr,
            .flags      = 0,
            .codeSize   = shader_code_size,
            .pCode      = ptr_shader_code
        };

        assert(vkCreateShaderModule(
            _ptr_device->getDeviceHandle(), 
            &shader_info, 
            nullptr, 
            &_shader_handle
        ) == VK_SUCCESS);

        assert(_shader_handle != VK_NULL_HANDLE);
    }

    ShaderModule::ShaderModule(ShaderModule&& shader_module) :
        _ptr_device             (move(shader_module._ptr_device)),
        _shader_type            (shader_module._shader_type),
        _specialization_info    (shader_module._specialization_info),
        _shader_handle          (VK_NULL_HANDLE)
    {
        swap(_shader_handle, shader_module._shader_handle);
    }

    ShaderModule::~ShaderModule()
    {
        if (_shader_handle != VK_NULL_HANDLE) {
            vkDestroyShaderModule(_ptr_device->getDeviceHandle(), _shader_handle, nullptr);
        }
    }

    PtrDevice& ShaderModule::getDevice() noexcept
    {
        return _ptr_device;
    }

    const PtrDevice& ShaderModule::getDevice() const noexcept
    {
        return _ptr_device;
    }

    VkShaderStageFlagBits ShaderModule::getShaderType() const noexcept
    {
        return _shader_type;
    }

    const VkShaderModule& ShaderModule::getShaderHandle() const noexcept
    {
        return _shader_handle;
    }

    SpecializationInfo& ShaderModule::getSpecializationInfo() noexcept
    {
        return _specialization_info;
    }

    const SpecializationInfo& ShaderModule::getSpecializationInfo() const noexcept
    {
        return _specialization_info;
    }

    PtrShaderModule ShaderModule::make(
        const PtrDevice&            ptr_device,
        VkShaderStageFlagBits       shader_type,
        const uint32_t*             ptr_shader_code,
        size_t                      shader_code_size,
        size_t                      specialization_info_size_data,
        size_t                      specialization_info_num_map_entires
    )
    {
        return make_shared<ShaderModule>(
            ptr_device,
            shader_type,
            ptr_shader_code,
            shader_code_size,
            specialization_info_size_data,
            specialization_info_num_map_entires
        );
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ShaderModule::Builder::Builder() :
        _specialization_info_size_data      (0),
        _specialization_info_num_map_entires(0)
    {}

    void ShaderModule::Builder::setDevice(const PtrDevice& ptr_device)
    {
        _ptr_device = ptr_device;
    }

    void ShaderModule::Builder::setShaderType(VkShaderStageFlagBits shader_type) noexcept
    {
        _shader_type = shader_type;
    }

    void ShaderModule::Builder::setShaderCode(
        const uint32_t* ptr_shader_code, 
        size_t          shader_code_size
    ) noexcept
    {
        _ptr_shader_code    = ptr_shader_code;
        _shader_code_size   = shader_code_size;
    }

    void ShaderModule::Builder::setSpecializationInfoSizeData(
        size_t specialization_info_size_data
    ) noexcept
    {
        _specialization_info_size_data = specialization_info_size_data;
    }

    void ShaderModule::Builder::setSpecializationInfoNumMapEntries(
        size_t specialization_info_num_map_entires
    ) noexcept
    {
        _specialization_info_num_map_entires = specialization_info_num_map_entires;
    }

    ShaderModule ShaderModule::Builder::build() const
    {
        return ShaderModule(
            _ptr_device,
            _shader_type,
            _ptr_shader_code,
            _shader_code_size,
            _specialization_info_size_data,
            _specialization_info_num_map_entires
        );
    }

    PtrShaderModule ShaderModule::Builder::buildPtr() const
    {
        return make_shared<ShaderModule>(
            _ptr_device,
            _shader_type,
            _ptr_shader_code,
            _shader_code_size,
            _specialization_info_size_data,
            _specialization_info_num_map_entires
        );
    }
}
