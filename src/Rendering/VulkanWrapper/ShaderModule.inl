//
//  ShaderModule.inl
//  PBRLib
//
//  Created by Асиф Мамедов on 07/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

namespace pbrlib
{
    inline SpecializationInfo::SpecializationInfo(size_t size_data,
                                                  size_t num_map_entires) :
        VkSpecializationInfo {
            .mapEntryCount = static_cast<uint32_t>(num_map_entires),
            .dataSize = size_data
        },
        _ptr_data(new uint8_t [size_data]),
        _ptr_map_enties(new VkSpecializationMapEntry[num_map_entires]),
        _current_byte_in_data(0),
        _current_map_entry(0)
    {
        VkSpecializationInfo::pData = static_cast<void*>(_ptr_data);
        VkSpecializationInfo::pMapEntries = _ptr_map_enties;
    }

    inline SpecializationInfo::SpecializationInfo(SpecializationInfo&& specialization_info) :
        _ptr_data(nullptr),
        _ptr_map_enties(nullptr)
    {
        swap(_ptr_data, specialization_info._ptr_data);
        swap(_ptr_map_enties, specialization_info._ptr_map_enties);
        memcpy(this, &specialization_info, sizeof(VkSpecializationInfo));
    }

    inline SpecializationInfo::SpecializationInfo(const SpecializationInfo& specialization_info) : 
        VkSpecializationInfo {
            .mapEntryCount = static_cast<uint32_t>(specialization_info.mapEntryCount),
            .dataSize = specialization_info.dataSize
        },
        _ptr_data(new uint8_t [specialization_info.dataSize]),
        _ptr_map_enties(new VkSpecializationMapEntry[specialization_info.mapEntryCount]),
        _current_byte_in_data(specialization_info._current_byte_in_data),
        _current_map_entry(specialization_info._current_map_entry)
    {
        VkSpecializationInfo::pData = static_cast<void*>(_ptr_data);
        VkSpecializationInfo::pMapEntries = _ptr_map_enties;
    }

    inline SpecializationInfo::~SpecializationInfo()
    {
        if (_ptr_data) {
            delete[] _ptr_data;
        }

        if (_ptr_map_enties) {
            delete[] _ptr_map_enties;
        }
    }

    inline void SpecializationInfo::addMapEntry(const uint8_t* ptr_data, size_t data_size, uint32_t constant_id)
    {
        assert(VkSpecializationInfo::dataSize > _current_byte_in_data + data_size &&
               VkSpecializationInfo::mapEntryCount > _current_map_entry);

        _ptr_map_enties[_current_map_entry] = {
            .constantID = constant_id,
            .offset = static_cast<uint32_t>(_current_byte_in_data),
            .size = data_size
        };

        memcpy(_ptr_data + _current_byte_in_data, ptr_data, data_size);

        _current_byte_in_data += data_size;
        _current_map_entry++;
    }

    inline uint8_t* SpecializationInfo::getData() noexcept
    {
        return _ptr_data;
    }

    inline const uint8_t* SpecializationInfo::getData() const noexcept
    {
        return _ptr_data;
    }

    inline VkSpecializationMapEntry* SpecializationInfo::getSpecializationMapEntries() noexcept
    {
        return _ptr_map_enties;
    }

    inline const VkSpecializationMapEntry* SpecializationInfo::getSpecializationMapEntries() const noexcept
    {
        return _ptr_map_enties;
    }

    inline size_t SpecializationInfo::getDataSize() const noexcept
    {
        return _current_byte_in_data;
    }

    inline size_t SpecializationInfo::getSpecializationMapEntriesNum() const noexcept
    {
        return _current_map_entry;
    }

    inline size_t SpecializationInfo::capacityData() const noexcept
    {
        return VkSpecializationInfo::dataSize;
    }

    inline size_t SpecializationInfo::capacitySpecializationMapEntries() const noexcept
    {
        return VkSpecializationInfo::mapEntryCount;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inline ShaderModule::ShaderModule(
        const shared_ptr<Device>& ptr_device,
        VkShaderStageFlagBits shader_type,
        const uint32_t* ptr_shader_code,
        size_t shader_code_size,
        size_t specialization_info_size_data,
        size_t specialization_info_num_map_entires
    ) :
        _ptr_device(ptr_device),
        _shader_type(shader_type),
        _shader_handle(VK_NULL_HANDLE),
        _specialization_info(specialization_info_size_data, specialization_info_num_map_entires)
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

    inline ShaderModule::ShaderModule(ShaderModule&& shader_module) :
        _ptr_device(shader_module._ptr_device),
        _shader_type(shader_module._shader_type),
        _shader_handle(VK_NULL_HANDLE),
        _specialization_info(move(shader_module._specialization_info))
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

    inline SpecializationInfo& ShaderModule::getSpecializationInfo() noexcept
    {
        return _specialization_info;
    }

    inline const SpecializationInfo& ShaderModule::getSpecializationInfo() const noexcept
    {
        return _specialization_info;
    }

    inline shared_ptr<ShaderModule> ShaderModule::make(const shared_ptr<Device>& ptr_device,
                                                       VkShaderStageFlagBits shader_type,
                                                       const uint32_t* ptr_shader_code,
                                                       size_t shader_code_size,
                                                       size_t specialization_info_size_data,
                                                       size_t specialization_info_num_map_entires)
    {
        return make_shared<ShaderModule>(ptr_device,
                                         shader_type,
                                         ptr_shader_code,
                                         shader_code_size,
                                         specialization_info_size_data,
                                         specialization_info_num_map_entires);
    }
}
