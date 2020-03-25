//
//  DescriptorPool.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 25/03/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef DescriptorPool_hpp
#define DescriptorPool_hpp

#include "Device.hpp"

#include <vector>

using namespace std;

/**
 * TODO: Добавить метод createDescriptorSet(...) в класс DescriptorPool.
*/

namespace pbrlib
{
    class DescriptorPool;

    using PtrDescriptorPool = shared_ptr<DescriptorPool>;


    class DescriptorPool
    {
    public:
        inline DescriptorPool(
            const PtrDevice&                    ptr_device,
            const vector<VkDescriptorPoolSize>& descriptor_pool_sizes,
            uint32_t                            max_sets
        );

        inline DescriptorPool(DescriptorPool&& descriptor_pool); 
        DescriptorPool(const DescriptorPool&) = delete;

        inline ~DescriptorPool();

        DescriptorPool& operator = (const DescriptorPool&)  = delete;
        DescriptorPool& operator = (DescriptorPool&&)       = delete;

        inline void reset(VkDescriptorPoolResetFlags reset_flags) const noexcept;

        inline PtrDevice&               getDevice()                 noexcept;
        inline const PtrDevice&         getDevice()                 const noexcept;
        inline const VkDescriptorPool&  getDescriptorPoolHandle()   const noexcept;

        inline static PtrDescriptorPool make(
            const PtrDevice&                    ptr_device,
            const vector<VkDescriptorPoolSize>& descriptor_pool_sizes,
            uint32_t                            max_sets
        );

    private:
        PtrDevice                       _ptr_device;
        VkDescriptorPool                _descriptor_pool_handle;
    };
}

#include "DescriptorPool.inl"

#endif /* DescriptorPool_hpp */
