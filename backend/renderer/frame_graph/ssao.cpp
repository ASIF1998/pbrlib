#include <backend/renderer/frame_graph/ssao.hpp>

#include <backend/profiling.hpp>

#include <backend/logger/logger.hpp>

#include <backend/renderer/vulkan/device.hpp>
#include <backend/renderer/vulkan/render_pass.hpp>
#include <backend/renderer/vulkan/compute_pipeline.hpp>

namespace pbrlib::backend
{
    SSAO::~SSAO()
    {
        const auto device_handle = _ptr_device->device();

        vkDestroyPipeline(device_handle, _pipeline_handle, nullptr);
    }

    bool SSAO::init(vk::Device& device, const RenderContext& context)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        if (!RenderPass::init(device, context))
        {
            log::error("[ssao] failed initialize");
            return false;
        }

        const auto* ptr_result_attachment = colorOutputAttach(SSAOAttachmentsName::result);

        _pipeline_layout = vk::PipelineLayout::Builder(*_ptr_device)
            .build();

        return rebuild();
    }

    bool SSAO::rebuild()
    {
        constexpr auto ssao_shader = "shaders/ssao/ssao.glsl.comp";

        auto prev_handle = _pipeline_handle;

        _pipeline_handle = vk::builders::ComputePipeline(*_ptr_device)
            .shader(ssao_shader)
            .pipelineLayoutHandle(_pipeline_layout->handle)
            .build();

        return true;
    }

    void SSAO::render(size_t item_id, vk::CommandBuffer& command_buffer)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;
    }

    void SSAO::prePass(vk::CommandBuffer& command_buffer)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;
    }

    void SSAO::postPass(vk::CommandBuffer& command_buffer)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;
    }

    VkPipelineStageFlags2 SSAO::srcStage() const noexcept
    {
        return VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT;
    }

    VkPipelineStageFlags2 SSAO::dstStage() const noexcept
    {
        return VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT;
    }
}