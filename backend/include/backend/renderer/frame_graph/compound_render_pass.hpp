#pragma once

#include <backend/renderer/frame_graph/render_pass.hpp>

#include <vector>
#include <memory>

namespace pbrlib
{
    template<typename T>
    concept IsRenderPass = requires(T render_pass)
    {
        dynamic_cast<RenderPass*>(&render_pass);
    };

    class CompoundRenderPass :
        public RenderPass
    {
    public:
        template<IsRenderPass T>
        void add(std::unique_ptr<T>&& subpass)
        {
            _subpasses.push_back(std::forward(subpass));
        }

        bool init(const vk::Device* ptr_device) override;

        void render(const SceneItem* ptr_item, vk::CommandBuffer& command_buffer) override;

    private:
        std::vector<std::unique_ptr<RenderPass>> _subpasses;
    };
}   