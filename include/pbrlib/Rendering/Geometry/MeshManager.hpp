//
//  MeshManager.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 03/06/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef MeshManager_hpp
#define MeshManager_hpp

#include <unordered_map>

#include "MeshAssimp.hpp"

namespace pbrlib
{
    /**
     * @class MeshManager.
     * @brief 
     *      Данный класс необходим для загрузки файлов с сеткой,
     *      не приводящей к ненужной повторной загрузке.
    */
    class MeshManager
    {
    public:
        /**
         * @brief Конструктор.
         * 
         * @param ptr_device            указатель на устройство.
         * @param queue_family_index    индекс семейства очередей.
         * @param memory_type           тип памяти на устройстве (ptr_device).
        */
        MeshManager(const PtrDevice& ptr_device, uint32_t queue_family_index, uint32_t memory_type);
        
        /**
         * @brief Метод необходимый для загрузки сеток.
         * @details 
         *      Если сетка уже загружена, то повторной загрузки
         *      осуществляться не будет.
         * 
         * @param path путь до файла.
         * @return Указатели на сетки.
        */
        std::optional<std::vector<PtrMesh>> load(const std::string_view path);
        
        void addMesh(const PtrMesh& ptr_mesh);
        void addMesh(PtrMesh&& ptr_mesh);
        void addMesh(const Mesh::Builder& mesh_builder);
        
        template<typename VertAttribAllocType, typename IndexAllocType>
        void addMesh(const Mesh::BuilderWithData<VertAttribAllocType, IndexAllocType>& mesh_builder)
        {
            PtrMesh ptr_mesh = mesh_builder.buildPtr();
            _meshes.insert(make_pair(ptr_mesh->getName(), ptr_mesh));
        }
        
        std::optional<PtrMesh>          get(const std::string_view name);
        std::optional<const PtrMesh>    get(const std::string_view name) const;
        
    private:
        std::unordered_map<std::string, PtrMesh>    _meshes;                //!< Загруженные сетки.
        std::vector<std::string>                    _loaded_files;          //!< Файлы которые уже загружены.
        MeshAssimp                                  _assimp_mesh_loader;    //!< Загрузчик сеток.
    };
}

#endif /* MeshManager_hpp */
