#include "../config.hpp"

#include "image_comparison.hpp"

#include <backend/renderer/vulkan/device.hpp>
#include <backend/renderer/vulkan/image.hpp>
#include <backend/renderer/vulkan/pixel_format.hpp>

#include <backend/renderer/vulkan/gpu_marker_colors.hpp>

#include <backend/renderer/vulkan/compute_pipeline.hpp>
#include <backend/renderer/vulkan/shader_compiler.hpp>

#include <backend/utils/paths.hpp>
#include <backend/renderer/vulkan/check.hpp>

#include <backend/logger/logger.hpp>

namespace pbrlib::testing
{
    ImageComparison::ImageComparison(backend::vk::Device& device) :
        _device (device)
    {
        pbrlib::backend::vk::builders::DescriptorSetLayout descriptor_set_layout_builder (_device);
        descriptor_set_layout_builder
            .addBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_COMPUTE_BIT)
            .addBinding(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_COMPUTE_BIT)
            .addBinding(2, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, VK_SHADER_STAGE_COMPUTE_BIT);

        if constexpr (generate_image_diff)
            descriptor_set_layout_builder.addBinding(3, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1, VK_SHADER_STAGE_COMPUTE_BIT);

        _descriptor_set_layout_handle = descriptor_set_layout_builder.build();

        _pipeline_layout_handle = pbrlib::backend::vk::builders::PipelineLayout(_device)
            .addSetLayout(_descriptor_set_layout_handle)
            .build();

        _descriptor_set_handle = _device.allocateDescriptorSet(_descriptor_set_layout_handle, "[vk-image-comparator] descriptor-set");

        const static auto shader_name = backend::utils::projectRoot() / "pbrlib-tests/image_comparison/image_comparison.glsl.comp";

        backend::vk::builders::ComputePipeline pipeline_builder (_device);
        pipeline_builder
            .pipelineLayoutHandle(_pipeline_layout_handle)
            .shader(shader_name);

        if constexpr (generate_image_diff)
            pipeline_builder.addDefine(backend::vk::shader::Define("PBRLIB_GENERATE_DIFF_IMAGE", ""));

        _pipeline_handle = pipeline_builder.build();

        constexpr VkSamplerCreateInfo sampler_create_info
        {
            .sType      = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
            .magFilter  = VK_FILTER_NEAREST,
            .minFilter  = VK_FILTER_NEAREST
        };

