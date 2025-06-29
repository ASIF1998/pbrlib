#include "../config.hpp"

#include "image_comparison.hpp"

#include <backend/renderer/vulkan/device.hpp>
#include <backend/renderer/vulkan/image.hpp>

#include <backend/renderer/vulkan/gpu_marker_colors.hpp>

#include <backend/renderer/vulkan/shader_compiler.hpp>

#include <backend/utils/paths.hpp>
#include <backend/utils/vulkan.hpp>

#include <backend/logger/logger.hpp>

#include <array>

namespace pbrlib::testing
{
    ImageComparison::ImageComparison(backend::vk::Device& device) :
        _device (device)
    {
        _pipeline_layout = pbrlib::backend::vk::PipelineLayout::Builder(_device)
            .addSet()
                .addBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_COMPUTE_BIT)
                .addBinding(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_COMPUTE_BIT)
                .addBinding(2, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, VK_SHADER_STAGE_COMPUTE_BIT)
            .build();

        _descriptor_set_handle = _device.allocateDescriptorSet(
            _pipeline_layout->sets_layout[0], 
            "[vk-image-comparator] descriptor-set"
        );

        const static auto shader_name = backend::utils::projectRoot() / "pbrlib-tests/image_comparison/image_comparison.glsl.comp";

        const VkComputePipelineCreateInfo pipeline_create_info
        {
            .sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,
            .stage = 
            {
                .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                .stage  = VK_SHADER_STAGE_COMPUTE_BIT,
                .module = backend::vk::shader::compile(_device, shader_name),
                .pName  = "main"
            },
            .layout = _pipeline_layout->handle
        };

        const auto device_handle = _device.device();

        VK_CHECK(vkCreateComputePipelines(
            device_handle, 
            VK_NULL_HANDLE, 
            1, &pipeline_create_info, 
            nullptr, 
            &_pipeline_handle
        ));

        vkDestroyShaderModule(device_handle, pipeline_create_info.stage.module, nullptr);

        const VkSamplerCreateInfo sampler_create_info
        {
            .sType      = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
            .magFilter  = VK_FILTER_NEAREST,
            .minFilter  = VK_FILTER_NEAREST
        };

        VK_CHECK(vkCreateSampler(device_handle, &sampler_create_info, nullptr, &_sampler_handle));

        _count_changed_pixels_buffer = pbrlib::backend::vk::builders::Buffer(_device)
            .addQueueFamilyIndex(_device.queue().family_index)
            .size(sizeof(uint32_t))
            .type(pbrlib::backend::vk::BufferType::eReadback)
            .usage(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT)
            .build(); 
        
        _count_changed_pixels_buffer->write(0, 0);
    }

    ImageComparison::~ImageComparison()
    {
        const auto device_handle = _device.device();
        
        vkDestroyPipeline(device_handle, _pipeline_handle, nullptr);
        vkDestroySampler(device_handle, _sampler_handle, nullptr);

        vkFreeDescriptorSets(_device.device(), _device.descriptorPool(), 1, &_descriptor_set_handle);
    }

    bool ImageComparison::compare(const backend::vk::Image& image_1, const backend::vk::Image& image_2)
    {
        if (image_1.width != image_2.width || image_1.height != image_2.height) [[unlikely]]
            return false;

        if (image_1.level_count != image_2.level_count) [[unlikely]]
            return false;
        
        if (image_1.layer_count != image_2.layer_count) [[unlikely]]
            return false;

        const VkDescriptorImageInfo image_1_info 
        { 
            .sampler        = _sampler_handle,
            .imageView      = image_1.view_handle,
            .imageLayout    = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        };

        const VkDescriptorImageInfo image_2_info 
        { 
            .sampler        = _sampler_handle,
            .imageView      = image_2.view_handle,
            .imageLayout    = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        };

        const VkDescriptorBufferInfo count_changed_pixels_buffer_info
        {
            .buffer = _count_changed_pixels_buffer->handle,
            .range  = _count_changed_pixels_buffer->size
        };

        const std::array desc_write_info
        {
            VkWriteDescriptorSet
            { 
                .sType              = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                .dstSet             = _descriptor_set_handle,
                .dstBinding         = 0,
                .descriptorCount    = 1,
                .descriptorType     = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                .pImageInfo         = &image_1_info
            },
            VkWriteDescriptorSet
            { 
                .sType              = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                .dstSet             = _descriptor_set_handle,
                .dstBinding         = 1,
                .descriptorCount    = 1,
                .descriptorType     = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                .pImageInfo         = &image_2_info
            },
            VkWriteDescriptorSet
            { 
                .sType              = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                .dstSet             = _descriptor_set_handle,
                .dstBinding         = 2,
                .descriptorCount    = 1,
                .descriptorType     = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
                .pBufferInfo        = &count_changed_pixels_buffer_info
            }
        };

        vkUpdateDescriptorSets(
            _device.device(),
            static_cast<uint32_t>(desc_write_info.size()), desc_write_info.data(),
            0, nullptr
        );

        auto command_buffer = _device.oneTimeSubmitCommandBuffer("vk-image-comparator");

        command_buffer.write([&image_1, this] (auto command_buffer_handle)
        {
            PBRLIB_PROFILING_VK_ZONE_SCOPED(_device, command_buffer_handle, "[vk-image-comparator] run-compare-images");

            vkCmdBindPipeline(command_buffer_handle, VK_PIPELINE_BIND_POINT_COMPUTE, _pipeline_handle);
            vkCmdBindDescriptorSets(command_buffer_handle, VK_PIPELINE_BIND_POINT_COMPUTE, _pipeline_layout->handle, 0, 1, &_descriptor_set_handle, 0, nullptr);
            vkCmdDispatch(command_buffer_handle, image_1.width, image_1.height, 1);
        }, "[vk-image-comparator] run-compare-images", backend::vk::marker_colors::compute_pipeline);

        _device.submit(command_buffer);

        uint32_t count_changed_pixels = 0;
        _count_changed_pixels_buffer->read(count_changed_pixels, 0);

        return count_changed_pixels == 0;
    }

    bool ImageComparison::compare (
        const backend::vk::Image&       image_1, 
        const std::filesystem::path&    path_to_reference
    )
    {
        if (path_to_reference.empty()) [[unlikely]]
            return false;

        if constexpr (generate_image_reference)
        {
            backend::vk::Image::Exporter(_device)
                .filename(path_to_reference)
                .image(&image_1)
                .exoprt();

            return true;
        }

        return compare(
            image_1, 
            backend::vk::Image::Loader(_device)
                .filename(path_to_reference)
                .load()
        );
    }
}