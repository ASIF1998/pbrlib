#pragma once

#include <backend/renderer/vulkan/buffer.hpp>
#include <backend/renderer/vulkan/image.hpp>
#include <backend/renderer/vulkan/pipeline_layout.hpp>
#include <backend/renderer/vulkan/unique_handler.hpp>

#include <optional>

#include <filesystem>

namespace pbrlib::backend::vk
{
    class Device;
    class Image;
}

namespace pbrlib::testing
{
    class ImageComparison final
    {
        void generateImageDiff(backend::vk::Image& image_1, backend::vk::Image& image_2);

    public:
        explicit ImageComparison(backend::vk::Device& device);

        ImageComparison(ImageComparison&& image_comparison)         = delete;
        ImageComparison(const ImageComparison& image_comparison)    = delete;

        ImageComparison& operator = (ImageComparison&& image_comparison)        = delete;
        ImageComparison& operator = (const ImageComparison& image_comparison)   = delete;

        [[nodiscard]]
        bool compare(backend::vk::Image& rendered_image, backend::vk::Image& reference_image);

        [[nodiscard]]
        bool compare (
            backend::vk::Image&             image,
            const std::filesystem::path&    path_to_reference
        );

    private:
        backend::vk::Device& _device;

        backend::vk::PipelineLayoutHandle   _pipeline_layout_handle;
        backend::vk::PipelineHandle         _pipeline_handle;

        backend::vk::DescriptorSetLayoutHandle  _descriptor_set_layout_handle;
        backend::vk::DescriptorSetHandle        _descriptor_set_handle;

        backend::vk::SamplerHandle          _sampler_handle;
        std::optional<backend::vk::Image>   _images_diff;
    };
}
