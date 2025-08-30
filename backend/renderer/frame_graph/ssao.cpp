#include <backend/renderer/frame_graph/ssao.hpp>

#include <backend/profiling.hpp>

#include <backend/logger/logger.hpp>

#include <backend/renderer/vulkan/device.hpp>
#include <backend/renderer/vulkan/render_pass.hpp>
#include <backend/renderer/vulkan/compute_pipeline.hpp>
#include <backend/renderer/vulkan/command_buffer.hpp>
#include <backend/renderer/vulkan/gpu_marker_colors.hpp>

#include <backend/renderer/frame_graph/filters/bilateral_blur.hpp>

#include <backend/scene/material_manager.hpp>

#include <backend/utils/vulkan.hpp>

#include <pbrlib/math/vec3.hpp>
#include <pbrlib/math/vec4.hpp>
#include <pbrlib/math/lerp.hpp>

#include <pbrlib/config.hpp>

#include <random>

#include <array>

namespace pbrlib::backend
{
    SSAO::SSAO(vk::Device& device, BilateralBlur* ptr_blur) :
        RenderPass(device),
        _ptr_blur (ptr_blur)
    { 
        if (!ptr_blur) [[unlikely]]
            throw exception::InvalidArgument("[ssao] pointer to blur is null");

        _result_image_desc_set_layout = vk::builders::DescriptorSetLayout(device)
            .addBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_COMPUTE_BIT)
            .build();

