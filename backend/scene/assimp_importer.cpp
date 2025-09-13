#include <backend/scene/assimp_importer.hpp>
#include <backend/renderer/vulkan/device.hpp>

#include <backend/logger/logger.hpp>

#include <pbrlib/scene/scene.hpp>

#include <pbrlib/math/vec2.hpp>
#include <pbrlib/math/vec3.hpp>
#include <pbrlib/math/vec4.hpp>
#include <pbrlib/math/quat.hpp>

#include <backend/components.hpp>

#include <backend/profiling.hpp>

#include <backend/scene/material_manager.hpp>
#include <backend/scene/mesh_manager.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <format>

namespace pbrlib::backend::utils
{
    union FloatBits
    {
        float       value;
        uint32_t    bits;
    };

    /// from https://github.com/zeux/niagara.git
    uint16_t quantize(float v)
    {
        const FloatBits bits 
        {
            .value = v
        };

        int32_t s   = (bits.bits >> 16) & 0x8000;
        int32_t em  = bits.bits & 0x7fffffff;

        int32_t h = (em - (112 << 23) + (1 << 12)) >> 13;
        h = (em < (113 << 23)) ? 0 : h;
        h = (em >= (143 << 23)) ? 0x7c00 : h;
        h = (em > (255 << 23)) ? 0x7e00 : h;

        return static_cast<uint16_t>(s | h);
    }

    pbrlib::math::mat4 cast(const aiMatrix4x4& matrix)
    {
        return pbrlib::math::mat4
        (
            matrix.a1, matrix.b1, matrix.c1, matrix.d1,
            matrix.a2, matrix.b2, matrix.c2, matrix.d2,
            matrix.a3, matrix.b3, matrix.c3, matrix.d3,
            matrix.a4, matrix.b4, matrix.c4, matrix.d4
        );
    }

    pbrlib::math::vec2 cast(const aiVector2D& vec)
    {
        return pbrlib::math::vec2(vec.x, vec.y);
    }

    pbrlib::math::u16vec2 castToHalf(const pbrlib::math::vec2& vec)
    {
        const auto x = quantize(vec.x);
        const auto y = quantize(vec.y);
        return pbrlib::math::u16vec2(x, y);
    }

    pbrlib::math::vec3 cast(const aiVector3D& vec)
    {
        return pbrlib::math::vec3(vec.x, vec.y, vec.z);
    }

    pbrlib::math::u16vec3 castToHalf(const aiVector3D& vec)
    {
        const auto x = quantize(vec.x);
        const auto y = quantize(vec.y);
        const auto z = quantize(vec.z);
        return pbrlib::math::u16vec3(x, y, z);
    }
    
    pbrlib::math::vec4 cast(const aiColor3D& col)
    {
        return pbrlib::math::vec4(col.r, col.g, col.b, 0.0f);
    }

    pbrlib::math::quat cast(const aiQuaternion& quat)
    {
        return pbrlib::math::quat(quat.x, quat.y, quat.z, quat.w);
    }
}

namespace pbrlib::backend
{
    class ScopedTransform
    {
    public:
        explicit ScopedTransform(AssimpImporter* ptr_importer, const math::mat4& transform) :
            _ptr_importer           (ptr_importer),
            _prev_transform         (ptr_importer->_current_state.transform)
        {
            _ptr_importer->_current_state.transform *= transform;
        }

        ~ScopedTransform()
        {
            _ptr_importer->_current_state.transform = _prev_transform;
        }

    private:
        AssimpImporter* _ptr_importer;
        math::mat4      _prev_transform;
    };
}

namespace pbrlib::backend
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

    AssimpImporter& AssimpImporter::materialManager(MaterialManager* ptr_material_manager)
    {
        _ptr_material_manager = ptr_material_manager;
        return *this;
    }
    
    AssimpImporter& AssimpImporter::meshManager(MeshManager* ptr_mesh_manager)
    {
        _ptr_mesh_manager = ptr_mesh_manager;
        return *this;
    }

    AssimpImporter& AssimpImporter::transform(const math::mat4& matrix)
    {
        _current_state.transform = matrix;
        return *this;
    }

    bool AssimpImporter::import()
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        if (_filename.empty() || !std::filesystem::exists(_filename) || std::filesystem::is_directory(_filename)) [[unlikely]]
        {
            log::error("[importer] incorrect filename: {}", _filename.string());
            return false;
        }

        if (!_ptr_material_manager) [[unlikely]]
        {
            log::error("[importer] pointer to material manager is null");
            return false;
        }
        
        if (!_ptr_mesh_manager) [[unlikely]]
        {
            log::error("[importer] pointer to mesh manager is null");
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

        if (!ptr_scene || !ptr_scene->mRootNode) [[unlikely]]
        {
            log::error("[importer] failed load '{}': {}", _filename.string(), importer.GetErrorString());
            return false;
        }

        if (!ptr_scene->HasMaterials()) [[unlikely]]
        {
            log::error("[importer] model '{}' don't has material", _filename.string());
            return false;
        }

        _ptr_root_item = &_ptr_scene->addItem(ptr_scene->mName.C_Str());

        processNode(ptr_scene, ptr_scene->mRootNode);

        importer.FreeScene();

        return true;
    }
}

namespace pbrlib::backend
{
    static auto vertexCount(const aiScene* ptr_scene, const aiNode* ptr_node)
        -> std::pair<size_t, size_t>
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        constexpr size_t num_indices_per_face = 3;

