#include <backend/scene/material_manager.hpp>
#include <backend/components.hpp>

#include <backend/renderer/vulkan/device.hpp>
#include <backend/renderer/vulkan/pipeline_layout.hpp>

#include <backend/profiling.hpp>

#include <backend/utils/vulkan.hpp>

#include <pbrlib/scene/scene.hpp>
#include <pbrlib/exceptions.hpp>

#include <pbrlib/math/vec3.hpp>

#include <format>

#include <array>
#include <ranges>

namespace pbrlib::backend
{
    MaterialManager::MaterialManager(vk::Device& device) :
        _device (device)
    {
        constexpr auto format   = VK_FORMAT_R8G8B8A8_UNORM;
        constexpr auto usage    = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

        _images.push_back (
            vk::builders::Image(_device)
                .addQueueFamilyIndex(_device.queue().family_index)
                .fillColor(math::vec3(0))
                .format(format)
                .name("default-image")
                .size(1, 1)
                .usage(usage)
                .build()
        );

        constexpr VkSamplerCreateInfo sampler_create_info
        {
            .sType      = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
            .magFilter  = VK_FILTER_LINEAR,
            .minFilter  = VK_FILTER_LINEAR,
        };

        VK_CHECK(vkCreateSampler(
            _device.device(),
            &sampler_create_info,
            nullptr,
            &_sampler_handle
        ));

        constexpr auto stages  = VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_COMPUTE_BIT;

        constexpr auto max_image_count = 2500;
            
        _descriptor_set_layout_handle = vk::builders::DescriptorSetLayout(_device)
            .addBinding(Bindings::eImages, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, max_image_count, stages)
            .addBinding(Bindings::eMaterial, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, stages)
            .build();

        _descriptor_set_handle = _device.allocateDescriptorSet(_descriptor_set_layout_handle, "[material-system] images");
    }

    MaterialManager::~MaterialManager()
    {
        const auto device_handle = _device.device();

        vkDeviceWaitIdle(device_handle);

        vkDestroyDescriptorSetLayout(device_handle, _descriptor_set_layout_handle, nullptr);
        vkFreeDescriptorSets(device_handle, _device.descriptorPool(), 1, &_descriptor_set_handle);
        vkDestroySampler(device_handle, _sampler_handle, nullptr);
    }

    void MaterialManager::add (
        SceneItem*                  ptr_scene_item,
        std::string_view            material_name,
        const CompressedImageData&  albedo,
        const CompressedImageData&  normal_map,
        const CompressedImageData&  metallic,
        const CompressedImageData&  roughness
    )
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        _descriptor_set_is_changed = true;
        
        if (ptr_scene_item && ptr_scene_item->hasComponent<components::Renderable>()) [[likely]]
        {
            auto& renderable = ptr_scene_item->getComponent<components::Renderable>();
            
            renderable.material_id = static_cast<uint32_t>(_materials.size());
        }
        else [[unlikely]]
            throw exception::InvalidState("[material-material] scene item doesn't have component::Renderable");

        auto& material = _materials.emplace_back();

        material.albedo     = getImageId(albedo, std::format("[{}] - albedo", material_name));
        material.normal_map = getImageId(normal_map, std::format("[{}] - normal-map", material_name));
        material.metallic   = getImageId(metallic, std::format("[{}] - metallic", material_name));
        material.roughness  = getImageId(roughness, std::format("[{}] - roughness", material_name));
    }

    uint32_t MaterialManager::getImageId(const CompressedImageData& compressed_image, std::string_view name)
    {
        if (!compressed_image.ptr_data || !compressed_image.channels_per_pixel || !compressed_image.size) [[unlikely]]
            return 0;

        const auto image_id = static_cast<uint32_t>(_images.size());

        _images.push_back (
            vk::decoders::Image(_device)
                .name(name)
                .channelsPerPixel(compressed_image.channels_per_pixel)
                .compressedImage(compressed_image.ptr_data, compressed_image.size)
                .decode()
        );

        return image_id;
    }

    void MaterialManager::update()
    {
        if (_descriptor_set_is_changed) [[unlikely]]
        {
            for (const auto i: std::views::iota(0u, _images.size()))
            {
                _device.writeDescriptorSet ({
                    .view_handle            = _images[i].view_handle,
                    .sampler_handle         = _sampler_handle,
                    .set_handle             = _descriptor_set_handle,
                    .expected_image_layout  = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                    .binding                = Bindings::eImages,
                    .array_element          = i
                });
            }

            _materials_indices_buffer = vk::builders::Buffer(_device)
                .addQueueFamilyIndex(_device.queue().family_index)
                .name("[material-system] material-indices")
                .size(_materials.size() * sizeof(Material))
                .type(vk::BufferType::eDeviceOnly)
                .usage(VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT)
                .build();

            _materials_indices_buffer->write(std::span<const Material>(_materials), 0);

            _device.writeDescriptorSet ({
                .buffer     = _materials_indices_buffer.value(),
                .set_handle = _descriptor_set_handle,
                .size       = static_cast<uint32_t>(_materials_indices_buffer->size),
                .binding    = Bindings::eMaterial
            });

            _descriptor_set_is_changed = false;
        }
    }

    std::pair<VkDescriptorSet, VkDescriptorSetLayout> MaterialManager::descriptorSet() const noexcept
    {
        return std::make_pair(_descriptor_set_handle, _descriptor_set_layout_handle.get());
    }

    size_t MaterialManager::imageCount() const noexcept
    {
        return _images.size();
    }

    size_t MaterialManager::materialCount() const noexcept
    {
        return _materials.size();
    }
}