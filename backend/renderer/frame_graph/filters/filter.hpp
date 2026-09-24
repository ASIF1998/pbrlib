#pragma once

#include <backend/renderer/frame_graph/render_pass.hpp>
#include <backend/renderer/vulkan/descriptor_group.hpp>

#include <string>
#include <string_view>

#include <optional>

namespace pbrlib::backend::vk
{
    class Image;
}

namespace pbrlib::backend
{
    class Filter :
        public RenderPass
    {
        void sync(Transition& transition) override;

    public:
        explicit Filter(std::string_view name, vk::Device& device, vk::Image& dst_image);

        void apply(vk::Image& image);

        [[nodiscard]] vk::Image& srcImage();
        [[nodiscard]] vk::Image& dstImage() noexcept;

        [[nodiscard]] vk::DescriptorGroup*          IODescriptorGroup() noexcept;
        [[nodiscard]] const vk::DescriptorGroup*    IODescriptorGroup() const noexcept;

    protected:
        void dispatchCompute(VkCommandBuffer command_buffer_handle);

    private:
        std::string _name;

        vk::Image* _ptr_dst_image = nullptr;
        vk::Image* _ptr_src_image = nullptr;

        std::optional<vk::DescriptorGroup> _io_descriptor_group;

        vk::SamplerHandle _input_image_sampler_handle;
    };
}
