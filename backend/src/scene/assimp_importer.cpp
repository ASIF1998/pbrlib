#include <backend/scene/assimp_importer.hpp>
#include <backend/renderer/vulkan/device.hpp>

#include <backend/logger/logger.hpp>

#include <pbrlib/scene/scene.hpp>

#include <pbrlib/math/vec2.hpp>
#include <pbrlib/math/vec3.hpp>
#include <pbrlib/math/vec4.hpp>
#include <pbrlib/math/quat.hpp>

#include <backend/scene/mesh_component.hpp>
#include <backend/scene/material_component.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <format>

namespace pbrlib::utils
{
    pbrlib::math::mat4 cast(const aiMatrix4x4& matrix)
    {
        return pbrlib::math::mat4(
            matrix.a1, matrix.a2, matrix.a3, matrix.a4,
            matrix.b1, matrix.b2, matrix.b3, matrix.b4,
            matrix.c1, matrix.c2, matrix.c3, matrix.c4,
            matrix.d1, matrix.d2, matrix.d3, matrix.d4
        );
    }

    pbrlib::math::vec2 cast(const aiVector2D& vec)
    {
        return pbrlib::math::vec2(vec.x, vec.y);
    }

    pbrlib::math::vec3 cast(const aiVector3D& vec)
    {
        return pbrlib::math::vec3(vec.x, vec.y, vec.z);
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

namespace pbrlib
{
    class ScopedTransform
    {
    public:
        explicit ScopedTransform(AssimpImporter* ptr_importer, const pbrlib::math::mat4& transform) :
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
        AssimpImporter*     _ptr_importer;
        pbrlib::math::mat4  _prev_transform;
    };
}

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

        _ptr_root_item = &_ptr_scene->addItem(ptr_scene->mName.C_Str());

        processNode(ptr_scene, ptr_scene->mRootNode);

        importer.FreeScene();

        return true;
    }
}

namespace pbrlib
{
    static auto vertexCount(const aiScene* ptr_scene, const aiNode* ptr_node)
        -> std::pair<size_t, size_t>
    {
        constexpr size_t num_indices_per_face = 3;

        size_t index_count  = 0;
        size_t vertex_count = 0;

        for (auto i: std::views::iota(0u, ptr_node->mNumMeshes))
        {
            const auto ptr_mesh = ptr_scene->mMeshes[ptr_node->mMeshes[i]];

            if (!ptr_mesh->HasFaces() || !ptr_mesh->HasPositions() || !ptr_mesh->HasTextureCoords(0))
                continue;

            vertex_count    += ptr_mesh->mNumVertices;
            index_count     += ptr_mesh->mNumFaces * num_indices_per_face;
        }

        return std::make_pair(vertex_count, index_count);
    }

    void AssimpImporter::processNode(const aiScene* ptr_scene, const aiNode* ptr_node)
    {
        log::engine::info("[Importer]\t - Process node: {}", ptr_node->mName.C_Str());

        ScopedTransform scoped_transform (this, utils::cast(ptr_node->mTransformation));

        if (auto [vertex_count, index_count] = vertexCount(ptr_scene, ptr_node); index_count && vertex_count)
        {
            log::engine::info("[Importer]\t\t - Index count: {}", index_count);
            log::engine::info("[Importer]\t\t - Vertex count: {}", vertex_count);

            std::vector<uint32_t>           indices;
            std::vector<VertexAttribute>    attributes;

            for (auto i: std::views::iota(0u, ptr_node->mNumMeshes))
            {
                const auto ptr_mesh = ptr_scene->mMeshes[ptr_node->mMeshes[i]];

                if (!ptr_mesh->HasFaces() || !ptr_mesh->HasPositions() || !ptr_mesh->HasTextureCoords(0))
                    continue;

                std::string_view name = ptr_mesh->mName.C_Str();

                auto& item = _ptr_root_item->addItem(name);

                processMaterial(ptr_scene, ptr_scene->mMaterials[ptr_mesh->mMaterialIndex], &item);

                for (auto j: std::views::iota(0u, ptr_mesh->mNumVertices))
                {
                    auto uv = utils::cast(ptr_mesh->mTextureCoords[0][j]);

                    VertexAttribute attribute;
                    attribute.pos       = utils::cast(ptr_mesh->mVertices[j]);
                    attribute.normal    = utils::cast(ptr_mesh->mNormals[j]);
                    attribute.tangent   = utils::cast(ptr_mesh->mTangents[j]);
                    attribute.uv        = pbrlib::math::vec2(uv.x, uv.y);

                    attributes.push_back(attribute);
                }

                std::span faces (ptr_mesh->mFaces, ptr_mesh->mNumFaces);
                for (const auto& face: faces)
                {
                    indices.push_back(face.mIndices[0]);
                    indices.push_back(face.mIndices[1]);
                    indices.push_back(face.mIndices[2]);
                }

                add(name, attributes, indices, &item);

                attributes.clear();
                indices.clear();
            }
        }

        for (auto i: std::views::iota(0u, ptr_node->mNumChildren))
            processNode(ptr_scene, ptr_node->mChildren[i]);
    }

