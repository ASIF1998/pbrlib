#pragma once

#include <backend/renderer/vulkan/image.hpp>
#include <backend/renderer/vulkan/buffer.hpp>

#include <limits>

#include <optional>

#include <string>
#include <string_view>

#include <vector>

namespace pbrlib
{
    class SceneItem;
}

namespace pbrlib::backend
{
    struct Material final
    {
        uint32_t albedo     = std::numeric_limits<uint32_t>::max();
        uint32_t normal_map = std::numeric_limits<uint32_t>::max();
        uint32_t metallic   = std::numeric_limits<uint32_t>::max();
        uint32_t roughness  = std::numeric_limits<uint32_t>::max();
    };

    struct CompressedImageData final
    {
        const uint8_t*  ptr_data            = nullptr;
        size_t          size                = 0;
        uint8_t         channels_per_pixel  = 0;
    };

    class MaterialManager final
    {
        uint32_t getImageId(const CompressedImageData& compressed_image, std::string_view name);

    public:
        struct Bindings
        {
            enum
            {
                eImages,
                eMaterial,

                eCount
            }; 
        };

        explicit MaterialManager(vk::Device& device);

        MaterialManager(MaterialManager&& material_manager)         = delete;
        MaterialManager(const MaterialManager& material_manager)    = delete;

        ~MaterialManager();

        MaterialManager& operator = (MaterialManager&& material_manager)        = delete;
        MaterialManager& operator = (const MaterialManager& material_manager)   = delete;  

        void add (
            SceneItem*                  ptr_scene_item,
            std::string_view            material_name,
            const CompressedImageData&  albedo,
            const CompressedImageData&  normal_map,
            const CompressedImageData&  metallic,
            const CompressedImageData&  roughness
        );

        void update();

        [[nodiscard]] std::pair<VkDescriptorSet, VkDescriptorSetLayout> descriptorSet() const noexcept;

        [[nodiscard]] size_t imageCount()       const noexcept;
        [[nodiscard]] size_t materialCount()    const noexcept;

    private:
        vk::Device& _device;

        std::vector<vk::Image>  _images;
        std::vector<Material>   _materials;

        VkSampler _sampler_handle = VK_NULL_HANDLE;

        std::optional<vk::Buffer> _materials_indices_buffer;       

        bool _descriptor_set_is_changed = true;

        VkDescriptorSet         _descriptor_set_handle          = VK_NULL_HANDLE;
        VkDescriptorSetLayout   _descriptor_set_layout_handle   = VK_NULL_HANDLE;
    };
}