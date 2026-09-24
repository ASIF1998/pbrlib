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
        _set_layout(descriptor_set_layout_builder.build()),
        _set_handle(device.allocateDescriptorSet(_set_layout, name))
    { }

    void DescriptorGroup::add(uint32_t bind_id, vk::Buffer& buffer)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        _resources.emplace(bind_id, &buffer);
    }

    void DescriptorGroup::add(uint32_t bind_id, vk::Image& image)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        _resources.emplace(bind_id, &image);
    }

    void DescriptorGroup::modify(std::function<void(uint32_t, vk::Image&)> modifier)
    {
        PBRLIB_ENABLE_PROFILING;

        if (!modifier) [[unlikely]]
        {
            log::warning("[vk-descriptor-group] image modifier is empty");
            return ;
        }

        for (auto& [bind_id, resource]: _resources)
        {
            if (std::holds_alternative<vk::Image*>(resource))
            {
                if (auto ptr_image = std::get<vk::Image*>(resource))
                    modifier(bind_id, *ptr_image);
            }
        }
    }

    void DescriptorGroup::modify(std::function<void(uint32_t, vk::Buffer&)> modifier)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        if (!modifier) [[unlikely]]
        {
            log::warning("[vk-descriptor-group] buffer modifier is empty");
            return ;
        }

        for (auto& [bind_id, resource]: _resources)
        {
            if (std::holds_alternative<vk::Buffer*>(resource))
            {
                if (auto ptr_buffer = std::get<vk::Buffer*>(resource))
                    modifier(bind_id, *ptr_buffer);
            }
        }
    }

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

    void DescriptorGroup::changeColorImagesLayout(CommandBuffer& command_buffer, VkImageLayout new_layout)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        modify([new_layout, &command_buffer] ([[maybe_unused]] uint32_t bind_id, vk::Image& image)
        {
            if (!isDepthImage(image.format)) [[likely]]
                image.transition(command_buffer, new_layout);
        });
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
            if (auto result = std::get_if<vk::Image*>(&resource))
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
            else if (auto result = std::get_if<vk::Buffer*>(&resource)) [[likely]]
            {
                auto ptr_buffer = *result;
                if (descriptor_group_transition.hasBind(bind_id) && ptr_buffer) 
                {
                    ptr_buffer->transition(command_buffer);
                }                
            }
            else
                throw exception::RuntimeError("[vk-descriptor-group] invalid resource");
        }
    }
}
