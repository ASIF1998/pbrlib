#include <backend/renderer/frame_graph/gbuffer_generator.hpp>

#include <backend/renderer/vulkan/shader_compiler.hpp>
#include <backend/renderer/vulkan/device.hpp>
#include <backend/utils/vulkan.hpp>

#include <backend/utils/paths.hpp>

#include <array>

namespace pbrlib
{
    GBufferGenerator::~GBufferGenerator()
    {
        vkDestroyRenderPass(_ptr_device->device(), _render_pass_handle, nullptr);
        vkDestroyPipelineLayout(_ptr_device->device(), _pipeline_layout_handle, nullptr);
        vkDestroyPipeline(_ptr_device->device(), _pipeline_handle, nullptr);
    }

    bool GBufferGenerator::init(const vk::Device* ptr_device)
    {
        _ptr_device = ptr_device;

        createPipeline();

        return true;
    }
    
    void GBufferGenerator::render(const SceneItem* ptr_item)
    {
        /// @todo
    }
}

namespace pbrlib
{
    void GBufferGenerator::createPipeline()
    {
        createPipelineLayout();
        createRenderPass();

        constexpr auto vert_shader = "shaders/gbuffer_generator/gbuffer_generator.vert";
        constexpr auto frag_shader = "shaders/gbuffer_generator/gbuffer_generator.frag";

        std::array<VkPipelineShaderStageCreateInfo, 2> stages;

        stages[0] = 
        { 
            .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .stage  = VK_SHADER_STAGE_VERTEX_BIT,
            .module = vk::shader::compile(_ptr_device, vert_shader),
            .pName  = "main"
        };
        
        stages[1] = 
        { 
            .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .stage  = VK_SHADER_STAGE_FRAGMENT_BIT,
            .module = vk::shader::compile(_ptr_device, frag_shader),
            .pName  = "main"
        };

        constexpr VkPipelineVertexInputStateCreateInfo vertex_input_state = 
        { 
            .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO
        };

        constexpr VkPipelineInputAssemblyStateCreateInfo input_assembly_state = 
        { 
            .sType      = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
            .topology   = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST
        };

        std::array<VkPipelineColorBlendAttachmentState, 1> attachments_state;

        attachments_state[0] =  
        {
            .blendEnable = VK_FALSE
        };

        constexpr VkPipelineRasterizationStateCreateInfo rasterization_state = 
        {
            .sType                      = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
            .depthClampEnable           = VK_FALSE,
            .rasterizerDiscardEnable    = VK_FALSE,
            .polygonMode                = VK_POLYGON_MODE_FILL,
            .cullMode                   = VK_CULL_MODE_BACK_BIT,
            .frontFace                  = VK_FRONT_FACE_COUNTER_CLOCKWISE,
            .depthBiasEnable            = VK_FALSE,
            .lineWidth                  = 1.0f
        };

        constexpr VkPipelineViewportStateCreateInfo viewport_state = 
        {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO
        };

        constexpr VkPipelineDepthStencilStateCreateInfo depth_stencil_state =
        {
            .sType              = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
            .depthTestEnable    = VK_TRUE,
            .depthWriteEnable   = VK_TRUE,
            .depthCompareOp     = VK_COMPARE_OP_LESS,
            .stencilTestEnable  = VK_FALSE
        };

        constexpr VkPipelineMultisampleStateCreateInfo multisample_state = 
        {
            .sType                  = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
            .rasterizationSamples   = VK_SAMPLE_COUNT_1_BIT
        };

        const VkPipelineColorBlendStateCreateInfo color_blend_state = 
        {
            .sType              = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
            .logicOpEnable      = VK_FALSE,
            .attachmentCount    = static_cast<uint32_t>(attachments_state.size()),
            .pAttachments       = attachments_state.data()
        };

        constexpr std::array dynamic_states = 
        {
            VK_DYNAMIC_STATE_VIEWPORT_WITH_COUNT,
            VK_DYNAMIC_STATE_SCISSOR_WITH_COUNT
        };

        const VkPipelineDynamicStateCreateInfo dynamic_state = 
        { 
            .sType              = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
            .dynamicStateCount  = static_cast<uint32_t>(dynamic_states.size()),
            .pDynamicStates     = dynamic_states.data()
        };

        const VkGraphicsPipelineCreateInfo pipeline_create_info = 
        {
            .sType                  = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
            .stageCount             = static_cast<uint32_t>(stages.size()),
            .pStages                = stages.data(),
            .pVertexInputState      = &vertex_input_state,
            .pInputAssemblyState    = &input_assembly_state,
            .pViewportState         = &viewport_state,
            .pRasterizationState    = &rasterization_state,
            .pMultisampleState      = &multisample_state,
            .pDepthStencilState     = &depth_stencil_state,
            .pColorBlendState       = &color_blend_state,
            .pDynamicState          = &dynamic_state,
            .layout                 = _pipeline_layout_handle,
            .renderPass             = _render_pass_handle,
            .subpass                = 0
        };

        VK_CHECK(
            vkCreateGraphicsPipelines(
                _ptr_device->device(),
                VK_NULL_HANDLE, 
                1, &pipeline_create_info,
                nullptr, 
                &_pipeline_handle
            )
        );

        for (const auto& shader_stage: stages)
            vkDestroyShaderModule(_ptr_device->device(), shader_stage.module, nullptr);
    }

