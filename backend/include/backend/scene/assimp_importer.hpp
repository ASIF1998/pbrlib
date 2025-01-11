#pragma once

#include <filesystem>

namespace pbrlib
{
    class Scene;
}

namespace pbrlib::vk
{
    class Device;
}

namespace pbrlib
{
    class AssimpImporter
    {
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
        Scene*  _ptr_scene  = nullptr;

        std::filesystem::path _filename;
    };
}