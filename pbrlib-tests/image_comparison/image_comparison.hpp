#pragma once

#include <backend/renderer/vulkan/buffer.hpp>
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
    public:
        explicit ImageComparison(pbrlib::backend::vk::Device& device);

        ~ImageComparison();

        [[nodiscard]]
        bool compare(const backend::vk::Image& image_1, const backend::vk::Image& image_2);

        [[nodiscard]]
        bool compare (
            const backend::vk::Image&       image_1,
            const std::filesystem::path&    path_to_reference
        );

    private:
        backend::vk::Device& _device;

        backend::vk::PipelineLayoutHandle   _pipeline_layout_handle;
        VkPipeline                          _pipeline_handle        = VK_NULL_HANDLE;

        backend::vk::DescriptorSetLayoutHandle  _descriptor_set_layout_handle;
        backend::vk::DescriptorSetHandle        _descriptor_set_handle;

        VkSampler _sampler_handle = VK_NULL_HANDLE;

        std::optional<backend::vk::Buffer> _count_changed_pixels_buffer;
    };
}