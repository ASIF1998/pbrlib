#include <backend/renderer/frame_graph/render_pass.hpp>
#include <backend/renderer/vulkan/image.hpp>

#include <backend/logger/logger.hpp>

namespace pbrlib::backend
{
    vk::DescriptorGroupTransition* Transition::set(uint32_t set_id)
    {
        if (!has(set_id))
            return nullptr;

        return &_sets[set_id];
    }

    vk::DescriptorGroupTransition& Transition::addSet(uint32_t set_id)
    {
        if (has(set_id)) [[unlikely]]
            throw exception::InvalidArgument("[vk-descriptor-group-transition] add set id again");

        return _sets[set_id];
    }

    bool Transition::has(uint32_t set_id) const
    {
        return _sets.find(set_id) != std::end(_sets);
    }

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

    void RenderPass::addColorOutput(std::string_view name, vk::Image* ptr_image)
    {
        _color_output_images.emplace(name, ptr_image);
    }

    vk::Image* RenderPass::colorOutputAttach(std::string_view name)
    {
        auto it = _color_output_images.find(name);
        if (it == std::end(_color_output_images)) [[unlikely]]
            throw exception::InvalidState(std::format("[render-pass] failed find color output attachment '{}'", name));

        return it->second;
    }

    void RenderPass::depthStencil(vk::Image* ptr_image)
    {
        _ptr_depth_stencil_image = ptr_image;
    }

    vk::Image* RenderPass::depthStencil() noexcept
    {
        return _ptr_depth_stencil_image;
    }

    const vk::Image* RenderPass::depthStencil() const noexcept
    {
        return _ptr_depth_stencil_image;
    }

    void RenderPass::draw(vk::CommandBuffer& command_buffer)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        Transition transition;
        sync(transition);
        for (const auto [set_id, descriptor_group]: _descriptor_groups)
        {
            if (const auto ptr_set_transition = transition.set(set_id))
                descriptor_group->transition(command_buffer, *ptr_set_transition);
        }
        
        render(command_buffer);
    }

    vk::Device& RenderPass::device() noexcept
    {
        return *_ptr_device;
    }

    const RenderContext& RenderPass::context() const noexcept
    {
        return *_ptr_context;
    }

    std::pair<uint32_t, uint32_t> RenderPass::size() const noexcept
    {
        return std::make_pair(_width, _height);
    }

    void RenderPass::descriptorGroup(uint32_t set_id, const vk::DescriptorGroup& descriptor_group)
    {
        _descriptor_groups.emplace(set_id, &descriptor_group);
    }

    const vk::DescriptorGroup* RenderPass::descriptorGroup(uint32_t set_id) const
    {
        if (const auto it = _descriptor_groups.find(set_id); it != std::end(_descriptor_groups)) [[likely]]
            return it->second;

        return nullptr;
    }
}
