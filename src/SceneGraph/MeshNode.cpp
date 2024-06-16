//
//  MeshNode.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 19/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <pbrlib/SceneGraph/MeshNode.hpp>

namespace pbrlib
{
    MeshNode::MeshNode(const string_view name) :
        SceneItem (name),
        _ptr_mesh (nullptr)
    {}

    MeshNode::MeshNode(const string_view name, const PtrMesh& ptr_mesh) :
        SceneItem (name),
        _ptr_mesh (ptr_mesh)
    {
        addComponent(ptr_mesh);
    }

    MeshNode::MeshNode(const PtrMesh& ptr_mesh) :
        SceneItem (ptr_mesh->getName()),
        _ptr_mesh (ptr_mesh)
    {
        addComponent(ptr_mesh);
    }

    void MeshNode::setMesh(const PtrMesh& ptr_mesh)
    {
        _ptr_mesh = ptr_mesh;
        addComponent(ptr_mesh);
    }

    PtrMesh& MeshNode::getMesh() noexcept
    {
        return _ptr_mesh;
    }

    const PtrMesh& MeshNode::getMesh() const noexcept
    {
        return _ptr_mesh;
    }

    void MeshNode::update(float delta_time, const Transform& world_transform)
    {
        SceneItem::update(delta_time, world_transform);

        /// Если мировой ограничивающий объём не является корректным и
        /// есть меш, то дополняем мировой ограничивающий объём объёмом
        /// меша в мировом пространстве.
        if (!_world_aabb_is_current && _ptr_mesh) {
            Vec3<float> p;
            const AABB& bbox    = _ptr_mesh->getAABB();

            Transform   t       = _world_transform * _local_transform;
            size_t      i       = 0;

            if (_ptr_children.empty()) {
                _world_bbox     = AABB(t(bbox.corner(0)));
                i               = 1;
            }

            for (; i < 8; i++) {
                p           = bbox.corner(i);
                _world_bbox = AABB::aabbUnion(_world_bbox, t(p));
            }
        }
    }

    PtrMeshNode MeshNode::make(const string_view name)
    {
        return make_shared<MeshNode>(name);
    }

    PtrMeshNode MeshNode::make(const string_view name, const PtrMesh& ptr_mesh)
    {
        return make_shared<MeshNode>(name, ptr_mesh);
    }

    PtrMeshNode MeshNode::make(const PtrMesh& ptr_mesh)
    {
        return make_shared<MeshNode>(ptr_mesh);
    }
}
