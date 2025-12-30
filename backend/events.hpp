#pragma once

#include <pbrlib/config.hpp>
#include <pbrlib/math/matrix4x4.hpp>

#include <filesystem>

namespace pbrlib
{
    class Scene;
}

namespace pbrlib::backend
{
    class MeshManager;
    class MaterialManager;
}

namespace pbrlib::backend::vk
{
    class Device;
}

namespace pbrlib::backend::events
{
    struct Initialize final
    { };

    struct Finalize final
    { };
        
    struct AssimpImporter final
    {
        vk::Device* ptr_device = nullptr;
        Scene*      ptr_scene  = nullptr;

        std::filesystem::path filename;

        pbrlib::math::mat4 transform;

        MaterialManager*    ptr_material_manager   = nullptr;
        MeshManager*        ptr_mesh_manager       = nullptr;
    };

    struct UpdateSSAO final
    {
        settings::SSAO settings;
    };

    struct UpdateFXAA final
    {
        settings::FXAA settings;
    };

    struct RecompilePipeline final
    { 
        uint32_t width  = 0;
        uint32_t height = 0;
    };
}