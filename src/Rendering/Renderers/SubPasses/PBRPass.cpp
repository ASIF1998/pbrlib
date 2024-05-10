//
//  PBRPass.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 19/07/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <pbrlib/Rendering/Renderers/SubPasses/PBRPass.hpp>

#include <pbrlib/Rendering/VulkanWrapper/PhysicalDevice.hpp>

#include <pbrlib/Rendering/VulkanWrapper/ComputePipeline.hpp>
#include <pbrlib/Rendering/VulkanWrapper/ShaderModule.hpp>
#include <pbrlib/Rendering/VulkanWrapper/PipelineLayout.hpp>
#include <pbrlib/Rendering/VulkanWrapper/DescriptorSet.hpp>

#include <pbrlib/Rendering/VulkanWrapper/CommandBuffer.hpp>


#include <pbrlib/Rendering/VulkanWrapper/Image.hpp>

#include <pbrlib/Rendering/Cameras/CameraBase.hpp>

#include <pbrlib/Rendering/Renderers/Shaders/PointLightData.h>
#include <pbrlib/Rendering/Renderers/Shaders/SpotLightData.h>
#include <pbrlib/Rendering/Renderers/Shaders/DirectionLightData.h>
#include <pbrlib/Rendering/Renderers/Shaders/NumLights.h>

#include <pbrlib/Rendering/Renderers/Shaders/CameraData.h>

#include <pbrlib/Util/enumCast.hpp>

#include <pbrlib/Rendering/Renderers/SubPasses/spv/PBRPass.glsl.comp.spv.h>

#include <algorithm>

using namespace std;

namespace pbrlib
{
    enum class PBRPassBindings :
        uint32_t
    {
        OutImage = 0,
        PositionAndMetallic,
        NormalAndRoughness,
        AlbedoAndBakedAO,
        TangentAndAnisotropy,
        NumLights,
        PointLightsData,
        SpotLightsData,
        DirectionLightsData,
        CameraData
    };

