#pragma once

#include <backend/renderer/vulkan/buffer.hpp>

#include <backend/renderer/vulkan/pipeline_layout.hpp>

#include <vulkan/vulkan.h>

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

        VkPipeline          _pipeline_handle        = VK_NULL_HANDLE;
        VkPipelineLayout    _pipeline_layout_handle = VK_NULL_HANDLE;

        VkDescriptorSet         _descriptor_set_handle          = VK_NULL_HANDLE;
        VkDescriptorSetLayout   _descriptor_set_layout_handle   = VK_NULL_HANDLE;

        VkSampler _sampler_handle = VK_NULL_HANDLE;

        std::optional<backend::vk::Buffer> _count_changed_pixels_buffer;
    };
}