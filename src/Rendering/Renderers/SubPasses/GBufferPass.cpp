//
//  GBufferPass.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 20/06/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <pbrlib/Rendering/Renderers/SubPasses/GBufferPass.hpp>

#include <pbrlib/Rendering/VulkanWrapper/Device.hpp>
#include <pbrlib/Rendering/VulkanWrapper/PhysicalDevice.hpp>

#include <pbrlib/Rendering/VulkanWrapper/ShaderModule.hpp>

#include <pbrlib/Rendering/VulkanWrapper/DescriptorSet.hpp>
#include <pbrlib/Rendering/VulkanWrapper/GraphicsPipeline.hpp>
#include <pbrlib/Rendering/VulkanWrapper/RenderPass.hpp>
#include <pbrlib/Rendering/VulkanWrapper/Framebuffer.hpp>
#include <pbrlib/Rendering/VulkanWrapper/Sampler.hpp>

#include <pbrlib/Rendering/VulkanWrapper/CommandBuffer.hpp>

#include <pbrlib/Rendering/Geometry/Mesh.hpp>

#include <pbrlib/Rendering/Material/Material.hpp>

#include <pbrlib/Rendering/Renderers/Shaders/MaterialData.h>

#include <pbrlib/Rendering/Renderers/SubPasses/spv/GBuffer.glsl.vert.spv.h>
#include <pbrlib/Rendering/Renderers/SubPasses/spv/GBuffer.glsl.frag.spv.h>

namespace pbrlib
{
    /**
     * @struct UniformBufferMatrices.
     * @brief Структура, описывающая uniform-буфер с необходимыми матрицами для прохода g-буфера.
    */
    struct UniformBufferMatrices
    {
        Matrix4x4<float> MVP;
        Matrix4x4<float> model_view_matrix;
        Matrix4x4<float> normal_matrix;
    };
    
    struct GBufferPassBindings
    {
        enum
        {
            MatricesData = 0,
            Albedo,
            NormalMap,
            Metallic,
            Roughness,
            AO,
            MaterailData
        };
    };

    struct GBufferPassLocations
    {
        enum
        {
            Position = 0,
            Normal,
            Tangent,
            UV
        };
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void GBufferPass::Builder::setDevice(const PtrDevice& ptr_device)
    {
        _ptr_device = ptr_device;
    }

    void GBufferPass::Builder::setPhysicalDevice(const PtrPhysicalDevice& ptr_physical_device)
    {
        _ptr_physical_device = ptr_physical_device;
    }

    void GBufferPass::Builder::setDescriptorPool(const PtrDescriptorPool& ptr_descriptor_pool)
    {
       _ptr_descriptor_pool = ptr_descriptor_pool;
    }

    void GBufferPass::Builder::setDeviceMemoryIndex(uint32_t memory_index)
    {
        _gpu_memory_index = memory_index;
    }

    void GBufferPass::Builder::windowSize(uint32_t width, uint32_t height)
    {
        _window_width   = width;
        _window_height  = height;
    }

    void GBufferPass::Builder::setQueue(const PtrDeviceQueue& ptr_queue) 
    {
        _ptr_device_queue = ptr_queue;
    }

    GBufferPass GBufferPass::Builder::build()
    {
        return GBufferPass(
            _ptr_device, _ptr_device_queue, _ptr_physical_device,
            _ptr_descriptor_pool,
            _gpu_memory_index,
            _window_width, _window_height
        );
    }
    
