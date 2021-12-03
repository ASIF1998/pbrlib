//
//  MeshManager.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 03/06/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include "MeshManager.hpp"

namespace pbrlib
{
    MeshManager::MeshManager(
        const PtrDevice&    ptr_device, 
        uint32_t            queue_family_index, 
        uint32_t            memory_type
    ) :
        _assimp_mesh_loader(ptr_device, queue_family_index, memory_type)
    {}

    optional<vector<PtrMesh>> MeshManager::load(const string_view path)
    {
        for (size_t i{0}; i < _loaded_files.size(); i++) {
            if (_loaded_files[i] == path) {
                ///  Этот файл уже загружен.
                return nullopt;
            }
        }

        _loaded_files.push_back(path.data());

        optional<vector<PtrMesh>> meshes = _assimp_mesh_loader.load(path);
        
        if (meshes.has_value()) {
            auto& temp = meshes.value();

            for (size_t i{0}; i < temp.size(); i++) {
                _meshes.insert(make_pair(temp[i]->getName(), temp[i]));
            }
        }

        return meshes;
    }

    void MeshManager::addMesh(const PtrMesh& ptr_mesh)
    {
        _meshes.insert(make_pair(ptr_mesh->getName(), ptr_mesh));
    }

    void MeshManager::addMesh(PtrMesh&& ptr_mesh)
    {
        _meshes.insert(make_pair(ptr_mesh->getName(), move(ptr_mesh)));
    }

    void MeshManager::addMesh(const Mesh::Builder& mesh_builder)
    {
        PtrMesh ptr_mesh = mesh_builder.buildPtr();
        _meshes.insert(make_pair(ptr_mesh->getName(), ptr_mesh));
    }

    optional<PtrMesh> MeshManager::get(const string_view name)
    {
        auto it = _meshes.find(name.data());
        return it != end(_meshes) ? make_optional(it->second) : nullopt;
    }

    optional<const PtrMesh> MeshManager::get(const string_view name) const
    {
        auto it = _meshes.find(name.data());
        return it != end(_meshes) ? make_optional(it->second) : nullopt;
    }
}
