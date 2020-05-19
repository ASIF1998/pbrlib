//
//  MeshNode.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 19/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef MeshNode_hpp
#define MeshNode_hpp

#include "../Rendering/Geometry/Mesh.hpp"
#include "Scene.hpp"

namespace pbrlib
{
    class MeshNode;
    class IMeshNodeModifier;

    using PtrMeshNode           = shared_ptr<MeshNode>;
    using PtrIMeshNodeModifier  = unique_ptr<IMeshNodeModifier>;

    class MeshNode :
        public Scene::Node
    {
    public:
        MeshNode(
            const string_view   name    = "Mesh Node",
            Scene::Node*        parent  = nullptr
        );

        MeshNode(
            const string_view   name,
            Scene::Node*        parent,
            const PtrMesh&      ptr_mesh
        );

        MeshNode(const PtrMesh& ptr_mesh);

        void setMesh(const PtrMesh& ptr_mesh);

        void addMeshNodeModifier(IMeshNodeModifier* ptr_mesh_modifire);

        PtrMesh&        getMesh() noexcept;
        const PtrMesh&  getMesh() const noexcept;

        template<typename MeshNodeModifier>
        inline MeshNodeModifier& getMeshNodeModifier();

        template<typename MeshNodeModifier>
        inline const MeshNodeModifier& getMeshNodeModifier() const;

        template<typename MeshNodeModifier>
        inline bool hasMeshNodeModifier() const;

        virtual void update(float delta_time, const Transform& world_transform) override;

        static PtrMeshNode make(
            const string_view   name    = "Mesh Node",
            Scene::Node*        parent  = nullptr
        );

        static PtrMeshNode make(
            const string_view   name,
            Scene::Node*        parent,
            const PtrMesh&      ptr_mesh
        );

        static PtrMeshNode make(const PtrMesh& ptr_mesh);

    private:
        PtrMesh                                             _ptr_mesh;
        unordered_map<type_index, PtrIMeshNodeModifier>     _mesh_node_modifiers;
    };

    class IMeshNodeModifier
    {
    public:
        IMeshNodeModifier(const string_view name = "Mesh Node Modifier");
        virtual ~IMeshNodeModifier();

        virtual void update(MeshNode* ptr_node, float delta_time) = 0;

        void setName(const string_view name);

        string&             getName() noexcept;
        const string&       getName() const noexcept;
        
        virtual type_index  getType() const = 0;

    private:
        string _name;
    };
}

#include "MeshNode.inl"

#endif /* MeshNode_hpp */
