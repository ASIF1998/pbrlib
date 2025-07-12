#include <backend/renderer/frame_graph/ssao.hpp>

#include <backend/profiling.hpp>

#include <backend/logger/logger.hpp>

#include <backend/renderer/vulkan/device.hpp>
#include <backend/renderer/vulkan/render_pass.hpp>
#include <backend/renderer/vulkan/compute_pipeline.hpp>
#include <backend/renderer/vulkan/command_buffer.hpp>
#include <backend/renderer/vulkan/gpu_marker_colors.hpp>

#include <backend/utils/vulkan.hpp>

namespace pbrlib::backend
{
    SSAO::SSAO(vk::Device& device) :
        RenderPass(device)
    { }

    SSAO::~SSAO()
    {
        const auto device_handle = _ptr_device->device();

        if (vkFreeDescriptorSets(device_handle, _ptr_device->descriptorPool(), 1, &_result_image_desc_set) != VK_SUCCESS) [[unlikely]]
            log::error("[ssao] failed free descriptor set with result image");
        
        if (vkFreeDescriptorSets(device_handle, _ptr_device->descriptorPool(), 1, &_ssao_desc_set) != VK_SUCCESS) [[unlikely]]
            log::error("[ssao] failed free descriptor set with data for compute");

        vkDestroyDescriptorSetLayout(device_handle, _result_image_desc_set_layout, nullptr);
        vkDestroyDescriptorSetLayout(device_handle, _ssao_desc_set_layout, nullptr);
        
        vkDestroySampler(device_handle, _result_image_sampler, nullptr);

        vkDestroyPipelineLayout(device_handle, _pipeline_layout_handle, nullptr);
        vkDestroyPipeline(device_handle, _pipeline_handle, nullptr);
    }

    bool SSAO::init(const RenderContext& context, uint32_t width, uint32_t height)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        if (!RenderPass::init(context, width, height)) [[unlikely]]
        {
            log::error("[ssao] failed initialize");
            return false;
        }

        _params_buffer = vk::builders::Buffer(*_ptr_device)
            .addQueueFamilyIndex(_ptr_device->queue().family_index)
            .name("[ssao] params")
            .size(sizeof(Params))
            .type(vk::BufferType::eDeviceOnly)
            .usage(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT)
            .build();

        _params_buffer->write(Params(), 0);

        createResultDescriptorSet();
        createSSAODescriptorSet();

        const auto [_, gbuffer_set_layout] = descriptorSet(SSAOInputSetsId::eGBuffer);

        _pipeline_layout_handle = vk::builders::PipelineLayout(*_ptr_device)
            .addSetLayout(gbuffer_set_layout)
            .addSetLayout(_ssao_desc_set_layout)
            .build();

        return rebuild();
    }

    bool SSAO::rebuild()
    {
        constexpr auto ssao_shader = "shaders/ssao/ssao.glsl.comp";

        auto prev_handle = _pipeline_handle;

        _pipeline_handle = vk::builders::ComputePipeline(*_ptr_device)
            .shader(ssao_shader)
            .pipelineLayoutHandle(_pipeline_layout_handle)
            .build();

        vkDestroyPipeline(_ptr_device->device(), prev_handle, nullptr);

        return true;
    }

    void SSAO::render(vk::CommandBuffer& command_buffer)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        command_buffer.write([this] (VkCommandBuffer command_buffer_handle)
        {
            const auto [gbuffer_descriptor_set, _] = descriptorSet(SSAOInputSetsId::eGBuffer);

            vkCmdBindPipeline(command_buffer_handle, VK_PIPELINE_BIND_POINT_COMPUTE, _pipeline_handle);
            vkCmdBindDescriptorSets(command_buffer_handle, VK_PIPELINE_BIND_POINT_COMPUTE, _pipeline_layout_handle, 0, 1, &gbuffer_descriptor_set, 0, nullptr);
            vkCmdBindDescriptorSets(command_buffer_handle, VK_PIPELINE_BIND_POINT_COMPUTE, _pipeline_layout_handle, 1, 1, &_ssao_desc_set, 0, nullptr);
            vkCmdDispatch(command_buffer_handle, _width, _height, 1);
        }, "[ssao] run-pipeline", vk::marker_colors::compute_pipeline);
    }

    VkPipelineStageFlags2 SSAO::srcStage() const noexcept
    {
        return VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT;
    }

    VkPipelineStageFlags2 SSAO::dstStage() const noexcept
    {
        return VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT;
    }

    std::pair<VkDescriptorSet, VkDescriptorSetLayout> SSAO::resultDescriptorSet() const noexcept
    {
        return std::make_pair(_result_image_desc_set, _result_image_desc_set_layout);
    }

    void SSAO::createSampler()
    {
        constexpr VkSamplerCreateInfo sampler_create_info 
        {
            .sType          = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
            .magFilter      = VK_FILTER_NEAREST,
            .minFilter      = VK_FILTER_NEAREST,
            .addressModeU   = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
            .addressModeV   = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
            .addressModeW   = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE
        };

        VK_CHECK(vkCreateSampler(
            _ptr_device->device(),
            &sampler_create_info,
            nullptr, 
            &_result_image_sampler
        ));
    }

    void SSAO::createResultDescriptorSet()
    {
        _result_image_desc_set_layout = vk::builders::DescriptorSetLayout(*_ptr_device)
            .addBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_COMPUTE_BIT)
            .build();

        _result_image_desc_set = _ptr_device->allocateDescriptorSet(_result_image_desc_set_layout, "[ssao] descritor set with results");

        createSampler();

        const auto ptr_result_image = colorOutputAttach(SSAOOutputAttachmentsNames::result);

        _ptr_device->writeDescriptorSet({
            .view_handle            = ptr_result_image->view_handle,
            .sampler_handle         = _result_image_sampler,
            .set_handle             = _result_image_desc_set,
            .expected_image_layout  = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
            .binding                = 0
        });
    }
    
    void SSAO::createSSAODescriptorSet()
    {
        _ssao_desc_set_layout = vk::builders::DescriptorSetLayout(*_ptr_device)
            .addBinding(0, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1, VK_SHADER_STAGE_COMPUTE_BIT)
            .addBinding(1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_COMPUTE_BIT)
            .build();

        _ssao_desc_set = _ptr_device->allocateDescriptorSet(_ssao_desc_set_layout, "[ssao] descritor set with data for compute");

        const auto ptr_result_image = colorOutputAttach(SSAOOutputAttachmentsNames::result);

        _ptr_device->writeDescriptorSet({
            .view_handle            = ptr_result_image->view_handle,
            .set_handle             = _ssao_desc_set,
            .expected_image_layout  = VK_IMAGE_LAYOUT_GENERAL,
            .binding                = 0
        });

        _ptr_device->writeDescriptorSet({
            .buffer     = _params_buffer.value(),
            .set_handle = _ssao_desc_set,
            .size       = static_cast<uint32_t>(_params_buffer->size),
            .binding    = 1
        });
    }
}