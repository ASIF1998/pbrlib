#include <backend/scene/assimp_importer.hpp>
#include <backend/renderer/vulkan/device.hpp>

#include <backend/logger/logger.hpp>

#include <backend/scene/mesh_component.hpp>

#include <pbrlib/scene/scene.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

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

        Assimp::Importer importer;
        
        importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_LINE | aiPrimitiveType_POINT);
        importer.SetPropertyBool(AI_CONFIG_IMPORT_COLLADA_IGNORE_UP_DIRECTION, true);

        constexpr auto assimp_read_flags = 
                aiProcess_GenNormals    
            |   aiProcess_CalcTangentSpace 
            |   aiProcess_GenUVCoords   
            |   aiProcess_JoinIdenticalVertices 
            |   aiProcess_Triangulate;

        const auto ptr_scene = importer.ReadFile(_filename.string(), assimp_read_flags);

        if (!ptr_scene || !ptr_scene->mRootNode)
        {
            log::engine::error("[Importer] Failed load '{}': {}", _filename.string(), importer.GetErrorString());
            return false;
        }

        if (!ptr_scene->HasMaterials())
        {
            log::engine::error("[Importer]: Model '{}' don't has material.", _filename.string());
            return false;
        }

        ptr_root_item = &_ptr_scene->addItem(ptr_scene->mName.C_Str());

        importer.FreeScene();

        return true;
    }
}

namespace pbrlib
{
    void AssimpImporter::processNode(const aiScene* ptr_scene, const aiNode* ptr_node)
    {

    }
}