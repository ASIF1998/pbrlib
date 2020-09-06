//
//  GBufferPass.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 20/06/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include "GBufferPass.hpp"

#include "../../VulkanWrapper/Device.hpp"

#include "../../VulkanWrapper/ShaderModule.hpp"

#include "../../VulkanWrapper/DescriptorSet.hpp"
#include "../../VulkanWrapper/GraphicsPipeline.hpp"
#include "../../VulkanWrapper/RenderPass.hpp"

#include "../../Geometry/Mesh.hpp"

#include "../../VulkanWrapper/CommandBuffer.hpp"

#include "../../VulkanWrapper/Sampler.hpp"
#include "../../Material/Material.hpp"

#include "../Shaders/MaterialData.h"

#include "spv/GBuffer.glsl.vert.spv.h"
#include "spv/GBuffer.glsl.frag.spv.h"

namespace pbrlib
{
    /**
     * @struct UniformBufferMatrices.
     * @brief Структура, описывающая uniform-буфер с необходимыми матрицами для прохода g-буфера.
    */
    struct UniformBufferMatrices
    {
        Matrix4x4<float> MVP;
        Matrix4x4<float> model_matrix;
        Matrix4x4<float> normal_matrix;
    };

    enum class GBufferPassBindings
    {
        MatricesData = 0,
        Albedo,
        NormalMap,
        Metallic,
        Roughness,
        AO,
        MaterailData
    };

    enum class GBufferPassLocations
    {
        Position = 0,
        Normal,
        Tangent,
        UV
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void GBufferPass::Builder::setDevice(const PtrDevice& ptr_device)
    {
        _ptr_device = ptr_device;
        
    }

    void GBufferPass::Builder::setDescriptorPool(const PtrDescriptorPool& ptr_descriptor_pool)
    {
       _ptr_descriptor_pool = ptr_descriptor_pool;
    }

    void GBufferPass::Builder::setRenderPass(const PtrRenderPass& ptr_render_pass)
    {
       _ptr_render_pass = ptr_render_pass;
    }

    void GBufferPass::Builder::setSubpassIndex(uint32_t subpass_index)
    {
        _subpass_index = subpass_index;
    }

    void GBufferPass::Builder::setDeviceMemoryIndex(uint32_t memory_index)
    {
        _gpu_memory_index = memory_index;
    }

    void GBufferPass::Builder::setDeviceQueueFamilyIndex(uint32_t queue_family_index)
    {
        _gpu_queue_family_index = queue_family_index;
    }

    void GBufferPass::Builder::windowSize(uint32_t width, uint32_t height)
    {
        _window_width   = width;
        _window_height  = height;
    }

    GBufferPass GBufferPass::Builder::build()
    {
        return GBufferPass(
            _ptr_device, 
            _ptr_descriptor_pool, 
            _ptr_render_pass, 
            _subpass_index, 
            _gpu_memory_index, 
            _gpu_queue_family_index,
            _window_width, _window_height
        );
    }
    