        size_t index_count  = 0;
        size_t vertex_count = 0;

        for (auto i: std::views::iota(0u, ptr_node->mNumMeshes))
        {
            const auto ptr_mesh = ptr_scene->mMeshes[ptr_node->mMeshes[i]];

            if (!ptr_mesh->HasFaces() || !ptr_mesh->HasPositions() || !ptr_mesh->HasTextureCoords(0)) [[unlikely]]
                continue;

            vertex_count    += ptr_mesh->mNumVertices;
            index_count     += ptr_mesh->mNumFaces * num_indices_per_face;
        }

        return std::make_pair(vertex_count, index_count);
    }

    CompressedImageData getComressedImage(
        const aiScene*      ptr_scene, 
        const aiMaterial*   ptr_material, 
        aiTextureType       texuture_type, 
        uint8_t             channels_per_pixel
    )
    {
        aiString texture_name;
        if (ptr_material->Get(AI_MATKEY_TEXTURE(texuture_type, 0), texture_name) != aiReturn_SUCCESS) [[unlikely]]
            return { };

        if (texture_name.C_Str()[0] != '*') [[unlikely]]
            return { };

        auto ptr_texture = ptr_scene->mTextures[std::stoi(texture_name.C_Str() + 1)];

        if (!ptr_texture) [[unlikely]]
            return  { };

        auto ptr_compressed_image   = reinterpret_cast<uint8_t*>(ptr_texture->pcData);
        auto compressed_image_size  = ptr_texture->mHeight == 0 ? 
            ptr_texture->mWidth : 
            ptr_texture->mWidth * ptr_texture->mHeight;

        return CompressedImageData
        {
            .ptr_data           = ptr_compressed_image,
            .size               = compressed_image_size,
            .channels_per_pixel = channels_per_pixel
        };
    }

    void AssimpImporter::processNode(const aiScene* ptr_scene, const aiNode* ptr_node)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        log::info("[importer]\t - porocess node: {}", ptr_node->mName.C_Str());

        ScopedTransform scoped_transform (this, utils::cast(ptr_node->mTransformation));

        if (auto [vertex_count, index_count] = vertexCount(ptr_scene, ptr_node); index_count && vertex_count) [[likely]]
        {
            log::info("[importer]\t\t - index count: {}", index_count);
            log::info("[importer]\t\t - vertex count: {}", vertex_count);

            std::vector<uint32_t>           indices;
            std::vector<VertexAttribute>    attributes;

            for (auto i: std::views::iota(0u, ptr_node->mNumMeshes))
            {
                const auto ptr_mesh = ptr_scene->mMeshes[ptr_node->mMeshes[i]];
                if (!ptr_mesh->HasFaces() || !ptr_mesh->HasPositions() || !ptr_mesh->HasTextureCoords(0)) [[unlikely]]
                    continue;

                std::string_view name = ptr_mesh->mName.C_Str();

                auto& item = _ptr_root_item->addItem(name);

                if (const auto ptr_material = ptr_scene->mMaterials[ptr_mesh->mMaterialIndex]) [[likely]]
                {
                    auto& component = item.addComponent<components::Renderable>();
                    component.ptr_item = &item;

                    _ptr_material_manager->add(
                        &item,
                        ptr_material->GetName().C_Str(),
                        getComressedImage(ptr_scene, ptr_material, aiTextureType_DIFFUSE, 4),
                        getComressedImage(ptr_scene, ptr_material, aiTextureType_NORMALS, 4),
                        getComressedImage(ptr_scene, ptr_material, aiTextureType_METALNESS, 1),
                        getComressedImage(ptr_scene, ptr_material, aiTextureType_DIFFUSE_ROUGHNESS, 1)
                    );
                }
                else 
                    continue;

                auto& renderable = item.getComponent<components::Renderable>();

                for (auto j: std::views::iota(0u, ptr_mesh->mNumVertices))
                {
                    auto uv = utils::cast(ptr_mesh->mTextureCoords[0][j]);

                    const auto pos = utils::cast(ptr_mesh->mVertices[j]);

                    renderable.bbox.add(pos);

                    VertexAttribute attribute;
                    attribute.pos       = pos;
                    attribute.normal    = utils::castToHalf(ptr_mesh->mNormals[j]);
                    attribute.tangent   = utils::castToHalf(ptr_mesh->mTangents[j]);
                    attribute.uv        = utils::castToHalf(pbrlib::math::vec2(uv.x, uv.y));

                    attributes.push_back(attribute);
                }

                std::span faces (ptr_mesh->mFaces, ptr_mesh->mNumFaces);
                for (const auto& face: faces)
                {
                    indices.push_back(face.mIndices[0]);
                    indices.push_back(face.mIndices[1]);
                    indices.push_back(face.mIndices[2]);
                }

                log::info("[importer]\t\t - volume: {}", renderable.bbox.volume());

                auto& transform = item.getComponent<pbrlib::components::Transform>();
                transform.transform = _current_state.transform;

                _ptr_mesh_manager->add(name, attributes, indices, &item);

                attributes.clear();
                indices.clear();
            }
        }

        for (auto i: std::views::iota(0u, ptr_node->mNumChildren))
            processNode(ptr_scene, ptr_node->mChildren[i]);
    }
}