#pragma once

#include <filesystem>

#include <assimp/scene.h>

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
        void processNode(const aiScene* ptr_scene, const aiNode* ptr_node);

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
    };
}