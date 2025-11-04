#include <backend/renderer/frame_graph/filters/bilateral_blur.hpp>

#include <backend/renderer/vulkan/device.hpp>
#include <backend/renderer/vulkan/pipeline_layout.hpp>
#include <backend/renderer/vulkan/compute_pipeline.hpp>
#include <backend/renderer/vulkan/command_buffer.hpp>
#include <backend/renderer/vulkan/gpu_marker_colors.hpp>
#include <backend/renderer/vulkan/shader_compiler.hpp>
#include <backend/renderer/vulkan/image.hpp>

#include <backend/utils/align_size.hpp>
#include <backend/utils/vulkan.hpp>

#include <backend/logger/logger.hpp>

#include <pbrlib/event_system.hpp>
#include <backend/events.hpp>

namespace pbrlib::backend
{
    BilateralBlur::BilateralBlur(vk::Device& device, std::string_view output_image_name, const Settings& settings):
        Filter              (device),
        _output_image_name  (output_image_name),
        _settings           (settings)
    {
        _settings.sample_count = utils::alignSize(_settings.sample_count, 2u);
        _settings.sample_count = std::clamp<uint32_t>(_settings.sample_count, 1, 8);

        _descriptor_set_layout_handle = vk::builders::DescriptorSetLayout(device)
            .addBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_COMPUTE_BIT)
            .addBinding(1, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1, VK_SHADER_STAGE_COMPUTE_BIT)
            .build();

        _descriptor_set_handle = device.allocateDescriptorSet (
            _descriptor_set_layout_handle,
            "[bilateral-blur] input descriptor set"
        );

        constexpr VkSamplerCreateInfo sampler_create_info 
        {
            .sType          = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
            .magFilter      = VK_FILTER_LINEAR,
            .minFilter      = VK_FILTER_LINEAR,
            .addressModeU   = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
            .addressModeV   = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
            .addressModeW   = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
            .borderColor    = VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK 
        };

        VK_CHECK(vkCreateSampler(
            device.device(),
            &sampler_create_info,
            nullptr, 
            &_sampler_handle.handle()
        ));
    }

    bool BilateralBlur::init(const RenderContext& context, uint32_t width, uint32_t height)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        if (!RenderPass::init(context, width, height)) [[unlikely]]
        {
            log::error("[bilateral-blur] failed initialize");
            return false;
        }

        EventSystem::on([this] ([[maybe_unused]] const events::RecompilePipeline& event)
        {
            createPipeline();
        });

        constexpr VkPushConstantRange push_constant_range
        {
            .stageFlags = VK_SHADER_STAGE_COMPUTE_BIT,
            .size       = sizeof(Settings)
        };

        _pipeline_layout_handle = vk::builders::PipelineLayout(device())
            .addSetLayout(_descriptor_set_layout_handle)
            .pushConstant(push_constant_range)
            .build();

        const auto& input_image         = srcImage();
        const auto  ptr_output_image    = colorOutputAttach(_output_image_name);

        device().writeDescriptorSet ({
            .view_handle            = input_image.view_handle.handle(),
            .sampler_handle         = _sampler_handle,
            .set_handle             = _descriptor_set_handle,
            .expected_image_layout  = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
            .binding                = 0
        });
        
        device().writeDescriptorSet ({
            .view_handle            = ptr_output_image->view_handle.handle(),
            .set_handle             = _descriptor_set_handle,
            .expected_image_layout  = VK_IMAGE_LAYOUT_GENERAL,
            .binding                = 1
        });

        return createPipeline();
    }

    bool BilateralBlur::createPipeline()
    {
        constexpr auto blur_shader = "shaders/bilateral_blur/bilateral_blur.glsl.comp";

        auto new_pipeline = vk::builders::ComputePipeline(device())
            .shader(blur_shader)
            .pipelineLayoutHandle(_pipeline_layout_handle)
            .specializationInfo
            (
                vk::shader::SpecializationInfo(_settings.sample_count)
                    .addEntry(0, 0, sizeof(_settings.sample_count))
            )
            .build();

        _pipeline_handle = std::move(new_pipeline);

        return true;
    }

    void BilateralBlur::render(vk::CommandBuffer& command_buffer) 
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        command_buffer.write([this] (VkCommandBuffer command_buffer_handle)
        {
            PBRLIB_PROFILING_VK_ZONE_SCOPED(device(), command_buffer_handle, "[bilateral-blur] run-pipeline");

            vkCmdBindPipeline(command_buffer_handle, VK_PIPELINE_BIND_POINT_COMPUTE, _pipeline_handle);

            vkCmdBindDescriptorSets(
                command_buffer_handle, 
                VK_PIPELINE_BIND_POINT_COMPUTE, 
                _pipeline_layout_handle, 
                0, 1, &_descriptor_set_handle.handle(), 
                0, nullptr
            );

            vkCmdPushConstants(
                command_buffer_handle,
                _pipeline_layout_handle, 
                VK_SHADER_STAGE_COMPUTE_BIT, 
                0, sizeof(Settings), &_settings
            );

            const auto [width, height] = size();

            const auto group_count_x = width / device().workGroupSize();
            const auto group_count_y = height / device().workGroupSize();

            vkCmdDispatch(command_buffer_handle, group_count_x, group_count_y, 1);
        }, "[bilateral-blur] run-pipeline", vk::marker_colors::bilateral_blur);
    }

    VkPipelineStageFlags2 BilateralBlur::srcStage() const noexcept
    {
        return VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT;
    }
    
    VkPipelineStageFlags2 BilateralBlur::dstStage() const noexcept
    {
        return VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT;
    }

    std::pair<VkDescriptorSet, VkDescriptorSetLayout> BilateralBlur::resultDescriptorSet() const noexcept
    {
        return std::make_pair(VK_NULL_HANDLE, VK_NULL_HANDLE);
    }

    BilateralBlur::Settings& BilateralBlur::settings() noexcept
    {
        return _settings;
    }

    const BilateralBlur::Settings& BilateralBlur::settings() const noexcept
    {
        return _settings;
    }
}