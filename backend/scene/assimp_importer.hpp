#pragma once

#include <backend/renderer/vulkan/image.hpp>

#include <pbrlib/math/matrix4x4.hpp>
#include <pbrlib/math/aabb.hpp>

#include <assimp/scene.h>

#include <filesystem>
#include <span>

namespace pbrlib
{
    class Scene;
    class SceneItem;
}

namespace pbrlib::backend
{
    class   MaterialManager;
    class   MeshManager;
    struct  VertexAttribute;
}

namespace pbrlib::backend
{
    class AssimpImporter final
    {
        friend class ScopedTransform;

        void processNode(const aiScene* ptr_scene, const aiNode* ptr_node);

    public:
        AssimpImporter() = default;

        AssimpImporter(AssimpImporter&& importer)       = delete;
        AssimpImporter(const AssimpImporter& importer)  = delete;

        AssimpImporter& operator = (AssimpImporter&& importer)      = delete;
        AssimpImporter& operator = (const AssimpImporter& importer) = delete;

        AssimpImporter& device(vk::Device* ptr_device)                          noexcept;
        AssimpImporter& scene(Scene* ptr_scene)                                 noexcept;
        AssimpImporter& materialManager(MaterialManager* ptr_material_manager)  noexcept;
        AssimpImporter& meshManager(MeshManager* ptr_mesh_manager)              noexcept;
        AssimpImporter& transform(const math::mat4& matrix)                     noexcept;
        AssimpImporter& filename(const std::filesystem::path& filename);

        bool import();

    private:
        vk::Device* _ptr_device = nullptr;
        Scene*      _ptr_scene  = nullptr;

        std::filesystem::path _filename;

        SceneItem* _ptr_root_item = nullptr;

        struct 
        {
            math::mat4 transform        = math::mat4(1.0f);
            SceneItem* ptr_scene_item   = nullptr;
        } _current_state;

        MaterialManager*    _ptr_material_manager   = nullptr;
        MeshManager*        _ptr_mesh_manager       = nullptr;
    };
}