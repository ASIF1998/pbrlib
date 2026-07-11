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

#include <backend/math/float16.hpp>

#include <ranges>

namespace pbrlib::math
{
    using half4 = pbrlib::math::Vec4<pbrlib::backend::math::float16_t>;

    template<typename T>
    struct Scalar final
    {
        using ElementType = T;

        explicit Scalar(T init_value = static_cast<T>(0.0f)) :
            value(init_value)
        { }

        [[nodiscard]] T operator [] ([[maybe_unused]] size_t i) const noexcept
        {
            return value;
        }
        
        [[nodiscard]] Scalar operator - (const Scalar& v)  const noexcept
        {
            return Scalar(value - v.value);
        }

        [[nodiscard]] T& operator [] ([[maybe_unused]] size_t i) noexcept
        {
            return value;
        }

        [[nodiscard]] Scalar operator * (Scalar s) const noexcept
        {
            return Scalar(value * s.value);
        }

        [[nodiscard]] Scalar operator * (T s) const noexcept
        {
            return Scalar(value * s);
        }

        Scalar& operator += (Scalar s) noexcept
        {
            value += s.value;
            return *this;
        }

        T value;

        static constexpr size_t element_count = 4;
    };

    template<typename T>
    [[nodiscard]] bool isfinite(Scalar<T> scalar)
    {
        return std::isfinite(static_cast<float>(scalar.value));
    }

    template<typename T>
    [[nodiscard]] Scalar<T> min(Scalar<T> s1, Scalar<T> s2)
    {
        return Scalar<T>(std::min(static_cast<float>(s1.value), static_cast<float>(s2.value)));
    }

    template<typename T>
    [[nodiscard]] Scalar<T> max(Scalar<T> s1, Scalar<T> s2)
    {
        return Scalar<T>(std::max(static_cast<float>(s1.value), static_cast<float>(s2.value)));
    }
}

namespace pbrlib::testing
{
    template<typename PixelType>
    bool psnr(const backend::vk::Image& rendered_image, const backend::vk::Image& reference_image)
    {
        const auto rendered_image_buffer = rendered_image.fetch("[vk-image-comparator] rendered image buffer");
        const auto reference_image_buffer = reference_image.fetch("[vk-image-comparator] reference image buffer");

        bool all_channels_passed = true;

        rendered_image_buffer.map([&reference_image_buffer, &all_channels_passed](std::span<const uint8_t> rendered_image_src)
        {
           reference_image_buffer.map([&rendered_image_src, &all_channels_passed](std::span<const uint8_t> reference_image_src)
            {
                using ChannelType = typename PixelType::ElementType;

                std::span<const PixelType> rendered_image (reinterpret_cast<const PixelType*>(rendered_image_src.data()), rendered_image_src.size() / sizeof(PixelType));
                std::span<const PixelType> reference_image (reinterpret_cast<const PixelType*>(reference_image_src.data()), reference_image_src.size() / sizeof(PixelType));

                PixelType min_value (std::numeric_limits<ChannelType>::max());
                PixelType max_value (-std::numeric_limits<ChannelType>::max());

                PixelType total_sq_error;

                size_t valid_pixel_count = 0;

                for (const auto& [pixel_1, pixel_2]: std::views::zip(rendered_image, reference_image))
                {
                    if (!pbrlib::math::isfinite(pixel_1)) [[unlikely]]
                        continue;

                    if (!pbrlib::math::isfinite(pixel_2)) [[unlikely]]
                        continue;

                    const auto diff = pixel_1 - pixel_2;

                    total_sq_error += diff * diff;
                    ++valid_pixel_count;

                    min_value = pbrlib::math::min(min_value, pixel_2);
                    max_value = pbrlib::math::max(max_value, pixel_2);
                }

                if (valid_pixel_count == 0) [[unlikely]]
                {
                    all_channels_passed = false;
                    return ;
                }

                const auto mse = total_sq_error * (ChannelType(1.0) / static_cast<ChannelType>(valid_pixel_count));
                
                const auto max_i = max_value - min_value;

                constexpr auto eps              = 2.2204460492503131e-16;
                constexpr auto psnr_threshold   = 45.0;
                
                for (const auto i: std::views::iota(0u, PixelType::element_count))
                {
                    auto psnr_value = 100.0;
                    if (mse[i] > ChannelType(1e-10))
                        psnr_value = 20.0 * std::log10(static_cast<float>(max_i[i]) / (std::sqrt(static_cast<float>(mse[i]))) + static_cast<float>(eps));

                    if (psnr_value < psnr_threshold)
                        all_channels_passed = false;
                }
            }); 
        });

        return all_channels_passed;
    }
}

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

    bool ImageComparison::compare(backend::vk::Image& rendered_image, backend::vk::Image& reference_image)
    {
        if (rendered_image.width != reference_image.width || rendered_image.height != reference_image.height) [[unlikely]]
            return false;

        if (rendered_image.level_count != reference_image.level_count) [[unlikely]]
            return false;

        if (rendered_image.layer_count != reference_image.layer_count) [[unlikely]]
            return false;

        if (rendered_image.layout != VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) [[likely]]
            rendered_image.changeLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        if (reference_image.layout != VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) [[likely]]
            reference_image.changeLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        const VkDeviceSize group_count_x        = rendered_image.width / _device.workGroupSize();
        const VkDeviceSize group_count_y        = rendered_image.height / _device.workGroupSize();
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
            .view_handle            = rendered_image.view_handle,
            .sampler_handle         = _sampler_handle,
            .set_handle             = _descriptor_set_handle,
            .expected_image_layout  = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
            .binding                = 0
        });

        _device.writeDescriptorSet ({
            .view_handle            = reference_image.view_handle,
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

        auto command_buffer = _device.oneTimeSubmitCommandBuffer("vk-image-comparator");

        command_buffer.write([&rendered_image, this] (auto command_buffer_handle)
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

            const auto group_count_x = rendered_image.width / _device.workGroupSize();
            const auto group_count_y = rendered_image.height / _device.workGroupSize();

            vkCmdDispatch(command_buffer_handle, group_count_x, group_count_y, 1);
        }, "[vk-image-comparator] run-compare-images", backend::vk::marker_colors::compute_pipeline);

        _device.submit(command_buffer);

        rendered_image.changeLayout(VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
        reference_image.changeLayout(VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);

        if (rendered_image.format == VK_FORMAT_R32G32B32A32_SFLOAT)
            return psnr<pbrlib::math::vec4>(rendered_image, reference_image);
        
        if (rendered_image.format == VK_FORMAT_R16G16B16A16_SFLOAT)
            return psnr<math::half4>(rendered_image, reference_image);

        if (rendered_image.format == VK_FORMAT_R16_SFLOAT)
            return psnr<math::Scalar<pbrlib::backend::math::float16_t>>(rendered_image, reference_image);

        backend::log::error("[vk-image-comparator] undefined pixel format: {}", static_cast<uint64_t>(rendered_image.format));
        return true;
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
