#include <backend/renderer/frame_graph/render_pass.hpp>

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
    {}

    void RenderPass::postPass(vk::CommandBuffer& command_buffer)
    { }

    void RenderPass::draw(vk::CommandBuffer& command_buffer)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        prePass(command_buffer);

        for (const auto item_id: std::views::iota(0u, _ptr_context->items.size()))
            render(item_id, command_buffer);

        postPass(command_buffer);
    }
}