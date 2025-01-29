#pragma once

#include <backend/renderer/vulkan/image.hpp>

#include <pbrlib/math/matrix4x4.hpp>

#include <assimp/scene.h>

#include <filesystem>
#include <span>

namespace pbrlib
{
    class Scene;
    class SceneItem;

    struct VertexAttribute;
}

namespace pbrlib
{
    class AssimpImporter
    {
        friend class ScopedTransform;

        void processNode (const aiScene* ptr_scene, const aiNode* ptr_node);

        void processMaterial (
            const aiScene*      ptr_scene, 
            const aiMaterial*   ptr_material, 
            SceneItem*          ptr_item
        );

        vk::Image getTexture(
            const aiScene*      ptr_scene, 
            const aiMaterial*   ptr_material, 
            aiTextureType       texuture_type, 
            int32_t             channels_per_pixel
        );

        vk::Image getImage(
            const aiTexture*    ptr_texture, 
            int32_t             channels_per_pixel,
            std::string_view    name
        ) const;

        vk::Image getDefaultTexture(int32_t channels_per_pixel);

        void add(
            std::string_view                    name,
            std::span<const VertexAttribute>    attributes, 
            std::span<const uint32_t>           indices,
            SceneItem*                          ptr_item
        );

    public:
        AssimpImporter() = default;

        AssimpImporter(AssimpImporter&& importer)       = delete;
        AssimpImporter(const AssimpImporter& importer)  = delete;

        AssimpImporter& operator = (AssimpImporter&& importer)      = delete;
        AssimpImporter& operator = (const AssimpImporter& importer) = delete;

        AssimpImporter& device(vk::Device* ptr_device);
        AssimpImporter& scene(Scene* ptr_scene);
        AssimpImporter& filename(const std::filesystem::path& filename);

        bool import();

    private:
        vk::Device* _ptr_device = nullptr;
        Scene*      _ptr_scene  = nullptr;

        std::filesystem::path _filename;

        SceneItem* _ptr_root_item = nullptr;

        struct 
        {
            pbrlib::math::mat4  transform;
            SceneItem*          ptr_scene_item = nullptr;
        } _current_state;
    };
}