    void GBufferGenerator::createPipelineLayout()
    {
        std::array<VkDescriptorSetLayoutBinding, 3> bindings;
        for (uint32_t i = 0; i < bindings.size(); ++i)
        {
            bindings[i] = 
            {
                .binding            = i,
                .descriptorType     = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC,
                .descriptorCount    = 1,
                .stageFlags         = VK_SHADER_STAGE_VERTEX_BIT
            };
        }

        VkDescriptorSetLayout descriptor_set_layout_handle = VK_NULL_HANDLE;
        const VkDescriptorSetLayoutCreateInfo descriptor_set_layout_create_info = 
        {
            .sType          = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
            .bindingCount   = static_cast<uint32_t>(bindings.size()),
            .pBindings      = bindings.data()
        };

        VK_CHECK(
            vkCreateDescriptorSetLayout(
                _ptr_device->device(),
                &descriptor_set_layout_create_info,
                nullptr,
                &descriptor_set_layout_handle
            )
        );

        constexpr VkPushConstantRange push_constant_range = 
        { 
            .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
            .offset     = 0,
            .size       = 68
        };

        const VkPipelineLayoutCreateInfo pipeline_layout_create_info = 
        { 
            .sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
            .setLayoutCount         = 1,
            .pSetLayouts            = &descriptor_set_layout_handle,
            .pushConstantRangeCount = 1,
            .pPushConstantRanges    = &push_constant_range
        };

        VK_CHECK(
            vkCreatePipelineLayout(
                _ptr_device->device(),
                &pipeline_layout_create_info,
                nullptr,
                &_pipeline_layout_handle
            )
        );

        vkDestroyDescriptorSetLayout(
            _ptr_device->device(),
            descriptor_set_layout_handle,
            nullptr
        );
    }

    void GBufferGenerator::createRenderPass()
    {
        std::array<VkAttachmentDescription, 2> attachments;

        attachments[0] = 
        {
            .format         = VK_FORMAT_R32G32B32A32_SFLOAT,
            .samples        = VK_SAMPLE_COUNT_1_BIT,
            .loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR,
            .storeOp        = VK_ATTACHMENT_STORE_OP_STORE,
            .initialLayout  = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            .finalLayout    = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        };
        
        attachments[1] = 
        {
            .format         = VK_FORMAT_D32_SFLOAT,
            .samples        = VK_SAMPLE_COUNT_1_BIT,
            .loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR,
            .storeOp        = VK_ATTACHMENT_STORE_OP_STORE,
            .initialLayout  = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL,
            .finalLayout    = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL
        };

        constexpr VkAttachmentReference color_attachment_reference 
        {
            .attachment = 0,
            .layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
        };

        constexpr VkAttachmentReference depth_attachment_reference =
        {
            .attachment = 1,
            .layout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL
        };

        const VkSubpassDescription subpass_description = 
        {
            .pipelineBindPoint          = VK_PIPELINE_BIND_POINT_GRAPHICS,
            .colorAttachmentCount       = 1,
            .pColorAttachments          = &color_attachment_reference,
            .pDepthStencilAttachment    = &depth_attachment_reference
        };

        /// @todo VkRenderPassCreateInfo2 
        const VkRenderPassCreateInfo render_pass_create_info = 
        {
            .sType              = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
            .attachmentCount    = static_cast<uint32_t>(attachments.size()),
            .pAttachments       = attachments.data(),
            .subpassCount       = 1,
            .pSubpasses         = &subpass_description
        };

        VK_CHECK(
            vkCreateRenderPass(
                _ptr_device->device(),
                &render_pass_create_info,
                nullptr,
                &_render_pass_handle
            )
        );
    }
}