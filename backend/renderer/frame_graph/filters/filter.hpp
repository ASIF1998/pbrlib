#pragma once

#include <backend/renderer/frame_graph/render_pass.hpp>
#include <backend/renderer/vulkan/unique_handler.hpp>

#include <string>
#include <string_view>

namespace pbrlib::backend::vk
{
    class Image;
}

namespace pbrlib::backend
{
    class Filter :
        public RenderPass
    {
    public:
        explicit Filter(std::string_view name, vk::Device& device, vk::Image& dst_image) noexcept;

        void apply(vk::Image& image);

        [[nodiscard]] vk::Image& srcImage();
        [[nodiscard]] vk::Image& dstImage() noexcept;

        [[nodiscard]] std::pair<VkDescriptorSet, VkDescriptorSetLayout> IODescriptorSet() noexcept;

    protected:
        void dispatchCompute(VkCommandBuffer command_buffer_handle);

    private:
        std::string _name;

        vk::Image* _ptr_dst_image = nullptr;
        vk::Image* _ptr_src_image = nullptr;

        vk::DescriptorSetLayoutHandle   _io_descriptor_set_layout_handle;
        vk::DescriptorSetHandle         _io_descriptor_set_handle;

        vk::SamplerHandle _input_image_sampler_handle;
    };
}