    void AssimpImporter::processMaterial (
        const aiScene*      ptr_scene, 
        const aiMaterial*   ptr_material, 
        SceneItem*          ptr_item
    )
    {
        MaterialComponent material_component
        {
            .albedo     = getTexture(ptr_scene, ptr_material, aiTextureType_DIFFUSE, 4),
            .normal_map = getTexture(ptr_scene, ptr_material, aiTextureType_NORMALS, 4),
            .metallic   = getTexture(ptr_scene, ptr_material, aiTextureType_METALNESS, 1),
            .roughness  = getTexture(ptr_scene, ptr_material, aiTextureType_DIFFUSE_ROUGHNESS, 1)
        };

        ptr_item->addComponent<MaterialComponent>(std::move(material_component));
    }

    void AssimpImporter::add (
        std::string_view                    name,
        std::span<const VertexAttribute>    attributes, 
        std::span<const uint32_t>           indices,
        SceneItem*                          ptr_item
    )
    {
        constexpr VkFlags shared_buffer_usage = 
                VK_BUFFER_USAGE_TRANSFER_DST_BIT
            |   VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;

        MeshComponent mesh =
        {
            .vertex_buffer = vk::Buffer::Builder(_ptr_device)
                .name(std::format("[VertexBuffer] {}", name))
                .addQueueFamilyIndex(_ptr_device->queue().family_index)
                .size(attributes.size_bytes())
                .usage(shared_buffer_usage | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT)
                .build(),
            .vertex_count = static_cast<uint32_t>(attributes.size()),

            .index_buffer = vk::Buffer::Builder(_ptr_device)
                .name(std::format("[IndexBuffer] {}", name))
                .addQueueFamilyIndex(_ptr_device->queue().family_index)
                .size(indices.size_bytes())
                .usage(shared_buffer_usage | VK_BUFFER_USAGE_INDEX_BUFFER_BIT)
                .build(),
            .index_count = static_cast<uint32_t>(indices.size())
        };

        mesh.vertex_buffer.write(attributes, 0);
        mesh.index_buffer.write(indices, 0);

        auto& transform = ptr_item->getComponent<TransformComponent>(); 

        ptr_item->addComponent<MeshComponent>(std::move(mesh));
        transform.transform = _current_state.transform;
    }

    vk::Image AssimpImporter::getTexture(
        const aiScene*      ptr_scene, 
        const aiMaterial*   ptr_material, 
        aiTextureType       texuture_type, 
        int32_t             channels_per_pixel
    )
    {
        aiString texture_name;
        if (ptr_material->Get(AI_MATKEY_TEXTURE(texuture_type, 0), texture_name) != aiReturn_SUCCESS)
            return getDefaultTexture(channels_per_pixel);

        if (texture_name.C_Str()[0] != '*')
            return getDefaultTexture(channels_per_pixel);

        auto ptr_texture = ptr_scene->mTextures[std::stoi(texture_name.C_Str() + 1)];

        if (!ptr_texture)
            return getDefaultTexture(channels_per_pixel);

        return getImage (
            ptr_texture, 
            channels_per_pixel, 
            std::format("[{}] - albedo", ptr_material->GetName().C_Str())
        );
    }

    vk::Image AssimpImporter::getImage(
        const aiTexture*    ptr_texture, 
        int32_t             channels_per_pixel,
        std::string_view    name
    ) const
    {
        auto ptr_compressed_image   = reinterpret_cast<uint8_t*>(ptr_texture->pcData);
        auto compressed_image_size  = ptr_texture->mHeight == 0 ? 
            ptr_texture->mWidth : 
            ptr_texture->mWidth * ptr_texture->mHeight;

        return vk::Image::Decoder(_ptr_device)
            .name(name)
            .channelsPerPixel(channels_per_pixel)
            .compressedImage(ptr_compressed_image, compressed_image_size)
            .decode();
    }

    vk::Image AssimpImporter::getDefaultTexture(int32_t channels_per_pixel)
    {
        constexpr std::array formats
        {
            VK_FORMAT_R8_UNORM,
            VK_FORMAT_R8G8_UNORM,
            VK_FORMAT_R8G8B8_UNORM,
            VK_FORMAT_R8G8B8A8_UNORM
        };

        return vk::Image::Builder(_ptr_device)
            .addQueueFamilyIndex(_ptr_device->queue().family_index)
            .fillColor(pbrlib::math::vec3(0))
            .format(formats[channels_per_pixel - 1])
            .name("Default image")
            .size(1, 1)
            .usage(VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_STORAGE_BIT)
            .build();
    }
}