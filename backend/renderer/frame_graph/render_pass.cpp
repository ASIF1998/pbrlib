#include <backend/renderer/frame_graph/render_pass.hpp>

#include <backend/renderer/vulkan/image.hpp>

#include <backend/logger/logger.hpp>

#include <backend/profiling.hpp>

#include <stdexcept>

#include <ranges>

namespace pbrlib::backend
{
    RenderPass::RenderPass(vk::Device& device) noexcept :
        _ptr_device (&device)
    { }

    bool RenderPass::init(const RenderContext& context, uint32_t width, uint32_t height)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        _ptr_context = &context;

        _width  = width;
        _height = height;

        return true;
    }

    void RenderPass::addColorInput (
        std::string_view        name, 
        vk::Image*              ptr_image, 
        VkImageLayout           new_layout, 
        VkPipelineStageFlags2   src_stage, 
        VkPipelineStageFlags2   dst_stage
    )
    {
        _color_input_images.emplace(name, std::make_tuple(ptr_image, new_layout, src_stage, dst_stage));
    }

    const vk::Image* RenderPass::colorInputAttach(std::string_view name) const
    {
        auto it = _color_input_images.find(name);
        if (it == std::end(_color_input_images))
            throw exception::InvalidState(std::format("[render-pass] failed find color input '{}'", name));

        return std::get<vk::Image*>(it->second);
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

    void RenderPass::draw(vk::CommandBuffer& command_buffer)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        sync(command_buffer);
        render(command_buffer);
    }

    void RenderPass::sync(vk::CommandBuffer& command_buffer)
    {
        for (auto& [name, sync_data]: _color_input_images)
        {
            auto [ptr_image, new_layout, src_stage, dst_stage] = sync_data;
            ptr_image->changeLayout(command_buffer, new_layout, src_stage, dst_stage);
        }
    }

    void RenderPass::descriptorSet(uint32_t set_id, VkDescriptorSet set_handle, VkDescriptorSetLayout set_layout)
    { 
        _input_descriptor_sets.emplace(set_id, std::make_pair(set_handle, set_layout));
    }

    std::pair<VkDescriptorSet, VkDescriptorSetLayout> RenderPass::descriptorSet(uint32_t set_id) const
    {
        if (auto res = _input_descriptor_sets.find(set_id); res != std::end(_input_descriptor_sets))
            return  res->second;

        throw exception::RuntimeError("[render-pass] failed find input descriptor set");
    }
}