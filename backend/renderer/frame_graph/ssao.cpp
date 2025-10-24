#include <backend/renderer/frame_graph/ssao.hpp>

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
        RenderPass  (device),
        _ptr_blur   (ptr_blur)
    { 
        if (!ptr_blur) [[unlikely]]
            throw exception::InvalidArgument("[ssao] pointer to blur is null");

        _result_image_desc_set_layout = vk::builders::DescriptorSetLayout(device)
            .addBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_COMPUTE_BIT)
            .build();

        _result_image_desc_set = device.allocateDescriptorSet(
            _result_image_desc_set_layout, 
            "[ssao] descritor set with results"
        );
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
            .size       = 2 * sizeof(pbrlib::math::mat4)
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

        device().writeDescriptorSet ({
            .buffer     = _params_buffer.value(),
            .set_handle = _ssao_desc_set,
            .size       = static_cast<uint32_t>(_params_buffer->size),
            .binding    = 1
        });

        constexpr auto ssao_shader = "shaders/ssao/ssao.glsl.comp";

        auto new_pipeline = vk::builders::ComputePipeline(device())
            .shader(ssao_shader)
            .pipelineLayoutHandle(_pipeline_layout_handle)
            .build();

        _pipeline_handle = std::move(new_pipeline);

        return true;
    }

    void SSAO::render(vk::CommandBuffer& command_buffer)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        command_buffer.write([this] (VkCommandBuffer command_buffer_handle)
        {
            PBRLIB_PROFILING_VK_ZONE_SCOPED(device(), command_buffer_handle, "[ssao] run-pipeline");

            vkCmdBindPipeline(command_buffer_handle, VK_PIPELINE_BIND_POINT_COMPUTE, _pipeline_handle);

            const std::array sets_descriptors
            {
                descriptorSet(InputDescriptorSetTraits<SSAO>::gbuffer).first,
                _ssao_desc_set.handle(),
                context().ptr_material_manager->descriptorSet().first
            };

            vkCmdBindDescriptorSets (
                command_buffer_handle, 
                VK_PIPELINE_BIND_POINT_COMPUTE, 
                _pipeline_layout_handle, 0, 
                static_cast<uint32_t>(sets_descriptors.size()), sets_descriptors.data(), 
                0, nullptr
            );

            const std::array matrices
            {
                context().projection,
                context().view
            };

            vkCmdPushConstants (
                command_buffer_handle, 
                _pipeline_layout_handle, 
                VK_SHADER_STAGE_COMPUTE_BIT, 
                0, static_cast<uint32_t>(sizeof(pbrlib::math::mat4) * matrices.size()), matrices.data()
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
        return std::make_pair(_result_image_desc_set.handle(), _result_image_desc_set_layout.handle());
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
            &_result_image_sampler.handle()
        ));
    }

    void SSAO::bindResultDescriptorSet()
    {
        createSampler();

        const auto ptr_result_image = colorOutputAttach(AttachmentsTraits<SSAO>::ssao);

        device().writeDescriptorSet ({
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

        _ssao_desc_set = device().allocateDescriptorSet(_ssao_desc_set_layout, "[ssao] descritor-set-with-data-for-compute");

        const auto ptr_result_image = colorOutputAttach(AttachmentsTraits<SSAO>::ssao);

        device().writeDescriptorSet ({
            .view_handle            = ptr_result_image->view_handle,
            .set_handle             = _ssao_desc_set,
            .expected_image_layout  = VK_IMAGE_LAYOUT_GENERAL,
            .binding                = 0
        });

        device().writeDescriptorSet ({
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
        constexpr std::array samples
        {
            pbrlib::math::vec4(0.62944734f, -0.7290461f, 0.9057919f, 0.0f),
            pbrlib::math::vec4(0.6700171f, -0.7460264f, 0.9688677f, 0.0f),
            pbrlib::math::vec4(0.8267517f, -0.557932f, 0.6323592f, 0.0f),
            pbrlib::math::vec4(-0.38366592f, -0.80491924f, 0.5472206f, 0.0f),
            pbrlib::math::vec4(-0.44300365f, -0.62323606f, 0.5468815f, 0.0f),
            pbrlib::math::vec4(0.9857626f, 0.91501355f, 0.9964613f, 0.0f),
            pbrlib::math::vec4(0.929777f, 0.93538976f, 0.15761304f, 0.0f),
            pbrlib::math::vec4(0.45167792f, 0.9411855f, 0.9811097f, 0.0f),
            pbrlib::math::vec4(0.9143338f, -0.78027654f, 0.48537564f, 0.0f),
            pbrlib::math::vec4(0.5962117f, 0.6005609f, 0.29702944f, 0.0f),
            pbrlib::math::vec4(-0.7162274f, -0.9904331f, 0.42176127f, 0.0f),
            pbrlib::math::vec4(-0.775071f, 0.83147097f, 0.63976336f, 0.0f),
            pbrlib::math::vec4(0.5844146f, 0.7568612f, 0.9594924f, 0.0f),
            pbrlib::math::vec4(0.0073252916f, 0.31148136f, 0.7979286f, 0.0f),
            pbrlib::math::vec4(-0.9285767f, -0.27741206f, 0.84912926f, 0.0f),
            pbrlib::math::vec4(-0.5761514f, 0.86798644f, 0.6813595f, 0.0f),
            pbrlib::math::vec4(0.35747027f, -0.202523f, 0.7577401f, 0.0f),
            pbrlib::math::vec4(0.4812944f, 0.48626482f, 0.47475863f, 0.0f),
            pbrlib::math::vec4(-0.21554601f, -0.15582466f, 0.6554779f, 0.0f),
            pbrlib::math::vec4(-0.6522697f, -0.6576266f, 0.30191308f, 0.0f),
            pbrlib::math::vec4(0.4120921f, 0.5945598f, 0.031832814f, 0.0f),
            pbrlib::math::vec4(-0.36689913f, -0.44615412f, 0.8724288f, 0.0f),
            pbrlib::math::vec4(-0.90765727f, -0.7017721f, 0.09713173f, 0.0f),
            pbrlib::math::vec4(0.9881369f, 0.64691556f, 0.8219032f, 0.0f),
            pbrlib::math::vec4(0.38965714f, -0.7496345f, 0.31709945f, 0.0f),
            pbrlib::math::vec4(0.5274999f, 0.90044403f, 0.49058902f, 0.0f),
            pbrlib::math::vec4(-0.9311079f, 0.32721102f, 0.4387443f, 0.0f),
            pbrlib::math::vec4(-0.74820673f, -0.23688316f, 0.21020907f, 0.0f),
            pbrlib::math::vec4(0.5310335f, -0.89756715f, 0.7951999f, 0.0f),
            pbrlib::math::vec4(-0.9271176f, -0.6262548f, 0.4087311f, 0.0f),
            pbrlib::math::vec4(-0.020471215f, -0.08402181f, 0.44558614f, 0.0f),
            pbrlib::math::vec4(-0.02486217f, 0.2926259f, 0.79397494f, 0.0f),
            pbrlib::math::vec4(0.41872954f, 0.84174955f, 0.75468665f, 0.0f),
            pbrlib::math::vec4(0.615062f, -0.4479499f, 0.70577425f, 0.0f),
            pbrlib::math::vec4(0.35940528f, -0.9943632f, 0.65509796f, 0.0f),
            pbrlib::math::vec4(0.4214077f, -0.67477655f, 0.64396095f, 0.0f),
            pbrlib::math::vec4(-0.76200473f, -0.087934375f, 0.49836403f, 0.0f),
            pbrlib::math::vec4(0.54783416f, 0.91948783f, 0.5737546f, 0.0f),
            pbrlib::math::vec4(-0.31922865f, 0.75351477f, 0.5852677f, 0.0f),
            pbrlib::math::vec4(0.6163509f, -0.55237615f, 0.017773867f, 0.0f),
            pbrlib::math::vec4(0.50253403f, 0.64249194f, 0.25509506f, 0.0f),
            pbrlib::math::vec4(0.64168155f, 0.011914015f, 0.940074f, 0.0f),
            pbrlib::math::vec4(0.39815342f, -0.174667f, 0.89090323f, 0.0f),
            pbrlib::math::vec4(-0.15366983f, 0.9185828f, 0.58095664f, 0.0f),
            pbrlib::math::vec4(0.09443104f, -0.68388486f, 0.13862443f, 0.0f),
            pbrlib::math::vec4(0.5234624f, -0.7014121f, 0.23015606f, 0.0f),
            pbrlib::math::vec4(-0.48498356f, 0.61946905f, 0.8407172f, 0.0f),
            pbrlib::math::vec4(0.97704315f, -0.49143565f, 0.33244824f, 0.0f),
            pbrlib::math::vec4(0.6285696f, -0.40033662f, 0.24352497f, 0.0f),
            pbrlib::math::vec4(-0.97292185f, 0.8585272f, 0.21723783f, 0.0f),
            pbrlib::math::vec4(-0.3000325f, 0.81472933f, 0.19659519f, 0.0f),
            pbrlib::math::vec4(0.69693553f, -0.4978323f, 0.95501757f, 0.0f),
            pbrlib::math::vec4(0.23208928f, 0.5577954f, 0.47328883f, 0.0f),
            pbrlib::math::vec4(0.9749192f, -0.29668105f, 0.06759536f, 0.0f),
            pbrlib::math::vec4(0.6616572f, 0.58719516f, 0.5852641f, 0.0f),
            pbrlib::math::vec4(0.18900704f, 0.09944713f, 0.7327987f, 0.0f),
            pbrlib::math::vec4(0.8343873f, 0.39046574f, 0.28583896f, 0.0f),
            pbrlib::math::vec4(0.35963953f, 0.51440036f, 0.39232045f, 0.0f),
            pbrlib::math::vec4(0.5074581f, 0.123114824f, 0.38044584f, 0.0f),
            pbrlib::math::vec4(-0.583864f, 0.13564324f, 0.5273714f, 0.0f),
            pbrlib::math::vec4(-0.8482915f, -0.19158304f, 0.05395007f, 0.0f),
            pbrlib::math::vec4(-0.2944752f, 0.061595082f, 0.59282386f, 0.0f),
            pbrlib::math::vec4(0.55833435f, -0.28730977f, 0.9340106f, 0.0f),
            pbrlib::math::vec4(0.9299327f, -0.74018764f, 0.15443838f, 0.0f)
        };

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

        if (_params.radius != ssao_settings.radius)
        {
            _params.radius = ssao_settings.radius;
            _params_buffer->write(_params, 0);
        }
    }
}