    PtrGBufferPass GBufferPass::Builder::buildPtr()
    {
        return GBufferPass::make(
            _ptr_device, 
            _ptr_descriptor_pool, 
            _ptr_render_pass, 
            _subpass_index, 
            _gpu_memory_index, 
            _gpu_queue_family_index,
            _window_width, _window_height
        );
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    GBufferPass::GBufferPass(
        const PtrDevice&            ptr_device, 
        const PtrDescriptorPool&    ptr_descriptor_pool,  
        const PtrRenderPass&        ptr_render_pass,
        uint32_t                    subpass_index,
        uint32_t                    gpu_memory_index,
        uint32_t                    gpu_queue_family_index,
        uint32_t                    window_width,
        uint32_t                    window_height
    )
    {
        ShaderModule::Builder build_vert_shader;
        ShaderModule::Builder build_frag_shader;

        GraphicsPipeline::Builder   build_graphics_pipline  (1, 4, 1, 1, 4);

        Buffer::Builder<UniformBufferMatrices>  build_uniform_matrices_data_buffer;
        Buffer::Builder<MaterialData>           build_uniform_material_data_buffer;
        
        DescriptorSetLayoutBindings descriptor_set_layout_bindings (ptr_device, 0);

        descriptor_set_layout_bindings.addBinding(util::enumCast(GBufferPassBindings::MatricesData), VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT);
        descriptor_set_layout_bindings.addBinding(util::enumCast(GBufferPassBindings::Albedo), VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT);
        descriptor_set_layout_bindings.addBinding(util::enumCast(GBufferPassBindings::NormalMap), VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT);
        descriptor_set_layout_bindings.addBinding(util::enumCast(GBufferPassBindings::Metallic), VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT);
        descriptor_set_layout_bindings.addBinding(util::enumCast(GBufferPassBindings::Roughness), VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT);
        descriptor_set_layout_bindings.addBinding(util::enumCast(GBufferPassBindings::AO), VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT);
        descriptor_set_layout_bindings.addBinding(util::enumCast(GBufferPassBindings::MaterailData), VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT);

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
        build_uniform_matrices_data_buffer.addQueueFamily(gpu_queue_family_index);

        build_uniform_material_data_buffer.setDevice(ptr_device);
        build_uniform_material_data_buffer.setMemoryTypeIndex(gpu_memory_index);
        build_uniform_material_data_buffer.setUsage(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
        build_uniform_material_data_buffer.setSize(1);
        build_uniform_material_data_buffer.addQueueFamily(gpu_queue_family_index);

        _ptr_uniform_matrices_data_buffer   = build_uniform_matrices_data_buffer.buildPtr();
        _ptr_uniform_material_data_buffer   = build_uniform_material_data_buffer.buildPtr();

        build_graphics_pipline.getVertexInputState().addVertexInputBindingDescription(
            0,
            sizeof(Mesh::VertexAttrib),
            VK_VERTEX_INPUT_RATE_VERTEX
        );

        build_graphics_pipline.getVertexInputState().addVertexInputAttributeDescription(
            util::enumCast(GBufferPassLocations::Position),
            0,
            VK_FORMAT_R32G32B32_SFLOAT,
            offsetof(Mesh::VertexAttrib, position)
        );

        build_graphics_pipline.getVertexInputState().addVertexInputAttributeDescription(
            util::enumCast(GBufferPassLocations::Normal),
            0,
            VK_FORMAT_R32G32B32_SFLOAT, offsetof(Mesh::VertexAttrib, normal)
        );

         build_graphics_pipline.getVertexInputState().addVertexInputAttributeDescription(
            util::enumCast(GBufferPassLocations::Tangent),
            0,
            VK_FORMAT_R32G32B32_SFLOAT, offsetof(Mesh::VertexAttrib, tangent)
        );

         build_graphics_pipline.getVertexInputState().addVertexInputAttributeDescription(
            util::enumCast(GBufferPassLocations::UV),
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
            window_width, window_height,
            0.0f, 1.0f
        );

        build_graphics_pipline.getViewportState().addScissor(0, 0, window_width, window_height);
        
        build_graphics_pipline.addShader(build_vert_shader.build());
        build_graphics_pipline.addShader(build_frag_shader.build());
        build_graphics_pipline.setRenderPass(ptr_render_pass);
        build_graphics_pipline.setPipleineLayout(PipelineLayout::make(ptr_device, ptr_descriptor_set_layout));
        build_graphics_pipline.setSubpassIndex(subpass_index);

        _ptr_pipeline =  build_graphics_pipline.buildPtr();
        
        _ptr_descriptor_set->writeBuffer(
            *_ptr_uniform_matrices_data_buffer, 
            0, sizeof(UniformBufferMatrices), 
            util::enumCast(GBufferPassBindings::MatricesData), VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER
        );

        _ptr_descriptor_set->writeBuffer(
            *_ptr_uniform_material_data_buffer, 
            0, sizeof(MaterialData), 
            util::enumCast(GBufferPassBindings::MaterailData), VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER
        );
    }

    void GBufferPass::draw(
        const Transform&            projection,
        const Transform&            view,
        const Scene::VisibleList&   drawable_objects, 
        const PtrCommandBuffer&     ptr_command_buffer,
        const PtrSampler&           ptr_sampler
    )
    {
        UniformBufferMatrices   uniform_matrices_buffer_data;
        MaterialData            uniform_material_buffer_data;

        for (size_t i{0}, size{drawable_objects.size()}; i < size; i++) {
            if (drawable_objects[i]->hasComponent<Mesh>()) {
                Mesh& mesh = drawable_objects[i]->getComponent<Mesh>();

                if(mesh.getMaterial()) {
                    const PtrMaterial&  ptr_material        = mesh.getMaterial();
                    Transform           current_transform   = drawable_objects[i]->getWorldTransform() * drawable_objects[i]->getLocalTransform();
                    
                    uniform_matrices_buffer_data.model_matrix   = current_transform.getMatrix();
                    uniform_matrices_buffer_data.MVP            = (projection * view * current_transform).getMatrix();
                    uniform_matrices_buffer_data.normal_matrix  = transpose(inverse(uniform_matrices_buffer_data.model_matrix));

                    uniform_material_buffer_data.anisotropy = ptr_material->getAnisotropy();

                    _ptr_uniform_matrices_data_buffer->setData(&uniform_matrices_buffer_data, 1);
                    _ptr_uniform_material_data_buffer->setData(&uniform_material_buffer_data, 1);
                    
                    PtrBuffer& ptr_vertex_buffer    = mesh.getVertexBuffer();
                    PtrBuffer& ptr_index_buffer     = mesh.getIndexBuffer();

                    _ptr_descriptor_set->writeImageView(
                        *ptr_material->getTexture<Material::Textures::Albedo>(), 
                        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, 
                        *ptr_sampler, 
                        util::enumCast(GBufferPassBindings::Albedo), VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER
                    );

                    _ptr_descriptor_set->writeImageView(
                        *ptr_material->getTexture<Material::Textures::NormalMap>(), 
                        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, 
                        *ptr_sampler, 
                        util::enumCast(GBufferPassBindings::NormalMap), VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER
                    );

                    _ptr_descriptor_set->writeImageView(
                        *mesh.getMaterial()->getTexture<Material::Textures::Metallic>(), 
                        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, 
                        *ptr_sampler, 
                        util::enumCast(GBufferPassBindings::Metallic), VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER
                    );

                    _ptr_descriptor_set->writeImageView(
                        *ptr_material->getTexture<Material::Textures::Roughness>(), 
                        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, 
                        *ptr_sampler, 
                        util::enumCast(GBufferPassBindings::Roughness), VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER
                    );

                    _ptr_descriptor_set->writeImageView(
                        *ptr_material->getTexture<Material::Textures::BakedAO>(), 
                        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, 
                        *ptr_sampler, 
                        util::enumCast(GBufferPassBindings::AO), VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER
                    );
                    
                    ptr_command_buffer->bindVertexBuffer(*ptr_vertex_buffer, mesh.getVertexBufferOffset());
                    ptr_command_buffer->bindIndexBuffer(*ptr_index_buffer, mesh.getIndexBufferOffset(), VK_INDEX_TYPE_UINT32);
                    ptr_command_buffer->bindDescriptorSet(_ptr_pipeline, *_ptr_descriptor_set);
                    ptr_command_buffer->drawIndexed(mesh.getNumIndices(), 1, 0, 0, 0);
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

    PtrGBufferPass GBufferPass::make(
        const PtrDevice&            ptr_device, 
        const PtrDescriptorPool&    ptr_descriptor_pool,  
        const PtrRenderPass&        ptr_render_pass,
        uint32_t                    subpass_index,
        uint32_t                    gpu_memory_index,
        uint32_t                    gpu_queue_family_index,
        uint32_t                    window_width,
        uint32_t                    window_height
    )
    {
        return make_unique<GBufferPass>(
            ptr_device, 
            ptr_descriptor_pool, 
            ptr_render_pass, 
            subpass_index, 
            gpu_memory_index, 
            gpu_queue_family_index,
            window_width, window_height
        );
    }
}
