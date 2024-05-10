//
//  MeshAssimp.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 03/06/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <pbrlib/Rendering/Geometry/MeshAssimp.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

namespace pbrlib
{
    /**
     * @brief Функция возвращающая количество вершин и индексов.
     * 
     * @param[in]   ptr_ai_scene            указатель на сцену.
     * @param[in]   ptr_ai_node             указатель на узел в сцене.
     * @param[out]  ptr_total_vertex_count  указатель на количество вершин в сцене.
     * @param[out]  ptr_total_index_count   указатель на количество индексов в сцене.
    */
    void countVerticesAndIndices(
        const aiScene*      ptr_ai_scene,
        aiNode const*       ptr_ai_node,
        size_t*             ptr_total_vertex_count,
        size_t*             ptr_total_index_count
    )
    {
        for (size_t i{0}; i < ptr_ai_node->mNumMeshes; i++) {
            const aiMesh* ptr_ai_mesh = ptr_ai_scene->mMeshes[ptr_ai_node->mMeshes[i]];
            const aiFace* ptr_ai_face = ptr_ai_mesh->mFaces;

            *ptr_total_vertex_count += ptr_ai_mesh->mNumVertices;
            *ptr_total_index_count  += ptr_ai_mesh->mNumFaces * ptr_ai_face->mNumIndices;
        }

        for (size_t i{0}; i < ptr_ai_node->mNumChildren; i++) {
            countVerticesAndIndices(ptr_ai_scene, ptr_ai_node->mChildren[i], ptr_total_vertex_count, ptr_total_index_count);
        }
    }