        _result_image_desc_set = device.allocateDescriptorSet(_result_image_desc_set_layout, "[ssao] descritor set with results");
    }

    SSAO::~SSAO()
    {
        const auto device_handle = device().device();

        if (vkFreeDescriptorSets(device_handle, device().descriptorPool(), 1, &_result_image_desc_set) != VK_SUCCESS) [[unlikely]]
            log::error("[ssao] failed free descriptor set with result image");
        
        if (vkFreeDescriptorSets(device_handle, device().descriptorPool(), 1, &_ssao_desc_set) != VK_SUCCESS) [[unlikely]]
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

        createSamplesBuffer();
        createParamsBuffer();

        bindResultDescriptorSet();
        createSSAODescriptorSet();

        const auto gbuffer_set_layout          = descriptorSet(InputDescriptorSetTraits<SSAO>::gbuffer).second;
        const auto material_manager_set_layout = context.ptr_material_manager->descriptorSet().second;

        constexpr VkPushConstantRange push_constant_range =
        {
            .stageFlags = VK_SHADER_STAGE_COMPUTE_BIT,
            .offset     = 0,
            .size       = sizeof(pbrlib::math::mat4)
        };

        _pipeline_layout_handle = vk::builders::PipelineLayout(device())
            .addSetLayout(gbuffer_set_layout)
            .addSetLayout(_ssao_desc_set_layout)
            .addSetLayout(material_manager_set_layout)
            .pushConstant(push_constant_range)
            .build();

        return rebuild(width, height);
    }

    bool SSAO::rebuild(uint32_t width, uint32_t height)
    {
        constexpr auto noise_width  = 4.0f;
        constexpr auto noise_height = 4.0f;

        _params.noise_scale.x = static_cast<float>(width) / noise_width;
        _params.noise_scale.y = static_cast<float>(height) / noise_height;

        device().writeDescriptorSet({
            .buffer     = _params_buffer.value(),
            .set_handle = _ssao_desc_set,
            .size       = static_cast<uint32_t>(_params_buffer->size),
            .binding    = 1
        });

        constexpr auto ssao_shader = "shaders/ssao/ssao.glsl.comp";

        auto prev_handle = _pipeline_handle;

        _pipeline_handle = vk::builders::ComputePipeline(device())
            .shader(ssao_shader)
            .pipelineLayoutHandle(_pipeline_layout_handle)
            .build();

        vkDestroyPipeline(device().device(), prev_handle, nullptr);

        return true;
    }

    void SSAO::render(vk::CommandBuffer& command_buffer)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        command_buffer.write([this] (VkCommandBuffer command_buffer_handle)
        {
            PBRLIB_PROFILING_VK_ZONE_SCOPED(device(), command_buffer_handle, "[ssao] run-pipeline");

            const auto gbuffer_descriptor_set   = descriptorSet(InputDescriptorSetTraits<SSAO>::gbuffer).first;
            const auto material_manager_set     = context().ptr_material_manager->descriptorSet().first;

            vkCmdBindPipeline(command_buffer_handle, VK_PIPELINE_BIND_POINT_COMPUTE, _pipeline_handle);
            vkCmdBindDescriptorSets(command_buffer_handle, VK_PIPELINE_BIND_POINT_COMPUTE, _pipeline_layout_handle, 0, 1, &gbuffer_descriptor_set, 0, nullptr);
            vkCmdBindDescriptorSets(command_buffer_handle, VK_PIPELINE_BIND_POINT_COMPUTE, _pipeline_layout_handle, 1, 1, &_ssao_desc_set, 0, nullptr);
            vkCmdBindDescriptorSets(command_buffer_handle, VK_PIPELINE_BIND_POINT_COMPUTE, _pipeline_layout_handle, 2, 1, &material_manager_set, 0, nullptr);

            const auto projection_view = context().projection * context().view;

            vkCmdPushConstants(
                command_buffer_handle, 
                _pipeline_layout_handle, 
                VK_SHADER_STAGE_COMPUTE_BIT, 
                0, sizeof(pbrlib::math::mat4), &projection_view
            );

            const auto [width, height] = size();

            const auto local_size_x = width / device().workGroupSize();
            const auto local_size_y = height / device().workGroupSize();

            vkCmdDispatch(command_buffer_handle, local_size_x, local_size_y, 1);
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
            device().device(),
            &sampler_create_info,
            nullptr, 
            &_result_image_sampler
        ));
    }

    void SSAO::bindResultDescriptorSet()
    {
        createSampler();

        const auto ptr_result_image = colorOutputAttach(AttachmentsTraits<SSAO>::ssao);

        device().writeDescriptorSet({
            .view_handle            = ptr_result_image->view_handle,
            .sampler_handle         = _result_image_sampler,
            .set_handle             = _result_image_desc_set,
            .expected_image_layout  = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
            .binding                = 0
        });
    }
    
    void SSAO::createSSAODescriptorSet()
    {
        _ssao_desc_set_layout = vk::builders::DescriptorSetLayout(device())
            .addBinding(0, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1, VK_SHADER_STAGE_COMPUTE_BIT)
            .addBinding(1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_COMPUTE_BIT)
            .addBinding(2, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_COMPUTE_BIT)
            .build();

        _ssao_desc_set = device().allocateDescriptorSet(_ssao_desc_set_layout, "[ssao] descritor set with data for compute");

        const auto ptr_result_image = colorOutputAttach(AttachmentsTraits<SSAO>::ssao);

        device().writeDescriptorSet({
            .view_handle            = ptr_result_image->view_handle,
            .set_handle             = _ssao_desc_set,
            .expected_image_layout  = VK_IMAGE_LAYOUT_GENERAL,
            .binding                = 0
        });

        device().writeDescriptorSet({
            .buffer     = _samples_buffer.value(),
            .set_handle = _ssao_desc_set,
            .size       = static_cast<uint32_t>(_samples_buffer->size),
            .binding    = 2
        });
    }

    void SSAO::createParamsBuffer()
    {
        _params_buffer = vk::builders::Buffer(device())
            .addQueueFamilyIndex(device().queue().family_index)
            .name("[ssao] params")
            .size(sizeof(Params))
            .type(vk::BufferType::eDeviceOnly)
            .usage(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT)
            .build();

        _params_buffer->write(_params, 0);
    }

    void SSAO::createSamplesBuffer()
    {
        std::uniform_real_distribution<float>   random_floats (0.0f, 1.0f);
        std::default_random_engine              generator;

        uint32_t i = 0;

        const auto generate_vector = [&generator, &random_floats, &i] ()
        {
            const auto x = random_floats(generator) * 2.0f - 1.0f;
            const auto y = random_floats(generator) * 2.0f - 1.0f;
            const auto z = random_floats(generator);

            const auto scale = static_cast<float>(i++) / 64.0f;

            return pbrlib::math::normalize(pbrlib::math::vec3(x, y, z)) * pbrlib::math::lepr(0.1f, 1.0f, scale * scale);
        };

        constexpr auto samples_count = 64;
        std::array<pbrlib::math::vec4, samples_count> samples;

        for (auto& sample: samples)
            sample = generate_vector();

        _samples_buffer = vk::builders::Buffer(device())
            .addQueueFamilyIndex(device().queue().family_index)
            .name("[ssao] samples")
            .size(samples.size() * sizeof(pbrlib::math::vec4))
            .type(vk::BufferType::eDeviceOnly)
            .usage(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT)
            .build();

        _samples_buffer->write(std::span<const pbrlib::math::vec4>(samples), 0);

        _params.sample_count = static_cast<uint32_t>(samples.size());
    }

    void SSAO::update(const Config& config)
    {
        auto& blur_settings = _ptr_blur->settings();
        auto& ssao_settings = config.ssao;

        blur_settings.sample_count  = ssao_settings.blur_samples_count;
        blur_settings.sigma_s       = ssao_settings.spatial_sigma;
        blur_settings.sigma_l       = ssao_settings.luminance_sigma;
    }
}