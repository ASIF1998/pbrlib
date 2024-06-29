//
//  MeshNode.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 19/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <pbrlib/SceneGraph/MeshNode.hpp>

using namespace pbrlib::math;

namespace pbrlib
{
    MeshNode::MeshNode(const std::string_view name) :
        SceneItem (name),
        _ptr_mesh (nullptr)
    {}

    MeshNode::MeshNode(const std::string_view name, const std::shared_ptr<Mesh> ptr_mesh) :
        SceneItem (name),
        _ptr_mesh (ptr_mesh)
    {
        addComponent(ptr_mesh);
    }

    MeshNode::MeshNode(std::shared_ptr<Mesh> ptr_mesh) :
        SceneItem (ptr_mesh->getName()),
        _ptr_mesh (ptr_mesh)
    {
        addComponent(ptr_mesh);
    }

    void MeshNode::setMesh(std::shared_ptr<Mesh> ptr_mesh)
    {
        _ptr_mesh = ptr_mesh;
        addComponent(ptr_mesh);
    }

    std::shared_ptr<const Mesh> MeshNode::getMesh() const noexcept
    {
        return _ptr_mesh;
    }

    void MeshNode::update(const InputStay* ptr_input_stay, float delta_time, const Transform& world_transform)
    {
        SceneItem::update(ptr_input_stay, delta_time, world_transform);

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

    std::unique_ptr<MeshNode> MeshNode::make(const std::string_view name)
    {
        return make_unique<MeshNode>(name);
    }

    std::unique_ptr<MeshNode> MeshNode::make(const std::string_view name, std::shared_ptr<Mesh> ptr_mesh)
    {
        return make_unique<MeshNode>(name, ptr_mesh);
    }

    std::unique_ptr<MeshNode> MeshNode::make(std::shared_ptr<Mesh> ptr_mesh)
    {
        return make_unique<MeshNode>(ptr_mesh);
    }
}
