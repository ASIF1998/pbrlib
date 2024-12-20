#include <backend/renderer/vulkan/descriptor_set.hpp>
#include <backend/renderer/vulkan/device.hpp>

namespace pbrlib::vk
{
    DescriptorSet::DescriptorSet(const Device* ptr_device) :
        _ptr_device (ptr_device)
    { }

    DescriptorSet::DescriptorSet(DescriptorSet&& descriptor_set) :
        _ptr_device (descriptor_set._ptr_device)
    {
        std::swap(handle, descriptor_set.handle);
    }

    DescriptorSet::~DescriptorSet()
    {
        if (handle != VK_NULL_HANDLE)
            vkFreeDescriptorSets(_ptr_device->device(), _ptr_device->descriptorPool(), 1, &handle);
    }

    DescriptorSet& DescriptorSet::operator = (DescriptorSet&& descriptor_set)
    {
        std::swap(handle, descriptor_set.handle);
        return *this;
    }
}