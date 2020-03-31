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
        DescriptorPool(
            const PtrDevice&                    ptr_device,
            const vector<VkDescriptorPoolSize>& descriptor_pool_sizes,
            uint32_t                            max_sets
        );

        DescriptorPool(
            const PtrDevice&            ptr_device,
            const VkDescriptorPoolSize& descriptor_pool_size,
            uint32_t                    max_sets
        );

        DescriptorPool(DescriptorPool&& descriptor_pool); 
        DescriptorPool(const DescriptorPool&) = delete;

        ~DescriptorPool();

        DescriptorPool& operator = (const DescriptorPool&)  = delete;
        DescriptorPool& operator = (DescriptorPool&&)       = delete;

        void reset(VkDescriptorPoolResetFlags reset_flags) const noexcept;

        PtrDevice&               getDevice()                 noexcept;
        const PtrDevice&         getDevice()                 const noexcept;
        const VkDescriptorPool&  getDescriptorPoolHandle()   const noexcept;

        static PtrDescriptorPool make(
            const PtrDevice&                    ptr_device,
            const vector<VkDescriptorPoolSize>& descriptor_pool_sizes,
            uint32_t                            max_sets
        );

        static PtrDescriptorPool make(
            const PtrDevice&            ptr_device,
            const VkDescriptorPoolSize& descriptor_pool_size,
            uint32_t                    max_sets
        );

    private:
        PtrDevice                       _ptr_device;
        VkDescriptorPool                _descriptor_pool_handle;
    };
}

#endif /* DescriptorPool_hpp */
