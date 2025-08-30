#include <backend/renderer/frame_graph/frame_graph.hpp>

#include <backend/renderer/canvas.hpp>

#include <backend/renderer/vulkan/device.hpp>

#include <backend/renderer/frame_graph/compound_render_pass.hpp>
#include <backend/renderer/frame_graph/gbuffer_generator.hpp>
#include <backend/renderer/frame_graph/ssao.hpp>

#include <backend/renderer/frame_graph/builders/ssao.hpp>

#include <backend/logger/logger.hpp>

#include <backend/profiling.hpp>

#include <pbrlib/config.hpp>

#include <pbrlib/scene/scene.hpp>

#include <pbrlib/exceptions.hpp>

namespace pbrlib::backend
{
    FrameGraph::FrameGraph (
        vk::Device&             device, 
        const pbrlib::Config&   config,
        Canvas&                 canvas,
        MaterialManager&        material_manager,
        MeshManager&            mesh_manager
    ) :
        _device (device),
        _canvas (canvas),
        _config (config)
    {
        _render_context.ptr_material_manager    = &material_manager;
        _render_context.ptr_mesh_manager        = &mesh_manager;
        
        build();
    }
}

namespace pbrlib::backend
{
    void FrameGraph::draw(const Camera& camera, std::span<const SceneItem*> items)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        if (!_ptr_render_pass)
        {
            log::error("[frame-graph] failed draw scene because render pass is empty");
            return ;
        }

        updatePerFrameData(camera, items);

        auto command_buffer = _device.oneTimeSubmitCommandBuffer("command-buffer-for-draw");

        _ptr_render_pass->draw(command_buffer);
        _device.submit(command_buffer);

        auto ptr_result = &_images.at(AttachmentsTraits<SSAO>::blur);

        ptr_result->changeLayout(VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
        _canvas.present(ptr_result);
    }
}

namespace pbrlib::backend
{
    std::unique_ptr<RenderPass> FrameGraph::buildGBufferGeneratorSubpass()
    {
        std::unique_ptr<RenderPass> ptr_gbuffer_generator = std::make_unique<GBufferGenerator>(_device);

        constexpr auto pos_uv           = AttachmentsTraits<GBufferGenerator>::pos_uv;
        constexpr auto normal_tangent   = AttachmentsTraits<GBufferGenerator>::normal_tangent;
        constexpr auto material_index   = AttachmentsTraits<GBufferGenerator>::material_index;

        ptr_gbuffer_generator->addColorOutput(pos_uv, &_images.at(pos_uv));
        ptr_gbuffer_generator->addColorOutput(normal_tangent, &_images.at(normal_tangent));
        ptr_gbuffer_generator->addColorOutput(material_index, &_images.at(material_index));
        ptr_gbuffer_generator->depthStencil(&_depth_buffer.value());

        ptr_gbuffer_generator->addSyncImage (
            &_depth_buffer.value(), 
            VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, 
            VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT , ptr_gbuffer_generator->srcStage()
        );

        return ptr_gbuffer_generator;
    }

    std::unique_ptr<RenderPass> FrameGraph::buildSSAOSubpass (
        vk::Image*              ptr_pos_uv, 
        vk::Image*              ptr_normal_tangent,
        vk::Image*              ptr_depth_buffer,
        const RenderPass*       ptr_gbuffer
    )
    {
        const auto src_stage = ptr_gbuffer->dstStage();

        const auto [gbuffer_set_handle, gbuffer_set_layout_handle] = ptr_gbuffer->resultDescriptorSet();

        return builders::SSAO(_device)
            .ssaoImage(_images.at(AttachmentsTraits<SSAO>::ssao))
            .blurImage(_images.at(AttachmentsTraits<SSAO>::blur))
            .settings(_config.ssao)            
            .addSync(ptr_pos_uv, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, src_stage)   
            .addSync(ptr_normal_tangent, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, src_stage)   
            .addSync(ptr_depth_buffer, VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL, src_stage)   
            .gbufferDescriptorSet(gbuffer_set_handle, gbuffer_set_layout_handle)
            .build();
    }

    void FrameGraph::build()
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        createResources();

        auto ptr_render_pass = std::make_unique<CompoundRenderPass>(_device);

        auto ptr_gbuffer_generator = buildGBufferGeneratorSubpass();

        auto ptr_pos_uv         = &_images.at(AttachmentsTraits<GBufferGenerator>::pos_uv);
        auto ptr_normal_tangent = &_images.at(AttachmentsTraits<GBufferGenerator>::normal_tangent);

        auto ptr_ssao = buildSSAOSubpass(
            ptr_pos_uv, 
            ptr_normal_tangent, 
            &_depth_buffer.value(),
            ptr_gbuffer_generator.get()
        );
        
        ptr_render_pass->add(std::move(ptr_gbuffer_generator));
        ptr_render_pass->add(std::move(ptr_ssao));

        _ptr_render_pass = std::move(ptr_render_pass);

        const auto [width, height] = _canvas.size();
        _ptr_render_pass->init(_render_context, width, height);
    }

    bool FrameGraph::rebuildPasses()
    {
        try
        {
            backend::log::info("[frame-graph] rebuild passes");
            
            const auto [width, height] = _canvas.size();

            return _ptr_render_pass ? _ptr_render_pass->rebuild(width, height) : false;
        } 
        catch (std::exception& ex)
        {
            backend::log::error("[frame-graph] failed rebuild: {}", ex.what());
        }

        return false;
    }

    template<HasAttachments T>
    void createRenderPassImages(vk::Device& device, auto& images, uint32_t width, uint32_t height)
    {
        for (const auto [name, format, usage]: AttachmentsTraits<T>::metadata())
        {
            images.emplace(
                name,
                vk::builders::Image(device)
                    .size(width, height)
                    .format(format)
                    .usage(usage)
                    .addQueueFamilyIndex(device.queue().family_index)
                    .name(name)
                    .build()
            );
        }
    }

    void FrameGraph::createResources()
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        const auto [width, height] = _canvas.size();

        _depth_buffer = vk::builders::Image(_device)
            .size(width, height)
            .format(VK_FORMAT_D32_SFLOAT)
            .usage(VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT)
            .addQueueFamilyIndex(_device.queue().family_index)
            .name("depth-buffer")
            .build();

        createRenderPassImages<GBufferGenerator>(_device, _images, width, height);
        createRenderPassImages<SSAO>(_device, _images, width, height);
    }
}

namespace pbrlib::backend
{
    void FrameGraph::updatePerFrameData(const Camera& camera, std::span<const SceneItem*> items)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        _render_context.items       = items;
        _render_context.projection  = camera.projection();
        _render_context.view        = camera.view();
    }

    void FrameGraph::update(const Config& config)
    {
        _config = config;
        if (_ptr_render_pass) [[likely]]
            _ptr_render_pass->update(config);
    }
}