//
//  DescriptorSet.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 27/03/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include "DescriptorSet.hpp"

#include "Sampler.hpp"
#include "Image.hpp"

#include "Buffer.hpp"

namespace pbrlib
{
    DescriptorSet::DescriptorSet(
        const PtrDescriptorPool&        ptr_descriptor_pool, 
        const PtrDescriptorSetLayout&   ptr_descriptor_set_layout
    ) :
        _ptr_descriptor_pool        (ptr_descriptor_pool),
        _ptr_descriptor_set_layout  (ptr_descriptor_set_layout),
        _descriptor_set_handle      (VK_NULL_HANDLE)
    {
        VkDescriptorSetAllocateInfo descriptor_set_allocate_info {
            .sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
            .descriptorPool     = ptr_descriptor_pool->getDescriptorPoolHandle(),
            .descriptorSetCount = 1,
            .pSetLayouts        = &_ptr_descriptor_set_layout->getDescriptorSetLayoutHandle()
        };

        assert(vkAllocateDescriptorSets(
            getDevice()->getDeviceHandle(),
            &descriptor_set_allocate_info,
            &_descriptor_set_handle
        ) == VK_SUCCESS);

        assert(_descriptor_set_handle != VK_NULL_HANDLE);
    }

    DescriptorSet::DescriptorSet(DescriptorSet&& descriptor_set) :
        _ptr_descriptor_pool        (move(descriptor_set._ptr_descriptor_pool)),
        _ptr_descriptor_set_layout  (move(descriptor_set._ptr_descriptor_set_layout)),
        _descriptor_set_handle      (VK_NULL_HANDLE)
    {
        swap(_descriptor_set_handle, descriptor_set._descriptor_set_handle);
    }

    DescriptorSet::~DescriptorSet()
    {
        if (_descriptor_set_handle != VK_NULL_HANDLE) {
            assert(vkFreeDescriptorSets(
                getDevice()->getDeviceHandle(),
                _ptr_descriptor_pool->getDescriptorPoolHandle(),
                1,
                &_descriptor_set_handle
            ) == VK_SUCCESS);
        }
    }

    PtrDevice& DescriptorSet::getDevice() noexcept
    {
        return _ptr_descriptor_pool->getDevice();
    }

    const PtrDevice& DescriptorSet::getDevice() const noexcept
    {
        return _ptr_descriptor_pool->getDevice();
    }

    PtrDescriptorPool& DescriptorSet::getDescriptorPool() noexcept
    {
        return _ptr_descriptor_pool;
    }

    const PtrDescriptorPool& DescriptorSet::getDescriptorPool() const noexcept
    {
        return _ptr_descriptor_pool;
    }

    PtrDescriptorSetLayout& DescriptorSet::getDescriptorSetLayout() noexcept
    {
        return _ptr_descriptor_set_layout;
    }

    const PtrDescriptorSetLayout& DescriptorSet::getDescriptorSetLayout() const noexcept
    {
        return _ptr_descriptor_set_layout;
    }

    const VkDescriptorSet& DescriptorSet::getDescriptorSetHandle() const noexcept
    {
        return _descriptor_set_handle;
    }

    void DescriptorSet::writeImageView(
        const ImageView&    image_view,
        VkImageLayout       image_layout,
        const Sampler&      sampler,
        uint32_t            binding, 
        VkDescriptorType    descriptor_type
    ) const
    {
        VkDescriptorImageInfo descriptor_image_info {
            .sampler        = sampler.getSamplerHandle(),
            .imageView      = image_view.getImageViewHandle(),
            .imageLayout    = image_layout
        };

        VkWriteDescriptorSet write_descriptor_set {
            .sType              = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstSet             = _descriptor_set_handle,
            .dstBinding         = binding,     
            .dstArrayElement    = 0,
            .descriptorCount    = 1,
            .descriptorType     = descriptor_type,
            .pImageInfo         = &descriptor_image_info
        };

        vkUpdateDescriptorSets(
            getDevice()->getDeviceHandle(),
            1, &write_descriptor_set,
            0, nullptr
        );
    }

    void DescriptorSet::writeImageView(
        const ImageView&    image_view,
        VkImageLayout       image_layout,
        uint32_t            binding, 
        VkDescriptorType    descriptor_type
    ) const
    {
        VkDescriptorImageInfo descriptor_image_info {
            .sampler        = VK_NULL_HANDLE,
            .imageView      = image_view.getImageViewHandle(),
            .imageLayout    = image_layout
        };

        VkWriteDescriptorSet write_descriptor_set {
            .sType              = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstSet             = _descriptor_set_handle,
            .dstBinding         = binding,     
            .dstArrayElement    = 0,
            .descriptorCount    = 1,
            .descriptorType     = descriptor_type,
            .pImageInfo         = &descriptor_image_info
        };

        vkUpdateDescriptorSets(
            getDevice()->getDeviceHandle(),
            1, &write_descriptor_set,
            0, nullptr
        );
    }

    void DescriptorSet::writeBuffer(
        const Buffer&       buffer,
        VkDeviceSize        offset,
        VkDeviceSize        range,
        uint32_t            binding, 
        VkDescriptorType    descriptor_type
    ) const
    {
        VkDescriptorBufferInfo descriptor_buffer_info {
            .buffer = buffer.getBufferHandle(),
            .offset = offset,
            .range  = range
        };

        VkWriteDescriptorSet write_descriptor_set {
            .sType              = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstSet             = _descriptor_set_handle,
            .dstBinding         = binding,     
            .dstArrayElement    = 0,
            .descriptorCount    = 1,
            .descriptorType     = descriptor_type,
            .pBufferInfo        = &descriptor_buffer_info
        };

        vkUpdateDescriptorSets(
            getDevice()->getDeviceHandle(),
            1, &write_descriptor_set,
            0, nullptr
        );
    }

    PtrDescriptorSet DescriptorSet::make(
        const PtrDescriptorPool&        ptr_descriptor_pool,
        const PtrDescriptorSetLayout&   ptr_descriptor_set_layout
    )
    {
        return make_shared<DescriptorSet>(ptr_descriptor_pool, ptr_descriptor_set_layout);
    }
}
