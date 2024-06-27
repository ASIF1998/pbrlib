//
//  MeshManager.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 03/06/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <pbrlib/Rendering/Geometry/MeshManager.hpp>

namespace pbrlib
{
    MeshManager::MeshManager(
        const Device*   ptr_device, 
        uint32_t        queue_family_index, 
        uint32_t        memory_type
    ) :
        _assimp_mesh_loader(ptr_device, queue_family_index, memory_type)
    {}

    std::optional<std::vector<std::shared_ptr<Mesh>>> MeshManager::load(const std::string_view path)
    {
        for (size_t i{0}; i < _loaded_files.size(); i++) 
        {
            if (_loaded_files[i] == path)
                return std::nullopt;
        }

        _loaded_files.push_back(path.data());

        std::optional<std::vector<std::shared_ptr<Mesh>>> meshes = _assimp_mesh_loader.load(path);
        
        if (meshes.has_value()) 
        {
            auto& temp = meshes.value();

            for (size_t i{0}; i < temp.size(); i++)
                _meshes.insert(std::make_pair(temp[i]->getName(), temp[i]));
        }

        return meshes;
    }

    void MeshManager::addMesh(std::shared_ptr<const Mesh> ptr_mesh)
    {
        _meshes.insert(std::make_pair(ptr_mesh->getName(), ptr_mesh));
    }

    void MeshManager::addMesh(const Mesh::Builder& mesh_builder)
    {
        std::shared_ptr ptr_mesh = mesh_builder.buildPtr();
        _meshes.insert(std::make_pair(ptr_mesh->getName(), ptr_mesh));
    }

    std::shared_ptr<const Mesh> MeshManager::get(const std::string_view name)
    {
        auto it = _meshes.find(name.data());
        return it != std::end(_meshes) ? it->second : nullptr;
    }
}