    /**
     * @brief Функция необходимая для обработки узла.
     * 
     * @param[out]  asset           хранит компоненты сеток представленных в сцене.
     * @param[in]   ptr_ai_scene    указатель на сцену.
     * @param[in]   ptr_ai_node     указатель на узел в сцене.
    */
    void processNode(
        MeshAssimp::Asset*  asset,
        const aiScene*      ptr_ai_scene,
        const aiNode*       ptr_ai_node
    )
    {
        Vec3<float> position;
        Vec2<float> uv;
        Vec3<float> normal;
        Vec3<float> tangent;

        for (size_t i{0}; i < ptr_ai_node->mNumMeshes; i++) {
            aiMesh const* ptr_ai_mesh = ptr_ai_scene->mMeshes[ptr_ai_node->mMeshes[i]];
            aiFace const* ptr_ai_face = ptr_ai_mesh->mFaces;

            auto* ptr_texture_coords = ptr_ai_mesh->mTextureCoords[0] ? ptr_ai_mesh->mTextureCoords[0] : ptr_ai_mesh->mTextureCoords[1];

            asset->meshes.push_back(MeshAssimp::Mesh{});
            MeshAssimp::Mesh& mesh = asset->meshes.back();

            mesh.ind_offset     = static_cast<uint32_t>(asset->indices.getSize() * sizeof(uint32_t));
            mesh.vert_offset    = static_cast<uint32_t>(asset->vert_attr.getSize() * sizeof(Mesh::VertexAttrib));
            mesh.num_vert       = ptr_ai_mesh->mNumVertices;
            mesh.num_ind        = ptr_ai_mesh->mNumFaces * ptr_ai_face->mNumIndices;
            mesh.name           = ptr_ai_mesh->mName.C_Str();
            mesh.bbox           = AABB::computeAABB(ptr_ai_mesh->mVertices, ptr_ai_mesh->mNumVertices);

            for (size_t j{0}; j < ptr_ai_mesh->mNumVertices; j++) {
                if (ptr_ai_mesh->mVertices) {
                    position.x = ptr_ai_mesh->mVertices[j].x;
                    position.y = ptr_ai_mesh->mVertices[j].y;
                    position.z = ptr_ai_mesh->mVertices[j].z;
                }

                if (ptr_texture_coords) {
                    uv.x = ptr_texture_coords[j].x;
                    uv.y = ptr_texture_coords[j].y;
                }

                if (ptr_ai_mesh->mNormals) {
                    normal.x = ptr_ai_mesh->mNormals[j].x;
                    normal.y = ptr_ai_mesh->mNormals[j].y;
                    normal.z = ptr_ai_mesh->mNormals[j].z;
                }

                if (ptr_ai_mesh->mTangents) {
                    tangent.x = ptr_ai_mesh->mTangents[j].x;
                    tangent.y = ptr_ai_mesh->mTangents[j].y;
                    tangent.z = ptr_ai_mesh->mTangents[j].z;
                }

                MeshAssimp::VertAttrib vert_attr = { };
                vert_attr.position  = position;
                vert_attr.normal    = normal;
                vert_attr.tangent   = tangent;
                vert_attr.uv        = uv;

                asset->vert_attr.pushBack(vert_attr);
            }

            for (size_t j{0}; j < ptr_ai_mesh->mNumFaces; j++) {
                for (size_t k{0}; k < ptr_ai_face[j].mNumIndices; k++) {
                    asset->indices.addData(ptr_ai_face[j].mIndices[k]);
                }
            }
        }

        for (size_t i{0}; i < ptr_ai_node->mNumChildren; i++) {
            processNode(asset, ptr_ai_scene, ptr_ai_node->mChildren[i]);
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    MeshAssimp::MeshAssimp(const PtrDevice& ptr_device, uint32_t queue_family_index, uint32_t memory_type) :
        _ptr_device         (ptr_device),
        _queue_family_index (queue_family_index),
        _memory_type        (memory_type)
    {}

    PtrDevice& MeshAssimp::getDevice() noexcept
    {
        return _ptr_device;
    }

    const PtrDevice& MeshAssimp::getDevice() const noexcept
    {
        return _ptr_device;
    }

    optional<vector<PtrMesh>> MeshAssimp::load(const string_view path)
    {
        Asset       asset;
        Importer    importer;

        importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_LINE | aiPrimitiveType_POINT);
        importer.SetPropertyBool(AI_CONFIG_IMPORT_COLLADA_IGNORE_UP_DIRECTION, true);

        const aiScene* ptr_ai_scene = importer.ReadFile(
            path.data(),
            aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace |
            aiProcess_GenUVCoords |
            aiProcess_FindInstances | aiProcess_JoinIdenticalVertices |
            aiProcess_ImproveCacheLocality | aiProcess_SortByPType |
            aiProcess_Triangulate
        );

        if (!ptr_ai_scene) {
            return nullopt;
        }

        if (!ptr_ai_scene->mRootNode) {
            return nullopt;
        }

        size_t total_vertex_count   = 0;
        size_t total_index_count    = 0;

        const aiNode* ptr_ai_node = ptr_ai_scene->mRootNode;

        countVerticesAndIndices(ptr_ai_scene, ptr_ai_node, &total_vertex_count, &total_index_count);

        asset.vert_attr.reserve(total_vertex_count);
        asset.indices.reserve(total_index_count);

        asset.vert_attr.addQueueFamily(_queue_family_index);
        asset.indices.addQueueFamily(_queue_family_index);

        asset.vert_attr.setMemoryTypeIndex(_memory_type);
        asset.indices.setMemoryTypeIndex(_memory_type);

        asset.vert_attr.setDevice(_ptr_device);
        asset.indices.setDevice(_ptr_device);

        asset.vert_attr.setUsage(
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT   | 
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT    |
            VK_BUFFER_USAGE_TRANSFER_DST_BIT
        );

        asset.indices.setUsage(
            VK_BUFFER_USAGE_INDEX_BUFFER_BIT    |
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT    |
            VK_BUFFER_USAGE_TRANSFER_DST_BIT
        );

        processNode(&asset, ptr_ai_scene, ptr_ai_node);

        vector<PtrMesh> meshes;

        PtrBuffer   ptr_vertex_buffer   = asset.vert_attr.buildPtr();
        PtrBuffer   ptr_index_buffer    = asset.indices.buildPtr();
        PtrMesh     ptr_mesh            = nullptr;

        meshes.reserve(asset.meshes.size());

        for (size_t i{0}; i < asset.meshes.size(); i++) {
            ptr_mesh = pbrlib::Mesh::make();

            ptr_mesh->_ptr_index_buffer             = ptr_index_buffer;
            ptr_mesh->_ptr_vertex_attrib_buffer     = ptr_vertex_buffer;
            ptr_mesh->_num_vertices                 = asset.meshes[i].num_vert;
            ptr_mesh->_vertex_attrib_buffer_offset  = asset.meshes[i].vert_offset;
            ptr_mesh->_num_indices                  = asset.meshes[i].num_ind;
            ptr_mesh->_index_buffer_offset          = asset.meshes[i].ind_offset;
            ptr_mesh->_index_type                   = VK_INDEX_TYPE_UINT32;
            ptr_mesh->_aabb                         = asset.meshes[i].bbox;
            ptr_mesh->_name                         = asset.meshes[i].name;
            ptr_mesh->_ptr_material                 = nullptr;

            meshes.push_back(ptr_mesh);
        }

        return make_optional(meshes);
    }
}
