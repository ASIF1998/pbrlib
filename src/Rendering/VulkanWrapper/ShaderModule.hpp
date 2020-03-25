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
    class ShaderModule;

    using PtrShaderModule = shared_ptr<ShaderModule>;

    class SpecializationInfo :
        private VkSpecializationInfo
    {
    public:
        inline SpecializationInfo(
            size_t size_data,
            size_t num_map_entires
        );

        inline SpecializationInfo(SpecializationInfo&& specialization_info);
        inline SpecializationInfo(const SpecializationInfo& specialization_info);

        inline ~SpecializationInfo();

        SpecializationInfo& operator = (SpecializationInfo&&)       = delete;
        SpecializationInfo& operator = (const SpecializationInfo&)  = delete;
        
        inline void addMapEntry(const uint8_t* ptr_data, size_t data_size, uint32_t constant_id);

        inline uint8_t*                         getData()                           noexcept;
        inline const uint8_t*                   getData()                           const noexcept;
        inline VkSpecializationMapEntry*        getSpecializationMapEntries()       noexcept;
        inline const VkSpecializationMapEntry*  getSpecializationMapEntries()       const noexcept;
        inline size_t                           getDataSize()                       const noexcept;
        inline size_t                           getSpecializationMapEntriesNum()    const noexcept;
        inline size_t                           capacityData()                      const noexcept;
        inline size_t                           capacitySpecializationMapEntries()  const noexcept;

    private:
        uint8_t*                    _ptr_data;
        VkSpecializationMapEntry*   _ptr_map_enties;
        size_t                      _current_byte_in_data;
        size_t                      _current_map_entry;
    };

    class ShaderModule
    {
    public:
        inline ShaderModule(
            const PtrDevice&            ptr_device,
            VkShaderStageFlagBits       shader_type,
            const uint32_t*             ptr_shader_code,
            size_t                      shader_code_size,
            size_t                      specialization_info_size_data       = 0,
            size_t                      specialization_info_num_map_entires = 0
        );

        inline ShaderModule(ShaderModule&& shader_module);
        ShaderModule(const ShaderModule&) = delete;
        
        inline ~ShaderModule();

        ShaderModule& operator = (ShaderModule&&)       = delete;
        ShaderModule& operator = (const ShaderModule&)  = delete;

        inline PtrDevice&                   getDevice()             noexcept;
        inline const PtrDevice&             getDevice()             const noexcept;
        inline VkShaderStageFlagBits        getShaderType()         const noexcept;
        inline const VkShaderModule&        getShaderHandle()       const noexcept;
        inline SpecializationInfo&          getSpecializationInfo() noexcept;
        inline const SpecializationInfo&    getSpecializationInfo() const noexcept;

        inline static PtrShaderModule make(
            const PtrDevice&            ptr_device,
            VkShaderStageFlagBits       shader_type,
            const uint32_t*             ptr_shader_code,
            size_t                      shader_code_size,
            size_t                      specialization_info_size_data       = 0,
            size_t                      specialization_info_num_map_entires = 0
        );

    private:
        PtrDevice               _ptr_device;
        VkShaderStageFlagBits   _shader_type;
        SpecializationInfo      _specialization_info;
        VkShaderModule          _shader_handle;
    };
}

#include "ShaderModule.inl"

#endif /* ShaderModule_hpp */
