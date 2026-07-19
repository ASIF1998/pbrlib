#pragma once

#include <backend/renderer/vulkan/unique_handler.hpp>
#include <backend/renderer/vulkan/image.hpp>
#include <backend/renderer/vulkan/buffer.hpp>

#include <map>
#include <variant>

namespace pbrlib::backend::vk
{
    using DescriptorBinderResource = std::variant<const vk::Buffer*, const vk::Image*>;

    class DescriptorBinder final
    {
    public:
        DescriptorBinder(const vk::DescriptorSetHandle& set_handle) noexcept;

        void add(uint32_t bind_id, const vk::Buffer& buffer);
        void add(uint32_t bind_id, const vk::Image& image);

    private:
        const vk::DescriptorSetHandle&                  _set_handle;
        std::map<uint32_t, DescriptorBinderResource>    _resources;
    };
}
