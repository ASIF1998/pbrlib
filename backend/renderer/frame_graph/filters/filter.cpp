#include <backend/renderer/frame_graph/filters/filter.hpp>
#include <backend/renderer/vulkan/pipeline_layout.hpp>
#include <backend/renderer/vulkan/device.hpp>
#include <backend/renderer/vulkan/image.hpp>
#include <backend/renderer/vulkan/check.hpp>

#include <pbrlib/exceptions.hpp>

#include <format>

namespace pbrlib::backend
{
    Filter::Filter(std::string_view name, vk::Device& device, vk::Image& dst_image) noexcept :
        RenderPass      (device),
        _name           (name),
        _ptr_dst_image  (&dst_image)
    {
        _io_descriptor_set_layout_handle = vk::builders::DescriptorSetLayout(device)
            .addBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_COMPUTE_BIT)
            .addBinding(1, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1, VK_SHADER_STAGE_COMPUTE_BIT)
            .build();

        _io_descriptor_set_handle = device.allocateDescriptorSet (
            _io_descriptor_set_layout_handle,
            std::format("[{}] input descriptor set", _name)
        );

        device.writeDescriptorSet ({
            .view_handle            = _ptr_dst_image->view_handle.handle(),
            .set_handle             = _io_descriptor_set_handle,
            .expected_image_layout  = VK_IMAGE_LAYOUT_GENERAL,
            .binding                = 1
        });
    }

    void Filter::apply(vk::Image& image)
    {
        _ptr_src_image = &image;

        constexpr auto dst_stage = VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT;

        addSyncImage (
            _ptr_src_image,
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
            VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT, dst_stage
        );

        addSyncImage (
            _ptr_dst_image,
            VK_IMAGE_LAYOUT_GENERAL,
            VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT, dst_stage
        );

        _input_image_sampler_handle = device().createLinearSampler();

        device().writeDescriptorSet ({
            .view_handle            = srcImage().view_handle.handle(),
            .sampler_handle         = _input_image_sampler_handle,
            .set_handle             = _io_descriptor_set_handle,
            .expected_image_layout  = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
            .binding                = 0
        });
    }

    vk::Image& Filter::srcImage()
    {
        if (!_ptr_src_image) [[unlikely]]
            throw exception::InvalidState(std::format("[{}] didn't set source image", _name));

        return *_ptr_src_image;
    }

    vk::Image& Filter::dstImage() noexcept
    {
        return *_ptr_dst_image;
    }

    std::pair<VkDescriptorSet, VkDescriptorSetLayout> Filter::IODescriptorSet() noexcept
    {
        return std::make_pair(_io_descriptor_set_handle.handle(), _io_descriptor_set_layout_handle.handle());
    }

    void Filter::dispatchCompute(VkCommandBuffer command_buffer_handle)
    {
        const auto [width, height] = size();

        const auto group_count_x = width / device().workGroupSize();
        const auto group_count_y = height / device().workGroupSize();

        vkCmdDispatch(command_buffer_handle, group_count_x, group_count_y, 1);
    }
}
