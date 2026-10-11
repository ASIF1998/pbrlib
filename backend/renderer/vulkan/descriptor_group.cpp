#include <backend/renderer/vulkan/descriptor_group.hpp>
#include <backend/renderer/vulkan/pipeline_layout.hpp>
#include <backend/renderer/vulkan/image.hpp>
#include <backend/renderer/vulkan/buffer.hpp>

#include <pbrlib/exceptions.hpp>

#include <backend/logger/logger.hpp>

#include <backend/profiling.hpp>

namespace pbrlib::backend::vk
{
    DescriptorGroupTransition& DescriptorGroupTransition::bind (uint32_t bind_id, VkPipelineStageFlags2 src_stage, VkPipelineStageFlags2 dst_stage, VkImageLayout image_layout)
    {
        _binds.emplace(bind_id, std::make_tuple(src_stage, dst_stage, image_layout));
        return *this;
    }

    std::optional<DescriptorGroupTransition::StageConfig> DescriptorGroupTransition::config(int32_t bind_id) const
    {
        if (const auto res = _binds.find(bind_id); res != std::end(_binds)) [[likely]]
        {
            return res->second;
        }

        return std::nullopt;
    }

    bool DescriptorGroupTransition::hasBind(uint32_t bind_id) const
    {
        return _binds.find(bind_id) != std::end(_binds);
    }

    DescriptorGroup::DescriptorGroup (
        Device&                                 device,
        const builders::DescriptorSetLayout&    descriptor_set_layout_builder,
        std::string_view                        name
    ) :
        _device     (device),
        _set_layout (descriptor_set_layout_builder.build()),
        _set_handle (device.allocateDescriptorSet(_set_layout, name))
    { }

    bool isDepthImage(VkFormat format) noexcept
    {
        switch (format)
        {
            case VK_FORMAT_D16_UNORM:
            case VK_FORMAT_D16_UNORM_S8_UINT:
            case VK_FORMAT_D24_UNORM_S8_UINT:
            case VK_FORMAT_D32_SFLOAT:
            case VK_FORMAT_D32_SFLOAT_S8_UINT:
                return true;
            default:
                return false;
        };
    }

    const VkDescriptorSet& DescriptorGroup::descriptorSetHandle() const noexcept
    {
        return _set_handle.handle();
    }

    const VkDescriptorSetLayout& DescriptorGroup::descriptorSetLayoutHandle() const noexcept
    {
        return _set_layout.handle();
    }

    void DescriptorGroup::transition(CommandBuffer& command_buffer, const DescriptorGroupTransition& descriptor_group_transition) const
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        for (auto& [bind_id, resource]: _resources)
        {
            if (auto result = std::get_if<const vk::Image*>(&resource))
            {
                if (auto ptr_image = *result)
                {
                    if (const auto config = descriptor_group_transition.config(bind_id))
                    {
                        const auto& [src_stage, dst_stage, layout] = config.value();
                        ptr_image->transition(command_buffer, layout, src_stage, dst_stage);
                    }
                }
            }
            else if (auto result = std::get_if<const vk::Buffer*>(&resource)) [[likely]]
            {
                auto ptr_buffer = *result;
                if (descriptor_group_transition.hasBind(bind_id) && ptr_buffer)
                    ptr_buffer->transition(command_buffer);
            }
            else
                throw exception::RuntimeError("[vk-descriptor-group] invalid resource");
        }
    }

    void DescriptorGroup::write(const DescriptorImageInfo& descriptor_image_info)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        if (descriptor_image_info.image.view_handle == VK_NULL_HANDLE) [[unlikely]]
            throw exception::InvalidArgument("[vk-descriptor-group] descriptor_image_info.view_handle is null");

        if (descriptor_image_info.expected_image_layout == VK_IMAGE_LAYOUT_UNDEFINED) [[unlikely]]
            throw exception::InvalidArgument("[vk-descriptor-group] descriptor_image_info.expected_image_layout is undefined");

        const auto descriptor_type = descriptor_image_info.sampler_handle == VK_NULL_HANDLE
            ?   VK_DESCRIPTOR_TYPE_STORAGE_IMAGE
            :   VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

        const VkDescriptorImageInfo image_info
        {
            .sampler        = descriptor_image_info.sampler_handle,
            .imageView      = descriptor_image_info.image.view_handle,
            .imageLayout    = descriptor_image_info.expected_image_layout
        };

        const VkWriteDescriptorSet write_info
        {
            .sType              = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstSet             = _set_handle,
            .dstBinding         = descriptor_image_info.binding,
            .dstArrayElement    = descriptor_image_info.array_element,
            .descriptorCount    = 1,
            .descriptorType     = descriptor_type,
            .pImageInfo         = &image_info
        };

        vkUpdateDescriptorSets (
            _device.device(),
            1, &write_info,
            0, nullptr
        );

        _resources[descriptor_image_info.binding] = &descriptor_image_info.image;
    }

    void DescriptorGroup::write(const DescriptorBufferInfo& descriptor_buffer_info)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        if (descriptor_buffer_info.buffer.handle == VK_NULL_HANDLE) [[unlikely]]
            throw exception::InvalidArgument("[vk-descriptor-group] descriptor_buffer_info.buffer.handle is null");

        const VkDescriptorBufferInfo buffer_info
        {
            .buffer	= descriptor_buffer_info.buffer.handle,
            .offset	= descriptor_buffer_info.offset,
            .range	= descriptor_buffer_info.size
        };

        const auto descriptor_type = descriptor_buffer_info.buffer.usage & VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT
            ?   VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER
            :   VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;

        const VkWriteDescriptorSet write_info
        {
            .sType              = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstSet             = _set_handle,
            .dstBinding         = descriptor_buffer_info.binding,
            .dstArrayElement    = descriptor_buffer_info.array_element,
            .descriptorCount    = 1,
            .descriptorType     = descriptor_type,
            .pBufferInfo        = &buffer_info
        };

        vkUpdateDescriptorSets (
            _device.device(),
            1, &write_info,
            0, nullptr
        );

        _resources[descriptor_buffer_info.binding] = &descriptor_buffer_info.buffer;
    }
}
