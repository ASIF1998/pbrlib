#pragma once

#include <backend/renderer/frame_graph/render_pass.hpp>

#include <backend/renderer/vulkan/image.hpp>

#include <pbrlib/config.hpp>

#include <pbrlib/camera.hpp>

#include <string>

#include <memory>
#include <map>
#include <optional>

namespace pbrlib::testing
{
    class FrameGraphResourcesGetter;
}

namespace pbrlib::backend
{
    class Canvas;
    class MaterialManager;
    class MeshManager;
}

namespace pbrlib::backend
{
    class FrameGraph final
    {
        friend class testing::FrameGraphResourcesGetter;

        void createResources();

        void build();

        std::unique_ptr<RenderPass> buildGBufferGeneratorSubpass();

        std::unique_ptr<RenderPass> buildSSAOSubpass (
            vk::Image*              ptr_pos_uv, 
            vk::Image*              ptr_normal_tangent,
            vk::Image*              ptr_depth_buffer,
            const RenderPass*       ptr_gbuffer
        );

        void updatePerFrameData(const Camera& camera, std::span<const SceneItem*> items);

    public:
        explicit FrameGraph (
            vk::Device&             device, 
            const pbrlib::Config&   config,
            Canvas&                 canvas,
            MaterialManager&        material_manager,
            MeshManager&            mesh_manager
        );

        FrameGraph(FrameGraph&& frame_graph)        = delete;
        FrameGraph(const FrameGraph& frame_graph)   = delete;

        FrameGraph& operator = (FrameGraph&& frame_graph)       = delete;
        FrameGraph& operator = (const FrameGraph& frame_graph)  = delete;

        void draw(const Camera& camera, std::span<const SceneItem*> items);

        bool rebuildPasses();

        void update(const Config& config);

    private:
        vk::Device& _device;
        Canvas&     _canvas;
        
        pbrlib::Config _config;

        std::unique_ptr<RenderPass> _ptr_render_pass;

        std::map<std::string, vk::Image, std::less<void>> _images;

        std::optional<vk::Image> _depth_buffer;

        RenderContext _render_context;
    };
}