#pragma once

#include <backend/renderer/frame_graph/render_pass.hpp>

#include <vector>
#include <memory>

namespace pbrlib
{
    template<typename T>
    concept IsRenderPass = requires(T render_pass)
    {
        dynamic_cast<IRenderPass*>(&render_pass);
    };

    class CompoundRenderPass :
        public IRenderPass
    {
    public:
        template<IsRenderPass T>
        void add(std::unique_ptr<T>&& subpass)
        {
            _subpasses.push_back(std::forward(subpass));
        }

        bool init(const vk::Device* ptr_device) override;

        void render(vk::Image& result) override;

    private:
        std::vector<std::unique_ptr<IRenderPass>> _subpasses;
    };
}   