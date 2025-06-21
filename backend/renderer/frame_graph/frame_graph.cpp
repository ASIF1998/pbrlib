#include <backend/renderer/frame_graph/frame_graph.hpp>

#include <backend/renderer/canvas.hpp>

#include <backend/renderer/vulkan/device.hpp>

#include <backend/renderer/frame_graph/gbuffer_generator.hpp>

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
        build();

        _render_context.ptr_material_manager    = &material_manager;
        _render_context.ptr_mesh_manager        = &mesh_manager;
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

        auto ptr_result = &_images.at(GBufferFinalAttachmentsName::pos_uv);

        ptr_result->changeLayout(VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
        _canvas.present(ptr_result);
    }
}

namespace pbrlib::backend
{
    void FrameGraph::build()
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        createResources();

        _ptr_render_pass = std::make_unique<GBufferGenerator>();

        _ptr_render_pass->addColorOutput(GBufferFinalAttachmentsName::pos_uv, &_images.at(GBufferFinalAttachmentsName::pos_uv));
        _ptr_render_pass->addColorOutput(GBufferFinalAttachmentsName::normal_tangent, &_images.at(GBufferFinalAttachmentsName::normal_tangent));
        _ptr_render_pass->addColorOutput(GBufferFinalAttachmentsName::material_index, &_images.at(GBufferFinalAttachmentsName::material_index));
        _ptr_render_pass->depthStencil(&_depth_buffer.value());

        if (!_ptr_render_pass->init(_device, _render_context))
            throw exception::InitializeError("[frame-graph] gbuffer-generator render pass");
    }

    bool FrameGraph::rebuildPasses()
    {
        backend::log::info("[frame-graph] rebuild passes");
        return _ptr_render_pass ? _ptr_render_pass->rebuild(_device, _render_context) : false;
    }

    void FrameGraph::createResources()
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        const auto [width, height] = _canvas.size();

        _depth_buffer = vk::Image::Builder(_device)
            .size(width, height)
            .format(VK_FORMAT_D32_SFLOAT)
            .usage(VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT)
            .addQueueFamilyIndex(_device.queue().family_index)
            .name("depth-buffer")
            .build();

        _images.emplace(
            GBufferFinalAttachmentsName::pos_uv,
            vk::Image::Builder(_device)
                .size(width, height)
                .format(VK_FORMAT_R32G32B32A32_SFLOAT)
                .usage(VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT)
                .addQueueFamilyIndex(_device.queue().family_index)
                .name(GBufferFinalAttachmentsName::pos_uv)
                .build()
        );
        
        _images.emplace(
            GBufferFinalAttachmentsName::normal_tangent,
            vk::Image::Builder(_device)
                .size(width, height)
                .format(VK_FORMAT_R32G32B32A32_SFLOAT)
                .usage(VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT)
                .addQueueFamilyIndex(_device.queue().family_index)
                .name(GBufferFinalAttachmentsName::normal_tangent)
                .build()
        );
        
        _images.emplace(
            GBufferFinalAttachmentsName::material_index,
            vk::Image::Builder(_device)
                .size(width, height)
                .format(VK_FORMAT_R16_UINT )
                .usage(VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT)
                .addQueueFamilyIndex(_device.queue().family_index)
                .name(GBufferFinalAttachmentsName::material_index)
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