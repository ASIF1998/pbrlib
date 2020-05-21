//
//  MeshNode.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 19/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include "MeshNode.hpp"

namespace pbrlib
{
    MeshNode::MeshNode(const string_view name, Scene::Node* parent) :
        Scene::Node (name, parent),
        _ptr_mesh   (nullptr)
    {}

    MeshNode::MeshNode(
        const string_view   name,
        Scene::Node*        parent,
        const PtrMesh&      ptr_mesh
    ) :
        Scene::Node (name, parent),
        _ptr_mesh   (ptr_mesh)
    {}

    MeshNode::MeshNode(const PtrMesh& ptr_mesh) :
        Scene::Node (),
        _ptr_mesh   (ptr_mesh)
    {}

    void MeshNode::setMesh(const PtrMesh& ptr_mesh)
    {
        _ptr_mesh = ptr_mesh;
    }

    void MeshNode::addMeshNodeModifier(IMeshNodeModifier* ptr_mesh_modifire)
    {
        auto it = _mesh_node_modifiers.find(ptr_mesh_modifire->getType());

        if (it != end(_mesh_node_modifiers)) {
            it->second.reset(ptr_mesh_modifire);
        } else {
            _mesh_node_modifiers.insert(make_pair(ptr_mesh_modifire->getType(), ptr_mesh_modifire));
        }
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
        for (auto s{begin(_mesh_node_modifiers)}, e{end(_mesh_node_modifiers)}; s != e; s++) {  
            s->second->update(this, delta_time);
        }

        Scene::Node::update(delta_time, world_transform);

        /// Если мировой ограничивающий объём не является корректным и
        /// есть меш, то дополняем мировой ограничивающий объём объёмом
        /// меша в мировом пространстве.
        if (!Scene::Node::_world_aabb_is_current && _ptr_mesh) {
            Vec3<float> p;
            const AABB& bbox    = _ptr_mesh->aabb;
            Transform   t       = Scene::Node::_world_transform * Scene::Node::_local_transform;
            size_t      i       = 0;

            if (Scene::Node::_ptr_children.empty()) {
                Scene::Node::_world_bbox    = AABB(t(bbox.corner(0)));
                i                           = 1;
            }

            for (; i < 8; i++) {
                p                           = bbox.corner(i);
                Scene::Node::_world_bbox    = AABB::aabbUnion(_world_bbox, t(p));
            }
        }
    }

    PtrMeshNode MeshNode::make(const string_view name, Scene::Node* parent)
    {
        return make_shared<MeshNode>(name, parent);
    }

    PtrMeshNode MeshNode::make(
        const string_view   name,
        Scene::Node*        parent,
        const PtrMesh&      ptr_mesh
    )
    {
        return make_shared<MeshNode>(name, parent, ptr_mesh);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    IMeshNodeModifier::IMeshNodeModifier(const string_view name) :
        _name(name)
    {}

    IMeshNodeModifier::~IMeshNodeModifier()
    {}

    void IMeshNodeModifier::setName(const string_view name)
    {
        _name = name;
    }

    string& IMeshNodeModifier::getName() noexcept
    {
        return _name;
    }

    const string& IMeshNodeModifier::getName() const noexcept
    {
        return _name;
    }
}
