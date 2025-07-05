#include <backend/renderer/frame_graph/frame_graph.hpp>

#include <backend/renderer/canvas.hpp>

#include <backend/renderer/vulkan/device.hpp>

#include <backend/renderer/frame_graph/compound_render_pass.hpp>
#include <backend/renderer/frame_graph/gbuffer_generator.hpp>
#include <backend/renderer/frame_graph/ssao.hpp>

#include <backend/logger/logger.hpp>

#include <backend/profiling.hpp>

#include <pbrlib/config.hpp>

#include <pbrlib/scene/scene.hpp>

#include <pbrlib/exceptions.hpp>

namespace pbrlib::backend
{
    FrameGraph::FrameGraph (
        vk::Device&         device, 
        Canvas&             canvas,
        MaterialManager&    material_manager,
        MeshManager&        mesh_manager
    ) :
        _device (device),
        _canvas (canvas)
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

        auto ptr_result = &_images.at(GBufferAttachmentsName::normal_tangent);

        ptr_result->changeLayout(VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
        _canvas.present(ptr_result);
    }
}

namespace pbrlib::backend
{
    std::unique_ptr<RenderPass> FrameGraph::buildGBufferGeneratorSubpass()
    {
        auto ptr_gbuffer_generator = std::make_unique<GBufferGenerator>();

        ptr_gbuffer_generator->addColorOutput(GBufferAttachmentsName::pos_uv, &_images.at(GBufferAttachmentsName::pos_uv));
        ptr_gbuffer_generator->addColorOutput(GBufferAttachmentsName::normal_tangent, &_images.at(GBufferAttachmentsName::normal_tangent));
        ptr_gbuffer_generator->addColorOutput(GBufferAttachmentsName::material_index, &_images.at(GBufferAttachmentsName::material_index));
        ptr_gbuffer_generator->depthStencil(&_depth_buffer.value());

        return ptr_gbuffer_generator;
    }

    std::unique_ptr<RenderPass> FrameGraph::buildSSAOSubpass (
        vk::Image*              ptr_pos_uv, 
        vk::Image*              ptr_normal_tangent, 
        VkPipelineStageFlags2   src_stage
    )
    {
        std::unique_ptr<RenderPass> ptr_ssao = std::make_unique<SSAO>();

        ptr_ssao->addColorOutput(SSAOOutputAttachmentsNames::result, &_images.at(SSAOOutputAttachmentsNames::result));

        ptr_ssao->addColorInput(SSAOInputAttachmentNames::pos_uv, ptr_pos_uv, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, src_stage, ptr_ssao->dstStage());
        ptr_ssao->addColorInput(SSAOInputAttachmentNames::normal_tangent, ptr_normal_tangent, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, src_stage, ptr_ssao->dstStage());

        return ptr_ssao;
    }

    void FrameGraph::build()
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        createResources();

        auto ptr_render_pass = std::make_unique<CompoundRenderPass>();

        auto ptr_gbuffer_generator = buildGBufferGeneratorSubpass();

        auto ptr_pos_uv         = &_images.at(GBufferAttachmentsName::pos_uv);
        auto ptr_normal_tangent = &_images.at(GBufferAttachmentsName::normal_tangent);

        auto ptr_ssao = buildSSAOSubpass(ptr_pos_uv, ptr_normal_tangent, ptr_gbuffer_generator->dstStage());
        
        ptr_render_pass->add(std::move(ptr_gbuffer_generator));
        ptr_render_pass->add(std::move(ptr_ssao));

        _ptr_render_pass = std::move(ptr_render_pass);

        _ptr_render_pass->init(_device, _render_context);
    }

    bool FrameGraph::rebuildPasses()
    {
        try
        {
            backend::log::info("[frame-graph] rebuild passes");
            return _ptr_render_pass ? _ptr_render_pass->rebuild() : false;
        } 
        catch (std::exception& ex)
        {
            backend::log::error("[frame-graph] failed rebuild: {}", ex.what());
        }

        return false;
    }

    void FrameGraph::createResources()
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        const auto [width, height] = _canvas.size();

        constexpr auto shared_usage_flags = 
                VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT 
            |   VK_IMAGE_USAGE_SAMPLED_BIT 
            |   VK_IMAGE_USAGE_TRANSFER_SRC_BIT
            |   VK_IMAGE_USAGE_STORAGE_BIT;

        _depth_buffer = vk::builders::Image(_device)
            .size(width, height)
            .format(VK_FORMAT_D32_SFLOAT)
            .usage(VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT)
            .addQueueFamilyIndex(_device.queue().family_index)
            .name("depth-buffer")
            .build();

        _images.emplace(
            GBufferAttachmentsName::pos_uv,
            vk::builders::Image(_device)
                .size(width, height)
                .format(VK_FORMAT_R32G32B32A32_SFLOAT)
                .usage(shared_usage_flags)
                .addQueueFamilyIndex(_device.queue().family_index)
                .name(GBufferAttachmentsName::pos_uv)
                .build()
        );
        
        _images.emplace(
            GBufferAttachmentsName::normal_tangent,
            vk::builders::Image(_device)
                .size(width, height)
                .format(VK_FORMAT_R32G32B32A32_SFLOAT)
                .usage(shared_usage_flags)
                .addQueueFamilyIndex(_device.queue().family_index)
                .name(GBufferAttachmentsName::normal_tangent)
                .build()
        );
        
        _images.emplace(
            GBufferAttachmentsName::material_index,
            vk::builders::Image(_device)
                .size(width, height)
                .format(VK_FORMAT_R16_UINT )
                .usage(shared_usage_flags)
                .addQueueFamilyIndex(_device.queue().family_index)
                .name(GBufferAttachmentsName::material_index)
                .build()
        );
        
        _images.emplace(
            SSAOOutputAttachmentsNames::result,
            vk::builders::Image(_device)
                .size(width, height)
                .format(VK_FORMAT_R16_SFLOAT)
                .usage(shared_usage_flags)
                .addQueueFamilyIndex(_device.queue().family_index)
                .name(SSAOOutputAttachmentsNames::result)
                .build()
        );
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
}