#include <backend/renderer/vulkan/descriptor_binder.hpp>

namespace pbrlib::backend::vk
{
    DescriptorBinder::DescriptorBinder(const vk::DescriptorSetHandle& set_handle) noexcept :
        _set_handle (set_handle)
    { }

    void DescriptorBinder::add(uint32_t bind_id, const vk::Buffer& buffer)
    {
        _resources.emplace(bind_id, &buffer);
    }

    void DescriptorBinder::add(uint32_t bind_id, const vk::Image& image)
    {
        _resources.emplace(bind_id, &image);
    }
}