        VK_CHECK(vkCreateSampler(
            _device.device(),
            &sampler_create_info,
            nullptr,
            &_sampler_handle.handle())
        );
    }

    bool ImageComparison::compare(backend::vk::Image& image_1, backend::vk::Image& image_2)
    {
        if (image_1.width != image_2.width || image_1.height != image_2.height) [[unlikely]]
            return false;

        if (image_1.level_count != image_2.level_count) [[unlikely]]
            return false;

        if (image_1.layer_count != image_2.layer_count) [[unlikely]]
            return false;

        if (image_1.layout != VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) [[likely]]
            image_1.changeLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        if (image_2.layout != VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) [[likely]]
            image_2.changeLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        const VkDeviceSize group_count_x        = image_1.width / _device.workGroupSize();
        const VkDeviceSize group_count_y        = image_1.height / _device.workGroupSize();
        const VkDeviceSize group_errors_count   = group_count_x * group_count_y; 

        _group_float_errors = pbrlib::backend::vk::builders::Buffer(_device)
            .addQueueFamilyIndex(_device.queue().family_index)
            .size(group_errors_count * sizeof(float))
            .type(pbrlib::backend::vk::BufferType::eReadback)
            .usage(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT)
            .build();

        std::vector<float> group_errors_in_ram (group_errors_count, 0.0f);
        _group_float_errors->write(std::span<const float>(group_errors_in_ram), 0);

        _device.writeDescriptorSet ({
            .view_handle            = image_1.view_handle,
            .sampler_handle         = _sampler_handle,
            .set_handle             = _descriptor_set_handle,
            .expected_image_layout  = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
            .binding                = 0
        });

        _device.writeDescriptorSet ({
            .view_handle            = image_2.view_handle,
            .sampler_handle         = _sampler_handle,
            .set_handle             = _descriptor_set_handle,
            .expected_image_layout  = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
            .binding                = 1
        });

        _device.writeDescriptorSet ({
            .buffer     = _group_float_errors.value(),
            .set_handle = _descriptor_set_handle,
            .size       = static_cast<uint32_t>(_group_float_errors->size),
            .binding    = 2
        });

        if constexpr (generate_image_diff)
        {
            _images_diff = backend::vk::builders::Image(_device)
                .size(image_1.width, image_1.height)
                .format(image_1.format)
                .usage(VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT)
                .addQueueFamilyIndex(_device.queue().family_index)
                .tiling(VK_IMAGE_TILING_OPTIMAL)
                .name("[vk-image-comparator] diff image")
                .fillColor(math::vec4(0))
                .build();

            if (!_images_diff) [[unlikely]]
                throw exception::RuntimeError("[vk-image-comparator] failed create diff image");

            _device.writeDescriptorSet ({
                .view_handle            = _images_diff->view_handle,
                .set_handle             = _descriptor_set_handle,
                .expected_image_layout  = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                .binding                = 3
            });
        }

        auto command_buffer = _device.oneTimeSubmitCommandBuffer("vk-image-comparator");

        command_buffer.write([&image_1, this] (auto command_buffer_handle)
        {
            PBRLIB_PROFILING_VK_ZONE_SCOPED(_device, command_buffer_handle, "[vk-image-comparator] run-compare-images");

            vkCmdBindPipeline(command_buffer_handle, VK_PIPELINE_BIND_POINT_COMPUTE, _pipeline_handle);

            vkCmdBindDescriptorSets(
                command_buffer_handle,
                VK_PIPELINE_BIND_POINT_COMPUTE,
                _pipeline_layout_handle,
                0, 1, &_descriptor_set_handle.handle(),
                0, nullptr
            );

            const auto group_count_x = image_1.width / _device.workGroupSize();
            const auto group_count_y = image_1.height / _device.workGroupSize();

            vkCmdDispatch(command_buffer_handle, group_count_x, group_count_y, 1);
        }, "[vk-image-comparator] run-compare-images", backend::vk::marker_colors::compute_pipeline);

        _device.submit(command_buffer);

        _group_float_errors->read(reinterpret_cast<uint8_t*>(group_errors_in_ram.data()), sizeof(float) * group_errors_in_ram.size(), 0);

        double total_error = 0.0;
        for (const auto error: group_errors_in_ram)
            total_error += static_cast<double>(error);

        const auto  mse     = total_error / static_cast<double>(image_1.width * image_1.height * backend::channelCount(image_1.format));
        double      psnr    = 100.0;
        if (mse > 1e-10)
        {
            std::visit([&psnr, mse](const auto& channel_value)
            {
                double max_i = static_cast<double>(channel_value);
                psnr = 10.0 * std::log10((max_i * max_i) / mse);
            }, backend::channelMaxValue(image_1.format));
        }

        constexpr auto psnr_threshold = 45.0;
        return psnr >= psnr_threshold;
    }

    bool ImageComparison::compare (
        backend::vk::Image&             image,
        const std::filesystem::path&    path_to_reference
    )
    {
        if (path_to_reference.empty()) [[unlikely]]
            return false;

        if constexpr (generate_image_reference)
        {
            backend::vk::exporters::Image(_device)
                .filename(path_to_reference)
                .image(&image)
                .save();

            return true;
        }

        auto reference_image = backend::vk::loaders::Image(_device)
            .filename(path_to_reference)
            .load();

        const bool is_equal = compare(image, reference_image);

        if constexpr (generate_image_diff)
        {
            if (!is_equal && _images_diff) [[unlikely]]
            {
                const auto filename             = path_to_reference.stem().string();
                const auto extension            = backend::channelSize(image.format) == 1 ? ".png" : ".exr";
                const auto path_to_diff_image   = std::format("pbrlib-tests/references/diffs/{}-diff.{}", filename, extension);

                _images_diff->changeLayout(VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);

                pbrlib::backend::vk::exporters::Image(_device)
                    .filename(backend::utils::projectRoot() / path_to_diff_image)
                    .image(&_images_diff.value())
                    .save();
            }
        }

        return is_equal;
    }
}
