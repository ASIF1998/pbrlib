//
//  DescriptorPool.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 25/03/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include "DescriptorPool.hpp"

#include <cassert>

namespace pbrlib
{
    DescriptorPool::DescriptorPool(
        const PtrDevice&                    ptr_device, 
        const vector<VkDescriptorPoolSize>& descriptor_pool_sizes,
        uint32_t                            max_sets
    ) :
        _ptr_device             (ptr_device),
        _descriptor_pool_handle (VK_NULL_HANDLE)
    {
        VkDescriptorPoolCreateInfo descriptor_pool_create_info = { };
        descriptor_pool_create_info.sType           = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        descriptor_pool_create_info.flags           = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        descriptor_pool_create_info.maxSets         = max_sets;
        descriptor_pool_create_info.poolSizeCount   = static_cast<uint32_t>(descriptor_pool_sizes.size());
        descriptor_pool_create_info.pPoolSizes      = descriptor_pool_sizes.data();

        assert(vkCreateDescriptorPool(
            _ptr_device->getDeviceHandle(),
            &descriptor_pool_create_info,
            nullptr,
            &_descriptor_pool_handle
        ) == VK_SUCCESS);

        assert(_descriptor_pool_handle != VK_NULL_HANDLE);
    }

    DescriptorPool::DescriptorPool(
        const PtrDevice&            ptr_device,
        const VkDescriptorPoolSize& descriptor_pool_size,
        uint32_t                    max_sets
    ) :
        _ptr_device             (ptr_device),
        _descriptor_pool_handle (VK_NULL_HANDLE)
    {
        VkDescriptorPoolCreateInfo descriptor_pool_create_info = { };
        descriptor_pool_create_info.sType           = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        descriptor_pool_create_info.flags           = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        descriptor_pool_create_info.maxSets         = max_sets;
        descriptor_pool_create_info.poolSizeCount   = 1;
        descriptor_pool_create_info.pPoolSizes      = &descriptor_pool_size;

        assert(vkCreateDescriptorPool(
            _ptr_device->getDeviceHandle(),
            &descriptor_pool_create_info,
            nullptr,
            &_descriptor_pool_handle
        ) == VK_SUCCESS);

        assert(_descriptor_pool_handle != VK_NULL_HANDLE);
    }

    DescriptorPool::DescriptorPool(DescriptorPool&& descriptor_pool) :
        _ptr_device             (move(descriptor_pool._ptr_device)),
        _descriptor_pool_handle (VK_NULL_HANDLE)
    {
        swap(_descriptor_pool_handle, descriptor_pool._descriptor_pool_handle);
    }

    DescriptorPool::~DescriptorPool()
    {
        if (_descriptor_pool_handle != VK_NULL_HANDLE) {
            vkDestroyDescriptorPool(_ptr_device->getDeviceHandle(), _descriptor_pool_handle, nullptr);
        }
    }

    void DescriptorPool::reset(VkDescriptorPoolResetFlags reset_flags) const noexcept
    {
        assert(vkResetDescriptorPool(
            _ptr_device->getDeviceHandle(), 
            _descriptor_pool_handle, 
            reset_flags
        ) == VK_SUCCESS);
    }

    PtrDevice& DescriptorPool::getDevice() noexcept
    {
        return _ptr_device;
    }

    const PtrDevice& DescriptorPool::getDevice() const noexcept
    {
        return _ptr_device;
    }

    const VkDescriptorPool& DescriptorPool::getDescriptorPoolHandle() const noexcept
    {
        return _descriptor_pool_handle;
    }

    PtrDescriptorPool DescriptorPool::make(
        const PtrDevice&                    ptr_device,
        const vector<VkDescriptorPoolSize>& descriptor_pool_sizes,
        uint32_t                            max_sets
    )
    {
        return make_shared<DescriptorPool>(ptr_device, descriptor_pool_sizes, max_sets);
    }

    PtrDescriptorPool DescriptorPool::make(
        const PtrDevice&            ptr_device,
        const VkDescriptorPoolSize& descriptor_pool_size,
        uint32_t                    max_sets
    )
    {
        return make_shared<DescriptorPool>(ptr_device, descriptor_pool_size, max_sets);
    }   
}