    enum class ConstantID :
        uint32_t
    {
        DistributionFunctionID = 0,
        GeometryFunctionID,
        FresnelApproximationFunctionID,
        PBR_OtherOptions,

        Num
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PBRPass::PBRPass(
        const PtrDevice&            ptr_device, 
        const PtrPhysicalDevice&    ptr_physical_device,
        const PtrDeviceQueue&       ptr_queue,
        const PtrDescriptorPool&    ptr_descriptor_pool,
        const PtrSampler&           ptr_sampler,
        const Optionals&            optionals
    ) :
        _ptr_device_queue (ptr_queue)
    {
        assert(ptr_queue);

        uint32_t queue_family_index = _ptr_device_queue->getFamilyIndex();
        
        _ptr_sampler = ptr_sampler;

        ShaderModule::Builder               build_shader;
        Buffer::Builder<PointLightData>     build_point_lights_data_buffer;
        Buffer::Builder<SpotLightData>      build_spot_lights_data_buffer;
        Buffer::Builder<DirectionLightData> build_direction_lights_data_buffer;
        Buffer::Builder<NumLights>          build_num_lights_buffer;
        Buffer::Builder<CameraData>         build_camera_data_buffer;

        DescriptorSetLayoutBindings bindings (ptr_device, 0);

        build_shader.setDevice(ptr_device);
        build_shader.setShaderCode(reinterpret_cast<const uint32_t*>(pbrpass_comp_spirv), sizeof(pbrpass_comp_spirv));
        build_shader.setShaderType(VK_SHADER_STAGE_COMPUTE_BIT);
        build_shader.setSpecializationInfoSizeData(sizeof(Optionals));
        build_shader.setSpecializationInfoNumMapEntries(utils::enumCast(ConstantID::Num));

        build_point_lights_data_buffer.setDevice(ptr_device);
        build_point_lights_data_buffer.setMemoryTypeIndex(ptr_physical_device->getMemoryTypeIndex(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT));
        build_point_lights_data_buffer.setSize(MAX_POINT_LIGHT);
        build_point_lights_data_buffer.setUsage(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
        build_point_lights_data_buffer.addQueueFamily(queue_family_index);

        build_spot_lights_data_buffer.setDevice(ptr_device);
        build_spot_lights_data_buffer.setMemoryTypeIndex(ptr_physical_device->getMemoryTypeIndex(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT));
        build_spot_lights_data_buffer.setSize(MAX_SPOT_LIGHT);
        build_spot_lights_data_buffer.setUsage(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
        build_spot_lights_data_buffer.addQueueFamily(queue_family_index);

        build_direction_lights_data_buffer.setDevice(ptr_device);
        build_direction_lights_data_buffer.setMemoryTypeIndex(ptr_physical_device->getMemoryTypeIndex(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT));
        build_direction_lights_data_buffer.setSize(MAX_DIRECTION_LIGHT);
        build_direction_lights_data_buffer.setUsage(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
        build_direction_lights_data_buffer.addQueueFamily(queue_family_index);

        build_num_lights_buffer.setDevice(ptr_device);
        build_num_lights_buffer.setMemoryTypeIndex(ptr_physical_device->getMemoryTypeIndex(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT));
        build_num_lights_buffer.setSize(1);
        build_num_lights_buffer.setUsage(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
        build_num_lights_buffer.addQueueFamily(queue_family_index);

        build_camera_data_buffer.setDevice(ptr_device);
        build_camera_data_buffer.setMemoryTypeIndex(ptr_physical_device->getMemoryTypeIndex(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT));
        build_camera_data_buffer.setSize(1);
        build_camera_data_buffer.setUsage(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
        build_camera_data_buffer.addQueueFamily(queue_family_index);

        bindings.addBinding(utils::enumCast(PBRPassBindings::OutImage), VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, VK_SHADER_STAGE_COMPUTE_BIT);
        bindings.addBinding(utils::enumCast(PBRPassBindings::PositionAndMetallic), VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_COMPUTE_BIT);
        bindings.addBinding(utils::enumCast(PBRPassBindings::NormalAndRoughness), VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_COMPUTE_BIT);
        bindings.addBinding(utils::enumCast(PBRPassBindings::AlbedoAndBakedAO), VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_COMPUTE_BIT);
        bindings.addBinding(utils::enumCast(PBRPassBindings::TangentAndAnisotropy), VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_COMPUTE_BIT);
        bindings.addBinding(utils::enumCast(PBRPassBindings::NumLights), VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_COMPUTE_BIT);
        bindings.addBinding(utils::enumCast(PBRPassBindings::PointLightsData), VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_COMPUTE_BIT);
        bindings.addBinding(utils::enumCast(PBRPassBindings::SpotLightsData), VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_COMPUTE_BIT);
        bindings.addBinding(utils::enumCast(PBRPassBindings::DirectionLightsData), VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_COMPUTE_BIT);
        bindings.addBinding(utils::enumCast(PBRPassBindings::CameraData), VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_COMPUTE_BIT);

        ShaderModule        compute_shader  = build_shader.build();
        SpecializationInfo& spec_info       = compute_shader.getSpecializationInfo();

        spec_info.addMapEntry(&optionals._distrib_func, sizeof(optionals._distrib_func), utils::enumCast(ConstantID::DistributionFunctionID));
        spec_info.addMapEntry(&optionals._geom_func, sizeof(optionals._geom_func), utils::enumCast(ConstantID::GeometryFunctionID));
        spec_info.addMapEntry(&optionals._fresnel_approx, sizeof(optionals._fresnel_approx), utils::enumCast(ConstantID::FresnelApproximationFunctionID));
        spec_info.addMapEntry(&optionals._other_options.mask, sizeof(optionals._other_options.mask), utils::enumCast(ConstantID::PBR_OtherOptions));

        _ptr_pipeline       = ComputePipeline::make(compute_shader, PipelineLayout::make(ptr_device, DescriptorSetLayout::make(move(bindings))));
        _ptr_descriptor_set = DescriptorSet::make(ptr_descriptor_pool, _ptr_pipeline->getPipelineLayout()->getDescriptorSetLayouts()[0]);

        _ptr_uniform_point_lights_data_buffer       = build_point_lights_data_buffer.buildPtr();
        _ptr_uniform_spot_lights_data_buffer        = build_spot_lights_data_buffer.buildPtr();
        _ptr_uniform_direction_lights_data_buffer   = build_direction_lights_data_buffer.buildPtr();
        _ptr_uniform_num_lights_buffer              = build_num_lights_buffer.buildPtr();

        _ptr_uniform_camera_data_buffer = build_camera_data_buffer.buildPtr();

        _ptr_descriptor_set->writeBuffer(
            *_ptr_uniform_num_lights_buffer, 
            0, _ptr_uniform_num_lights_buffer->getSize(),
            utils::enumCast(PBRPassBindings::NumLights), VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER
        );

        _ptr_descriptor_set->writeBuffer(
            *_ptr_uniform_point_lights_data_buffer, 
            0, _ptr_uniform_point_lights_data_buffer->getSize(),
            utils::enumCast(PBRPassBindings::PointLightsData), VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER
        );

        _ptr_descriptor_set->writeBuffer(
            *_ptr_uniform_spot_lights_data_buffer, 
            0, _ptr_uniform_spot_lights_data_buffer->getSize(),
            utils::enumCast(PBRPassBindings::SpotLightsData), VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER
        );

        _ptr_descriptor_set->writeBuffer(
            *_ptr_uniform_direction_lights_data_buffer,
            0, _ptr_uniform_direction_lights_data_buffer->getSize(),
            utils::enumCast(PBRPassBindings::DirectionLightsData), VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER
        );

        _ptr_descriptor_set->writeBuffer(
            *_ptr_uniform_camera_data_buffer, 
            0, _ptr_uniform_camera_data_buffer->getSize(),
            utils::enumCast(PBRPassBindings::CameraData), VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER
        );
    }

    PBRPass::PBRPass(
        const PtrDevice&            ptr_device, 
        const PtrPhysicalDevice&    ptr_physical_device,
        const PtrDeviceQueue&       ptr_queue,
        const PtrDescriptorPool&    ptr_descriptor_pool,
        const ImageView&            position_and_metallic_image_view,
        const ImageView&            normal_and_roughness_image_view,
        const ImageView&            albedo_and_baked_AO_image_view,
        const ImageView&            anisotropy_image_view,
        const PtrSampler&           ptr_sampler,
        const Optionals&            optionals
    ) :
        PBRPass(ptr_device, ptr_physical_device, ptr_queue, ptr_descriptor_pool, ptr_sampler, optionals)
    {
        _ptr_position_and_metallic_image_view   = &position_and_metallic_image_view;
        _ptr_normal_and_roughness_image_view    = &normal_and_roughness_image_view;
        _ptr_albedo_and_baked_AO_image_view     = &albedo_and_baked_AO_image_view;
        _ptr_anisotropy_image_view              = &anisotropy_image_view;

        for (uint32_t binding {1} ; binding < 5; binding++) {
            _ptr_descriptor_set->writeImageView(
                *_ptr_images_views[binding - 1],
                VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                *_ptr_sampler,
                binding,
                VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER
            );
        }
    }

    void PBRPass::draw(
        const Scene::PtrNode&           ptr_camera,
        const PtrPrimaryCommandBuffer&  ptr_command_buffer,
        const vector<Scene::PtrNode>    point_lights,
        const vector<Scene::PtrNode>    spot_lights,
        const vector<Scene::PtrNode>    direction_lights
    )
    {
        const CameraBase& camera = ptr_camera->getComponent<CameraBase>();

        VkExtent3D  out_image_size  = _out_image_view->getImage()->getImageInfo().image_extend;
        NumLights   num_lights;

        num_lights.point    = static_cast<uint32_t>(std::min<size_t>(MAX_POINT_LIGHT, point_lights.size()));
        num_lights.spot     = static_cast<uint32_t>(std::min<size_t>(MAX_SPOT_LIGHT, spot_lights.size()));
        num_lights.direct   = static_cast<uint32_t>(std::min<size_t>(MAX_DIRECTION_LIGHT, direction_lights.size()));

        vector<PointLightData>      point_lights_data       (num_lights.point);
        vector<SpotLightData>       spot_lights_data        (num_lights.spot);
        vector<DirectionLightData>  direction_lights_data   (num_lights.direct);

        CameraData camera_data = { };
        camera_data.position = (ptr_camera->getWorldTransform() * ptr_camera->getLocalTransform())(ptr_camera->getComponent<CameraBase>().getPosition());

        /// Point Lights.
        for (size_t i{0}; i < num_lights.point; i++) {
            const PointLight&   point_light     = point_lights[i]->getComponent<PointLight>();
            Transform           light_transform = camera.getView() * point_lights[i]->getWorldTransform() * point_lights[i]->getLocalTransform();

            point_lights_data[i].color_and_intensity    = Vec4<float>(point_light.getColor(), point_light.getIntensity());
            point_lights_data[i].position               = light_transform(point_light.getPosition());
        }

        /// Spot Lights.
        for (size_t i{0}; i < num_lights.spot; i++) {
            const SpotLight&    spot_light      = spot_lights[i]->getComponent<SpotLight>();
            Transform           light_transform = camera.getView() * spot_lights[i]->getWorldTransform() * spot_lights[i]->getLocalTransform();

            spot_lights_data[i].color_and_intensity         = Vec4<float>(spot_light.getColor(), spot_light.getIntensity());
            spot_lights_data[i].position_and_inner_radius   = Vec4<float>(light_transform(spot_light.getPosition()), spot_light.getInnerRadius());
            spot_lights_data[i].direction_and_outer_radius  = Vec4<float>(spot_light.getDirection(), spot_light.getOuterRadius());
        }

        /// Direction Lights.
        for (size_t i{0}; i < num_lights.direct; i++) {
            const DirectionLight& direction_light = direction_lights[i]->getComponent<DirectionLight>();

            direction_lights_data[i].color_and_intensity    = Vec4<float>(direction_light.getColor(), direction_light.getIntensity());
            direction_lights_data[i].direction_to_light     = direction_light.getDirectionToLight();
        }

        _ptr_uniform_num_lights_buffer->getDeviceMemory()->setData(&num_lights, 1);
        _ptr_uniform_camera_data_buffer->getDeviceMemory()->setData(&camera_data, 1);

        if (num_lights.point) {
            _ptr_uniform_point_lights_data_buffer->getDeviceMemory()->setData(point_lights_data);
        }

        if (num_lights.spot) {
            _ptr_uniform_spot_lights_data_buffer->getDeviceMemory()->setData(spot_lights_data);
        }

        if (num_lights.direct) {
            _ptr_uniform_direction_lights_data_buffer->getDeviceMemory()->setData(direction_lights_data);
        }

        _ptr_descriptor_set->writeImageView(
            *_out_image_view,
            VK_IMAGE_LAYOUT_GENERAL,
            utils::enumCast(PBRPassBindings::OutImage), VK_DESCRIPTOR_TYPE_STORAGE_IMAGE
        );

        ptr_command_buffer->reset();
        ptr_command_buffer->begin();

        ptr_command_buffer->bindToPipeline(getPipeline());

        ptr_command_buffer->bindDescriptorSet(_ptr_pipeline, *_ptr_descriptor_set);
        ptr_command_buffer->dispatch(out_image_size.width, out_image_size.height, 1);
        
        ptr_command_buffer->end();

        _ptr_device_queue->submit(*ptr_command_buffer);
        _ptr_device_queue->waitIdle();
    }
}
