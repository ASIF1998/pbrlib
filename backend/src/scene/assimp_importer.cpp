#include <backend/scene/assimp_importer.hpp>
#include <backend/renderer/vulkan/device.hpp>

#include <backend/logger/logger.hpp>

#include <pbrlib/scene/scene.hpp>

namespace pbrlib
{
    AssimpImporter& AssimpImporter::device(vk::Device* ptr_device)
    {
        _ptr_device = ptr_device;
        return *this;
    }

    AssimpImporter& AssimpImporter::scene(Scene* ptr_scene)
    {
        _ptr_scene = ptr_scene;
        return *this;
    }

    AssimpImporter& AssimpImporter::filename(const std::filesystem::path& filename)
    {
        _filename = filename;
        return *this;
    }

    bool AssimpImporter::import()
    {
        if (_filename.empty() || !std::filesystem::exists(_filename) || std::filesystem::is_directory(_filename))
        {
            log::engine::error("[Importer] Incorrect filename: {}", _filename.string());
            return false;
        }

        return true;
    }
}