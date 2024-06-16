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

    using PtrMeshNode = shared_ptr<MeshNode>;

    class MeshNode :
        public SceneItem
    {
    public:
        MeshNode(const string_view name = "Mesh Node");
        MeshNode(const string_view name, const PtrMesh& ptr_mesh);
        MeshNode(const PtrMesh& ptr_mesh);

        void setMesh(const PtrMesh& ptr_mesh);

        PtrMesh&        getMesh() noexcept;
        const PtrMesh&  getMesh() const noexcept;

        virtual void update(float delta_time, const Transform& world_transform) override;

        static PtrMeshNode make(const string_view name = "Mesh Node");
        static PtrMeshNode make(const string_view name, const PtrMesh& ptr_mesh);
        static PtrMeshNode make(const PtrMesh& ptr_mesh);

    private:
        PtrMesh _ptr_mesh;
    };
}

#endif /* MeshNode_hpp */
