#include <backend/renderer/frame_graph/frame_graph.hpp>

#include <backend/renderer/canvas.hpp>

#include <backend/renderer/vulkan/device.hpp>

#include <backend/renderer/frame_graph/compound_render_pass.hpp>
#include <backend/renderer/frame_graph/gbuffer_generator.hpp>
#include <backend/renderer/frame_graph/ssao.hpp>

#include <backend/renderer/frame_graph/builders/ssao.hpp>
#include <backend/renderer/frame_graph/builders/gbuffer_generator.hpp>

#include <backend/renderer/frame_graph/filters/fxaa.hpp>

#include <backend/renderer/vulkan/gpu_marker_colors.hpp>

#include <backend/logger/logger.hpp>

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

        clearImages(command_buffer);

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
        auto ptr_pos_uv_image           = &_images.at(AttachmentsTraits<GBufferGenerator>::pos_uv);
        auto ptr_nor_tan_image          = &_images.at(AttachmentsTraits<GBufferGenerator>::normal_tangent);
        auto ptr_mat_index_image        = &_images.at(AttachmentsTraits<GBufferGenerator>::material_index);
        auto ptr_depth_stencil_image    = &_depth_buffer.value();

        return builders::GBufferGenerator(_device)
            .posUvImage(*ptr_pos_uv_image)
            .normalTangentImage(*ptr_nor_tan_image)
            .materialIndexImage(*ptr_mat_index_image)
            .depthStencilImage(*ptr_depth_stencil_image)
            .build();
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

    void FrameGraph::setupAA(CompoundRenderPass& compound_render_pass, vk::Image& image, settings::AA aa)
    {
        if (aa == settings::AA::eNone)
            return ;

        if (aa == settings::AA::eFXAA)
        {
            std::unique_ptr<Filter> ptr_fxaa = std::make_unique<FXAA>(_device);
            ptr_fxaa->apply(image);

            ptr_fxaa->addSyncImage (
                &image, 
                VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, 
                VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT, VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT
            );

            compound_render_pass.add(std::move(ptr_fxaa));
        }
    }

    void FrameGraph::build()
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        createResources();

        auto ptr_render_pass = std::make_unique<CompoundRenderPass>(_device);

        auto ptr_gbuffer_generator = buildGBufferGeneratorSubpass();

        auto ptr_pos_uv         = &_images.at(AttachmentsTraits<GBufferGenerator>::pos_uv);
        auto ptr_normal_tangent = &_images.at(AttachmentsTraits<GBufferGenerator>::normal_tangent);

        auto ptr_ssao = buildSSAOSubpass (
            ptr_pos_uv, 
            ptr_normal_tangent, 
            &_depth_buffer.value(),
            ptr_gbuffer_generator.get()
        );
        
        ptr_render_pass->add(std::move(ptr_gbuffer_generator));
        ptr_render_pass->add(std::move(ptr_ssao));

        setupAA(*ptr_render_pass, _images.at(AttachmentsTraits<SSAO>::blur), _config.aa);

        _ptr_render_pass = std::move(ptr_render_pass);

        const auto [width, height] = _canvas.size();
        _ptr_render_pass->init(_render_context, width, height);
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
        createRenderPassImages<FXAA>(_device, _images, width, height);
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

namespace pbrlib::backend
{
    void FrameGraph::clearImages(vk::CommandBuffer& command_buffer)
    {
        for (auto& [_, image]: _images)
            image.changeLayout(command_buffer, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

        command_buffer.write([this] (VkCommandBuffer command_buffer_handle)
        {
            constexpr VkClearColorValue clear_color = {0.0, 0.0, 0.0, 0.0};

            constexpr VkImageSubresourceRange range
            {
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .levelCount = 1,
                .layerCount = 1
            };

            for (auto& [_, image]: _images)
            {
                vkCmdClearColorImage(
                    command_buffer_handle, 
                    image.handle.handle(), image.layout, 
                    &clear_color, 
                    1, &range
                );
            }
        }, "[frame-graph] clear-images", vk::marker_colors::clear);
    }
}