#pragma once

#include <backend/scene/mesh_component.hpp>

#include <pbrlib/math/matrix4x4.hpp>

#include <assimp/scene.h>

#include <filesystem>
#include <span>

namespace pbrlib
{
    class Scene;
    class SceneItem;
}

namespace pbrlib::vk
{
    class Device;
}

namespace pbrlib
{
    class AssimpImporter
    {
        friend class ScopedTransform;

        void processNode(const aiScene* ptr_scene, const aiNode* ptr_node);

        void add(
            std::string_view                    name,
            std::span<const VertexAttribute>    attributes, 
            std::span<const uint32_t>           indices
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

        SceneItem* ptr_root_item = nullptr;

        struct 
        {
            pbrlib::math::mat4 transform;
        } _current_state;
    };
}