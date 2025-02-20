#include <backend/renderer/frame_graph/render_pass.hpp>

#include <backend/logger/logger.hpp>

#include <stdexcept>

namespace pbrlib
{
    void RenderPass::addColorOutput(std::string_view name, const vk::Image* ptr_image)
    {
        _color_output_images.emplace(name, ptr_image);
    }

    const vk::Image* RenderPass::colorOutputAttach(std::string_view name)
    {
        auto it = _color_output_images.find(name);
        if (it == std::end(_color_output_images))
            throw std::runtime_error(std::format("[RenderPass] Failed find color output attachment '{}'", name));

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
}