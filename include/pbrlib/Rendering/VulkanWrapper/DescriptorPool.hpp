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

#include <span>

/**
 * TODO: Добавить метод createDescriptorSet(...) в класс DescriptorPool.
*/

namespace pbrlib
{
    class DescriptorPool
    {
    public:
        DescriptorPool(
            const Device*                           ptr_device,
            std::span<const VkDescriptorPoolSize>   descriptor_pool_sizes,
            uint32_t                                max_sets
        );

        DescriptorPool(
            const Device*               ptr_device,
            const VkDescriptorPoolSize& descriptor_pool_size,
            uint32_t                    max_sets
        );

        DescriptorPool(DescriptorPool&& descriptor_pool); 
        DescriptorPool(const DescriptorPool&) = delete;

        ~DescriptorPool();

        DescriptorPool& operator = (const DescriptorPool&)  = delete;
        DescriptorPool& operator = (DescriptorPool&&)       = delete;

        void reset(VkDescriptorPoolResetFlags reset_flags) const noexcept;

        const Device*           getDevice()                 const noexcept;
        const VkDescriptorPool& getDescriptorPoolHandle()   const noexcept;

        static std::unique_ptr<DescriptorPool> make(
            const Device*                           ptr_device,
            std::span<const VkDescriptorPoolSize>   descriptor_pool_sizes,
            uint32_t                                max_sets
        );

        static std::unique_ptr<DescriptorPool> make(
            const Device*               ptr_device,
            const VkDescriptorPoolSize& descriptor_pool_size,
            uint32_t                    max_sets
        );

    private:
        const Device*       _ptr_device;
        VkDescriptorPool    _descriptor_pool_handle;
    };
}

#endif /* DescriptorPool_hpp */
