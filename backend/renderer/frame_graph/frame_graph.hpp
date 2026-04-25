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
    class CompoundRenderPass;
}

namespace pbrlib::backend
{
    class FrameGraph final
    {
        friend class testing::FrameGraphResourcesGetter;

        using RenderPassesImages = std::map <
            std::string, 
            vk::Image, 
            std::less<void>
        >;

        void createResources(uint32_t width, uint32_t height);
        void initFrameSync();

        void build(uint32_t width, uint32_t height);

        std::unique_ptr<RenderPass> buildGBufferGeneratorSubpass();

        std::unique_ptr<RenderPass> buildSSAOSubpass (
            vk::Image*              ptr_pos_uv,
            vk::Image*              ptr_normal_tangent,
            vk::Image*              ptr_depth_buffer,
            const RenderPass*       ptr_gbuffer
        );

        void setupAA(CompoundRenderPass& compound_render_pass, vk::Image& image, settings::AA aa);

        void updatePerFrameData(const Camera& camera, std::span<const SceneItem*> items);

        void clearImages(vk::CommandBuffer& command_buffer);

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

        void preRenderCallback(const std::function<void()>& callback);
        void postRenderCallback(const std::function<void()>& callback);
        void presentToDisplayCallback(const std::function<void()>& callback);

    private:
        vk::Device& _device;
        Canvas&     _canvas;

        pbrlib::Config _config;

        std::unique_ptr<RenderPass> _ptr_render_pass;

        RenderPassesImages          _render_passes_images;
        std::optional<vk::Image>    _depth_buffer;

        std::vector<vk::SemaphoreHandle>    _image_available_semaphores;
        std::vector<vk::SemaphoreHandle>    _render_finished_semaphores;
        std::vector<vk::FenceHandle>        _in_flight_fences;

        RenderContext _render_context;

        std::function<void()> _pre_render_callback;
        std::function<void()> _post_render_callback;
        std::function<void()> _present_to_display_callback;
    };
}
