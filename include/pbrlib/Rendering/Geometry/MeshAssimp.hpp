//
//  MeshAssimp.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 03/06/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef MeshAssimp_hpp
#define MeshAssimp_hpp

#include <optional>

#include "Mesh.hpp"

#include <pbrlib/Moving/Transform.hpp>

#include <assimp/scene.h>

using namespace Assimp;

namespace pbrlib
{
    /**
     * @class MeshAssimp.
     * @brief 
     *      Данный класс необходим для загрузки сетки при помощи 
     *      библиотеки Assimp.
    */
    class MeshAssimp
    {
    private:
        using VertAttrib = Mesh::VertexAttrib;

        struct Mesh
        {
            uint32_t        vert_offset;
            uint32_t        ind_offset;
            uint32_t        num_vert;
            uint32_t        num_ind;
            AABB            bbox;
            std::string     name;
        };

        struct Asset
        {
            Buffer::BuilderWithData<VertAttrib> vert_attr;
            Buffer::BuilderWithData<uint32_t>   indices;
            std::vector<MeshAssimp::Mesh>       meshes;
        };

    private:
        friend void processNode(
            MeshAssimp::Asset*  asset,
            const aiScene*      ptr_ai_scene,
            const aiNode*       ptr_ai_node
        );

    public:
        /**
         * @brief Конструктор.
         * 
         * @param ptr_device            указатель на устройство.
         * @param queue_family_index    индекс семейства очередей.
         * @param memory_type           тип памяти на устройстве (ptr_device).
        */
        MeshAssimp(const PtrDevice& ptr_device, uint32_t queue_family_index, uint32_t memory_type);

        PtrDevice&          getDevice() noexcept;
        const PtrDevice&    getDevice() const noexcept;

        /**
         * @brief Метод необходимый для загрузки сеток.
         * 
         * @param path путь до файла.
         * @return Указатели на сетки.
        */
        std::optional<std::vector<PtrMesh>> load(const std::string_view path);

    private:
        PtrDevice _ptr_device;          //!< Указатель на устройство.
        uint32_t  _queue_family_index;  //!< Индекс семейства очередей на устройстве.
        uint32_t  _memory_type;         //!< Тип памяти на устройстве.
    };
}

#endif /* MeshAssimp_hpp */
