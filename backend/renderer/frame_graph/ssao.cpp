#include <backend/renderer/frame_graph/ssao.hpp>

namespace pbrlib::backend
{
    bool SSAO::init(vk::Device& device, const RenderContext& context)
    {
        return true;
    }

    bool SSAO::rebuild(vk::Device& device, const RenderContext& context)
    {
        return true;
    }

    void SSAO::render(size_t item_id, vk::CommandBuffer& command_buffer)
    {

    }

    void SSAO::prePass(vk::CommandBuffer& command_buffer)
    {

    }

    void SSAO::postPass(vk::CommandBuffer& command_buffer)
    {
        
    }
}