#include <backend/renderer/frame_graph/render_pass.hpp>

#include <backend/renderer/vulkan/image.hpp>

#include <backend/logger/logger.hpp>

#include <backend/profiling.hpp>

#include <stdexcept>

#include <ranges>

namespace pbrlib::backend
{
    bool RenderPass::init(vk::Device& device, const RenderContext& context)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        _ptr_device     = &device;
        _ptr_context    = &context;

        return true;
    }

    void RenderPass::addColorInput(vk::Image* ptr_image, VkImageLayout new_layout, VkPipelineStageFlags2 src_stage, VkPipelineStageFlags2 dst_stage)
    {
        _color_input_images.push_back(std::make_tuple(ptr_image, new_layout, src_stage, dst_stage));
    }

    void RenderPass::addColorOutput(std::string_view name, vk::Image* ptr_image)
    {
        _color_output_images.emplace(name, ptr_image);
    }

    vk::Image* RenderPass::colorOutputAttach(std::string_view name)
    {
        auto it = _color_output_images.find(name);
        if (it == std::end(_color_output_images))
            throw exception::InvalidState(std::format("[render-pass] failed find color output attachment '{}'", name));

        return it->second;
    }

    void RenderPass::depthStencil(const vk::Image* ptr_image)
    {
        _ptr_depth_stencil_image = ptr_image;
    }

    const vk::Image* RenderPass::depthStencil() const noexcept
    {
        return _ptr_depth_stencil_image;
    }

    void RenderPass::prePass(vk::CommandBuffer& command_buffer)
    { }

    void RenderPass::postPass(vk::CommandBuffer& command_buffer)
    { }

    void RenderPass::draw(vk::CommandBuffer& command_buffer)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        sync(command_buffer);
        prePass(command_buffer);

        for (const auto item_id: std::views::iota(0u, _ptr_context->items.size()))
            render(item_id, command_buffer);

        postPass(command_buffer);
    }

    void RenderPass::sync(vk::CommandBuffer& command_buffer)
    {
        for (auto [ptr_image, new_layout, src_stage, dst_stage]: _color_input_images)
            ptr_image->changeLayout(command_buffer, new_layout, src_stage, dst_stage);
    }
}