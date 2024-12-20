#pragma once

#include <vulkan/vulkan.h>

#include <vector>

namespace pbrlib::vk
{
    class Device;
}

namespace pbrlib::vk
{
    class DescriptorSet
    {
        friend class Device;

        DescriptorSet(const Device* ptr_device);

        const Device* _ptr_device;

    public:
        DescriptorSet(DescriptorSet&& descriptor_set);
        DescriptorSet(const DescriptorSet& descriptor_set) = delete;

        ~DescriptorSet();

        DescriptorSet& operator = (DescriptorSet&& descriptor_set);
        DescriptorSet& operator = (const DescriptorSet& descriptor_set) = delete;

        VkDescriptorSet handle = VK_NULL_HANDLE;
    };
}