    PtrGBufferPass GBufferPass::Builder::buildPtr()
    {
        return GBufferPass::make(
            _ptr_device, _ptr_device_queue, _ptr_physical_device,
            _ptr_descriptor_pool, 
            _gpu_memory_index,
            _window_width, _window_height
        );
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PtrRenderPass makeRenderPass(const PtrDevice& ptr_device, const PtrAttachments& image_view)
    {
        RenderPass::Builder build_render_pass       (GBufferPass::OutputImagesViewsIDs::Count, 1, 0);
        SubpassDescription  subpass_description1    (0, 4, 0);

        subpass_description1.addColorAttachment(GBufferPass::OutputImagesViewsIDs::PositionAndMetallic, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
        subpass_description1.addColorAttachment(GBufferPass::OutputImagesViewsIDs::NormalAndRoughness, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
        subpass_description1.addColorAttachment(GBufferPass::OutputImagesViewsIDs::AlbedoAndBakedAO, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
        subpass_description1.addColorAttachment(GBufferPass::OutputImagesViewsIDs::TangentAndAnisotropy, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
        subpass_description1.setDepthStencilAttachment(GBufferPass::OutputImagesViewsIDs::Depth, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

        build_render_pass.setDevice(ptr_device);
        
        build_render_pass.addAttachmentDescription(
            image_view->at(GBufferPass::OutputImagesViewsIDs::PositionAndMetallic).getFormat(),
            VK_SAMPLE_COUNT_1_BIT,
            VK_ATTACHMENT_LOAD_OP_LOAD,
            VK_ATTACHMENT_STORE_OP_STORE,
            VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            VK_ATTACHMENT_STORE_OP_DONT_CARE,
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        );

        build_render_pass.addAttachmentDescription(
            image_view->at(GBufferPass::OutputImagesViewsIDs::NormalAndRoughness).getFormat(),
            VK_SAMPLE_COUNT_1_BIT,
            VK_ATTACHMENT_LOAD_OP_LOAD,
            VK_ATTACHMENT_STORE_OP_STORE,
            VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            VK_ATTACHMENT_STORE_OP_DONT_CARE,
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        );

        build_render_pass.addAttachmentDescription(
            image_view->at(GBufferPass::OutputImagesViewsIDs::AlbedoAndBakedAO).getFormat(),
            VK_SAMPLE_COUNT_1_BIT,
            VK_ATTACHMENT_LOAD_OP_LOAD,
            VK_ATTACHMENT_STORE_OP_STORE,
            VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            VK_ATTACHMENT_STORE_OP_DONT_CARE,
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        );

        build_render_pass.addAttachmentDescription(
           image_view->at(GBufferPass::OutputImagesViewsIDs::Depth).getFormat(),
            VK_SAMPLE_COUNT_1_BIT,
            VK_ATTACHMENT_LOAD_OP_LOAD,
            VK_ATTACHMENT_STORE_OP_STORE,
            VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            VK_ATTACHMENT_STORE_OP_DONT_CARE,
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
        );
        
        build_render_pass.addAttachmentDescription(
            image_view->at(GBufferPass::OutputImagesViewsIDs::TangentAndAnisotropy).getFormat(),
            VK_SAMPLE_COUNT_1_BIT,
            VK_ATTACHMENT_LOAD_OP_LOAD,
            VK_ATTACHMENT_STORE_OP_STORE,
            VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            VK_ATTACHMENT_STORE_OP_DONT_CARE,
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        );
    
        build_render_pass.addSubpassDescription(move(subpass_description1));

        return build_render_pass.buildPtr();
    }

    PtrAttachments makeFramebufferAttachments(
        const PtrDevice&            ptr_device,
        const PtrPhysicalDevice&    ptr_physical_device,
        uint32_t                    gpu_queue_family_index,
        uint32_t                    width,
        uint32_t                    height
    )
    {
        PtrAttachments ptr_framebuffer_attachments (new vector<ImageView>());

        Image::Builder<Image::TexelType::RGBA, float, Image::NumBits::NB32>         image_rgba_builder;
        ImageView::Builder                                                          image_view_builder;
        Image::Builder<Image::TexelType::DepthStencil, float, Image::NumBits::NB32> depth_image_builder;

        image_rgba_builder.setDevice(ptr_device);
        image_rgba_builder.setExtend(width, height, 1);
        image_rgba_builder.setImageType(VK_IMAGE_TYPE_2D);
        image_rgba_builder.setMemoryTypeIndex(ptr_physical_device->memory.getMemoryType(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT));
        image_rgba_builder.setNumArrayLayers(1);
        image_rgba_builder.setNumMipLevels(1);
        image_rgba_builder.setNumSamples(VK_SAMPLE_COUNT_1_BIT);
        image_rgba_builder.setTiling(VK_IMAGE_TILING_OPTIMAL);
        image_rgba_builder.setUsage(VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT);
        image_rgba_builder.addQueueFamilyIndex(gpu_queue_family_index);
        
        image_view_builder.setAspectMask(VK_IMAGE_ASPECT_COLOR_BIT);
        image_view_builder.setBaseArrayLayer(0);
        image_view_builder.setBaseMipLevel(0);
        image_view_builder.setFormat(VK_FORMAT_R32G32B32A32_SFLOAT);
        image_view_builder.setImage(image_rgba_builder.buildPtr());
        image_view_builder.setLayerCount(1);
        image_view_builder.setLevelCount(1);
        image_view_builder.setType(VK_IMAGE_VIEW_TYPE_2D);

        depth_image_builder.setDevice(ptr_device);
        depth_image_builder.setExtend(width, height, 1);
        depth_image_builder.setImageType(VK_IMAGE_TYPE_2D);
        depth_image_builder.setMemoryTypeIndex(ptr_physical_device->memory.getMemoryType(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT));
        depth_image_builder.setNumArrayLayers(1);
        depth_image_builder.setNumMipLevels(1);
        depth_image_builder.setNumSamples(VK_SAMPLE_COUNT_1_BIT);
        depth_image_builder.setTiling(VK_IMAGE_TILING_OPTIMAL);
        depth_image_builder.setUsage(VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT);
        depth_image_builder.addQueueFamilyIndex(gpu_queue_family_index);

        ptr_framebuffer_attachments->push_back(image_view_builder.build());
        
        image_view_builder.setImage(image_rgba_builder.buildPtr());
        ptr_framebuffer_attachments->push_back(image_view_builder.build());

        image_view_builder.setImage(image_rgba_builder.buildPtr());
        ptr_framebuffer_attachments->push_back(image_view_builder.build());
        
        image_view_builder.setImage(depth_image_builder.buildPtr());
        image_view_builder.setFormat(VK_FORMAT_D32_SFLOAT_S8_UINT);
        image_view_builder.setAspectMask(VK_IMAGE_ASPECT_DEPTH_BIT);
        ptr_framebuffer_attachments->push_back(image_view_builder.build());

        image_view_builder.setImage(image_rgba_builder.buildPtr());
        image_view_builder.setFormat(VK_FORMAT_R32G32B32A32_SFLOAT);
        image_view_builder.setAspectMask(VK_IMAGE_ASPECT_COLOR_BIT);
        ptr_framebuffer_attachments->push_back(image_view_builder.build());
        
        return ptr_framebuffer_attachments;
    }

    PtrFramebuffer makeFramebuffer(
        const PtrAttachments&   ptr_framebuffer_attachments,
        const PtrRenderPass&    ptr_render_pass,
        uint32_t                width,
        uint32_t                height
    )
    {
        Framebuffer::Builder    build_framebuffer;
        PtrFramebuffer  framebuffer;

        build_framebuffer.setAttachments(ptr_framebuffer_attachments);
        build_framebuffer.setRenderPass(ptr_render_pass);
        build_framebuffer.setWidth(width);
        build_framebuffer.setHeight(height);
        build_framebuffer.setNumLayers(1);

        return build_framebuffer.buildPtr();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    GBufferPass::GBufferPass(
        const PtrDevice&            ptr_device, 
        const PtrDeviceQueue&       ptr_queue,
        const PtrPhysicalDevice&    ptr_physical_device,
        const PtrDescriptorPool&    ptr_descriptor_pool,  
        uint32_t                    gpu_memory_index,
        uint32_t                    window_width,
        uint32_t                    window_height
    ) :
        _ptr_device_queue (ptr_queue)
    {
        assert(ptr_queue);

        uint32_t queue_family_index = _ptr_device_queue->getFamilyIndex();

        ShaderModule::Builder build_vert_shader;
        ShaderModule::Builder build_frag_shader;
        
        constexpr uint32_t subpass_index = 0;

        GraphicsPipeline::Builder   build_graphics_pipline  (1, 4, 1, 1, 4);

        Buffer::Builder<UniformBufferMatrices>  build_uniform_matrices_data_buffer;
        Buffer::Builder<MaterialData>           build_uniform_material_data_buffer;
        
        DescriptorSetLayoutBindings descriptor_set_layout_bindings (ptr_device, 0);

        _ptr_framebuffer_attachments    = makeFramebufferAttachments(ptr_device, ptr_physical_device, queue_family_index, window_width, window_height);
        _ptr_render_pass                = makeRenderPass(ptr_device, _ptr_framebuffer_attachments);
        _ptr_framebuffer                = makeFramebuffer(_ptr_framebuffer_attachments, _ptr_render_pass, window_width, window_height);

        descriptor_set_layout_bindings.addBinding(GBufferPassBindings::MatricesData, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT);
        descriptor_set_layout_bindings.addBinding(GBufferPassBindings::Albedo, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT);
        descriptor_set_layout_bindings.addBinding(GBufferPassBindings::NormalMap, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT);
        descriptor_set_layout_bindings.addBinding(GBufferPassBindings::Metallic, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT);
        descriptor_set_layout_bindings.addBinding(GBufferPassBindings::Roughness, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT);
        descriptor_set_layout_bindings.addBinding(GBufferPassBindings::AO, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT);
        descriptor_set_layout_bindings.addBinding(GBufferPassBindings::MaterailData, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT);

        PtrDescriptorSetLayout ptr_descriptor_set_layout = DescriptorSetLayout::make(move(descriptor_set_layout_bindings));

        _ptr_descriptor_set = DescriptorSet::make(ptr_descriptor_pool, ptr_descriptor_set_layout);

        build_vert_shader.setDevice(ptr_device);
        build_vert_shader.setShaderCode(reinterpret_cast<const uint32_t*>(gbuffer_vert_spirv), sizeof(gbuffer_vert_spirv));
        build_vert_shader.setShaderType(VK_SHADER_STAGE_VERTEX_BIT);
        
        build_frag_shader.setDevice(ptr_device);
        build_frag_shader.setShaderCode(reinterpret_cast<const uint32_t*>(gbuffer_frag_spirv), sizeof(gbuffer_frag_spirv));
        build_frag_shader.setShaderType(VK_SHADER_STAGE_FRAGMENT_BIT);

        build_uniform_matrices_data_buffer.setDevice(ptr_device);
        build_uniform_matrices_data_buffer.setMemoryTypeIndex(gpu_memory_index);
        build_uniform_matrices_data_buffer.setUsage(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
        build_uniform_matrices_data_buffer.setSize(1);
        build_uniform_matrices_data_buffer.addQueueFamily(queue_family_index);

        build_uniform_material_data_buffer.setDevice(ptr_device);
        build_uniform_material_data_buffer.setMemoryTypeIndex(gpu_memory_index);
        build_uniform_material_data_buffer.setUsage(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
        build_uniform_material_data_buffer.setSize(1);
        build_uniform_material_data_buffer.addQueueFamily(queue_family_index);

        _ptr_uniform_matrices_data_buffer   = build_uniform_matrices_data_buffer.buildPtr();
        _ptr_uniform_material_data_buffer   = build_uniform_material_data_buffer.buildPtr();

        build_graphics_pipline.getVertexInputState().addVertexInputBindingDescription(
            0,
            sizeof(Mesh::VertexAttrib),
            VK_VERTEX_INPUT_RATE_VERTEX
        );

        build_graphics_pipline.getVertexInputState().addVertexInputAttributeDescription(
            GBufferPassLocations::Position,
            0,
            VK_FORMAT_R32G32B32_SFLOAT,
            offsetof(Mesh::VertexAttrib, position)
        );

        build_graphics_pipline.getVertexInputState().addVertexInputAttributeDescription(
            GBufferPassLocations::Normal,
            0,
            VK_FORMAT_R32G32B32_SFLOAT, offsetof(Mesh::VertexAttrib, normal)
        );

         build_graphics_pipline.getVertexInputState().addVertexInputAttributeDescription(
            GBufferPassLocations::Tangent,
            0,
            VK_FORMAT_R32G32B32_SFLOAT, offsetof(Mesh::VertexAttrib, tangent)
        );

         build_graphics_pipline.getVertexInputState().addVertexInputAttributeDescription(
            GBufferPassLocations::UV,
            0,
            VK_FORMAT_R32G32B32_SFLOAT, offsetof(Mesh::VertexAttrib, uv)
        );

        build_graphics_pipline.getColorBlendState().addAttchament(VK_FALSE);
        build_graphics_pipline.getColorBlendState().addAttchament(VK_FALSE);
        build_graphics_pipline.getColorBlendState().addAttchament(VK_FALSE);
        build_graphics_pipline.getColorBlendState().addAttchament(VK_FALSE);
        
        build_graphics_pipline.getColorBlendState().setLogicOp(VK_LOGIC_OP_COPY);

        build_graphics_pipline.getDepthStencilState().depthBoundsTestEnable(VK_FALSE);
        build_graphics_pipline.getDepthStencilState().depthTestEnable(VK_TRUE);
        build_graphics_pipline.getDepthStencilState().depthWriteEnable(VK_TRUE);
        build_graphics_pipline.getDepthStencilState().stencilTestEnable(VK_FALSE);
        build_graphics_pipline.getDepthStencilState().setDepthCompareOp(VK_COMPARE_OP_LESS_OR_EQUAL);
        
        build_graphics_pipline.getInputAssemblyState().setTopology(VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);

        build_graphics_pipline.getMultisampleState().alphaToCoverageEnable(VK_FALSE);
        build_graphics_pipline.getMultisampleState().alphaToOneEnable(VK_FALSE);
        build_graphics_pipline.getMultisampleState().sampleShadingEnable(VK_FALSE);
        build_graphics_pipline.getMultisampleState().setMinSampleShading(1.0f);
        build_graphics_pipline.getMultisampleState().setRasterizationSamples(VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT);

        build_graphics_pipline.getRasterizationState().depthBiasEnable(VK_FALSE);
        build_graphics_pipline.getRasterizationState().depthClampEnable(VK_FALSE);
        build_graphics_pipline.getRasterizationState().setLineWidth(1.0f);
        build_graphics_pipline.getRasterizationState().setPolygonMode(VkPolygonMode::VK_POLYGON_MODE_FILL);
        build_graphics_pipline.getRasterizationState().setCullMode(VK_CULL_MODE_NONE);
        build_graphics_pipline.getRasterizationState().setDepthBiasClamp(0.0f);
        build_graphics_pipline.getRasterizationState().setDepthBiasConstantFactor(0.0f);
        build_graphics_pipline.getRasterizationState().setDepthBiasSlopeFactor(0.0f);

        build_graphics_pipline.getViewportState().addViewport(
            0.0f, 0.0f,
            static_cast<float>(window_width), static_cast<float>(window_height),
            0.0f, 1.0f
        );

        build_graphics_pipline.getViewportState().addScissor(0, 0, window_width, window_height);
        
        build_graphics_pipline.addShader(build_vert_shader.build());
        build_graphics_pipline.addShader(build_frag_shader.build());
        build_graphics_pipline.setRenderPass(_ptr_render_pass);
        build_graphics_pipline.setPipleineLayout(PipelineLayout::make(ptr_device, ptr_descriptor_set_layout));
        build_graphics_pipline.setSubpassIndex(subpass_index);

        _ptr_pipeline =  build_graphics_pipline.buildPtr();
        
        _ptr_descriptor_set->writeBuffer(
            *_ptr_uniform_matrices_data_buffer, 
            0, sizeof(UniformBufferMatrices), 
            GBufferPassBindings::MatricesData, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER
        );

        _ptr_descriptor_set->writeBuffer(
            *_ptr_uniform_material_data_buffer, 
            0, sizeof(MaterialData), 
            GBufferPassBindings::MaterailData, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER
        );
    }

    void GBufferPass::draw(
        const Transform&                projection,
        const Transform&                view,
        const Scene::VisibleList&       drawable_objects, 
        const PtrPrimaryCommandBuffer&  ptr_command_buffer,
        const PtrSampler&               ptr_sampler
    )
    {
        UniformBufferMatrices   uniform_matrices_buffer_data;
        MaterialData            uniform_material_buffer_data;

        ptr_command_buffer->reset();
        ptr_command_buffer->begin();
        
        Vec4<float> color_image_clear_val                   (0.0);
        Vec4<float> alebdo_and_ao                           (1.0, 1.0, 1.0, 0.0);
        Vec4<float> tangent_and_aniso_image_clear_val       (0.0);
        Vec4<float> pos_and_metallic_image_clear_val        (0.0);
        Vec4<float> normal_and_roughness_image_clear_val    (0.0);

        float       depth_clear_val     = 1.0;
        uint32_t    stencil_clear_val   = 0;

        VkImageSubresourceRange color_image_subresource_range = { };
        color_image_subresource_range.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
        color_image_subresource_range.baseMipLevel   = 0;
        color_image_subresource_range.levelCount     = 1;
        color_image_subresource_range.baseArrayLayer = 0;
        color_image_subresource_range.layerCount     = 1;

        VkImageSubresourceRange depth_stencil_image_subresource_range = { };
        depth_stencil_image_subresource_range.aspectMask     = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
        depth_stencil_image_subresource_range.baseMipLevel   = 0;
        depth_stencil_image_subresource_range.levelCount     = 1;
        depth_stencil_image_subresource_range.baseArrayLayer = 0;
        depth_stencil_image_subresource_range.layerCount     = 1;

        ptr_command_buffer->imageMemoryBarrier(
            VK_PIPELINE_STAGE_HOST_BIT,
            VK_PIPELINE_STAGE_TRANSFER_BIT,
            0,
            0,
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            0,
            0,
            *outputImpl(OutputImagesViewsIDs::AlbedoAndBakedAO).getImage(),
            color_image_subresource_range
        );

        ptr_command_buffer->imageMemoryBarrier(
            VK_PIPELINE_STAGE_HOST_BIT,
            VK_PIPELINE_STAGE_TRANSFER_BIT,
            0,
            0,
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            0,
            0,
            *outputImpl(OutputImagesViewsIDs::TangentAndAnisotropy).getImage(),
            color_image_subresource_range
        );

        ptr_command_buffer->imageMemoryBarrier(
            VK_PIPELINE_STAGE_HOST_BIT,
            VK_PIPELINE_STAGE_TRANSFER_BIT,
            0,
            0,
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            0,
            0,
            *outputImpl(OutputImagesViewsIDs::PositionAndMetallic).getImage(),
            color_image_subresource_range
        );

        ptr_command_buffer->imageMemoryBarrier(
            VK_PIPELINE_STAGE_HOST_BIT,
            VK_PIPELINE_STAGE_TRANSFER_BIT,
            0,
            0,
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            0,
            0,
            *outputImpl(OutputImagesViewsIDs::NormalAndRoughness).getImage(),
            color_image_subresource_range
        );

        ptr_command_buffer->imageMemoryBarrier(
            VK_PIPELINE_STAGE_HOST_BIT,
            VK_PIPELINE_STAGE_TRANSFER_BIT,
            0,
            0,
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            0,
            0,
            *outputImpl(OutputImagesViewsIDs::Depth).getImage(),
            depth_stencil_image_subresource_range
        );

        ptr_command_buffer->clearColorImage(
            *outputImpl(OutputImagesViewsIDs::AlbedoAndBakedAO).getImage(),
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            alebdo_and_ao,
            color_image_subresource_range
        );

        ptr_command_buffer->clearColorImage(
            *outputImpl(OutputImagesViewsIDs::TangentAndAnisotropy).getImage(),
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            tangent_and_aniso_image_clear_val,
            color_image_subresource_range
        );

        ptr_command_buffer->clearColorImage(
            *outputImpl(OutputImagesViewsIDs::PositionAndMetallic).getImage(),
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            pos_and_metallic_image_clear_val,
            color_image_subresource_range
        );

        ptr_command_buffer->clearColorImage(
            *outputImpl(OutputImagesViewsIDs::NormalAndRoughness).getImage(),
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            normal_and_roughness_image_clear_val,
            color_image_subresource_range
        );

        ptr_command_buffer->clearDepthStencilImage(
            *outputImpl(OutputImagesViewsIDs::Depth).getImage(),
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            depth_clear_val, stencil_clear_val,
            depth_stencil_image_subresource_range
        );

        ptr_command_buffer->end();

        _ptr_device_queue->submit(*ptr_command_buffer);
        _ptr_device_queue->waitIdle();

        for (size_t i{0}, size{drawable_objects.size()}; i < size; i++) {
            if (drawable_objects[i]->hasComponent<Mesh>()) {
                Mesh& mesh = drawable_objects[i]->getComponent<Mesh>();

                if(mesh.getMaterial()) {
                    /*  ----------------------------------------------------------------------  */
                    const PtrMaterial&  ptr_material    = mesh.getMaterial();
                    Transform           model           = drawable_objects[i]->getWorldTransform() * drawable_objects[i]->getLocalTransform();

                    uniform_matrices_buffer_data.model_view_matrix  = (view * model).getMatrix();
                    uniform_matrices_buffer_data.MVP                = projection.getMatrix() * uniform_matrices_buffer_data.model_view_matrix;
                    uniform_matrices_buffer_data.normal_matrix      = transpose(inverse(uniform_matrices_buffer_data.model_view_matrix));

                    uniform_material_buffer_data.anisotropy = ptr_material->getAnisotropy();

                    _ptr_uniform_matrices_data_buffer->getDeviceMemory()->setData(&uniform_matrices_buffer_data, 1);
                    _ptr_uniform_material_data_buffer->getDeviceMemory()->setData(&uniform_material_buffer_data, 1);
                    
                    PtrBuffer& ptr_vertex_buffer    = mesh.getVertexBuffer();
                    PtrBuffer& ptr_index_buffer     = mesh.getIndexBuffer();

                    _ptr_descriptor_set->writeImageView(
                        *ptr_material->getTexture<Material::Textures::Albedo>(), 
                        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, 
                        *ptr_sampler, 
                        GBufferPassBindings::Albedo, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER
                    );

                    _ptr_descriptor_set->writeImageView(
                        *ptr_material->getTexture<Material::Textures::NormalMap>(), 
                        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, 
                        *ptr_sampler, 
                        GBufferPassBindings::NormalMap, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER
                    );

                    _ptr_descriptor_set->writeImageView(
                        *mesh.getMaterial()->getTexture<Material::Textures::Metallic>(), 
                        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, 
                        *ptr_sampler, 
                        GBufferPassBindings::Metallic, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER
                    );

                    _ptr_descriptor_set->writeImageView(
                        *ptr_material->getTexture<Material::Textures::Roughness>(), 
                        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, 
                        *ptr_sampler, 
                        GBufferPassBindings::Roughness, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER
                    );

                    _ptr_descriptor_set->writeImageView(
                        *ptr_material->getTexture<Material::Textures::BakedAO>(), 
                        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, 
                        *ptr_sampler, 
                        GBufferPassBindings::AO, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER
                    );

                    ptr_command_buffer->reset();
                    ptr_command_buffer->begin();

                    ptr_command_buffer->begineRenderPass(getFramebuffer());
                    ptr_command_buffer->bindToPipeline(getPipeline());

                    ptr_command_buffer->bindVertexBuffer(*ptr_vertex_buffer, mesh.getVertexBufferOffset());
                    ptr_command_buffer->bindIndexBuffer(*ptr_index_buffer, mesh.getIndexBufferOffset(), VK_INDEX_TYPE_UINT32);
                    ptr_command_buffer->bindDescriptorSet(_ptr_pipeline, *_ptr_descriptor_set);
                    ptr_command_buffer->drawIndexed(mesh.getNumIndices(), 1, 0, 0, 0);

                    ptr_command_buffer->endRenderPass();
                    ptr_command_buffer->end();

                    _ptr_device_queue->submit(*ptr_command_buffer);
                    _ptr_device_queue->waitIdle();
                }
            }
        }
    }

    PtrGraphicsPipeline& GBufferPass::getPipeline() noexcept
    {
        return _ptr_pipeline;
    }

    const PtrGraphicsPipeline& GBufferPass::getPipeline() const noexcept
    {
        return _ptr_pipeline;
    }

    PtrDescriptorSet& GBufferPass::getDescriptorSet() noexcept
    {
        return _ptr_descriptor_set;
    }

    const PtrDescriptorSet& GBufferPass::getDescriptorSet() const noexcept
    {
        return _ptr_descriptor_set;
    }

    const PtrFramebuffer& GBufferPass::getFramebuffer() const noexcept
    {
        return _ptr_framebuffer;
    }

    const ImageView& GBufferPass::outputImpl(size_t id) const
    {
        return _ptr_framebuffer_attachments->at(id);
    }

    void GBufferPass::outputImpl(PtrImageView& ptr_image_view, size_t id)
    {}

    PtrGBufferPass GBufferPass::make(
        const PtrDevice&            ptr_device, 
        const PtrDeviceQueue&       ptr_queue,
        const PtrPhysicalDevice&    ptr_physical_device,
        const PtrDescriptorPool&    ptr_descriptor_pool,  
        uint32_t                    gpu_memory_index,
        uint32_t                    window_width,
        uint32_t                    window_height
    )
    {
        return make_unique<GBufferPass>(
            ptr_device, 
            ptr_queue,
            ptr_physical_device,
            ptr_descriptor_pool, 
            gpu_memory_index, 
            window_width, window_height
        );
    }
}