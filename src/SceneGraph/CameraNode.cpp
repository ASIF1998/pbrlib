//
//  CameraNode.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 18/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include "CameraNode.hpp"

namespace pbrlib
{
    CameraNode::CameraNode(const string_view name, Scene::Node* parent) :
        Scene::Node (name, parent),
        _ptr_camera (nullptr)
    {}

    CameraNode::CameraNode(
        const string_view   name,
        Scene::Node*        parent,
        const PtrICamera&   ptr_camera
    ) :
        Scene::Node (name, parent),
        _ptr_camera (ptr_camera)
    {}

    CameraNode::CameraNode(
        const string_view                   name,
        Scene::Node*                        parent,
        const PerspectiveCamera::Builder&   camera_builder
    ) :
        Scene::Node (name, parent),
        _ptr_camera (camera_builder.ptrBuild())
    {}

    CameraNode::CameraNode(const PerspectiveCamera::Builder& camera_builder) :
        Scene::Node (),
        _ptr_camera (camera_builder.ptrBuild())
    {}
    
    CameraNode::CameraNode(const PtrICamera& ptr_camera) :
        Scene::Node (),
        _ptr_camera (ptr_camera)
    {}

    void CameraNode::setCamera(const PtrICamera& ptr_camera)
    {
        _ptr_camera = ptr_camera;
    }

    void CameraNode::setCamera(const PerspectiveCamera::Builder& camera_builder)
    {
        _ptr_camera = camera_builder.ptrBuild();
    }

    PtrICamera& CameraNode::getCamera() noexcept
    {
        return _ptr_camera;
    }

    void CameraNode::addCameraNodeModifier(ICameraNodeModifier* ptr_camera_modifire)
    {
        auto it = _camera_node_modifiers.find(ptr_camera_modifire->getType());

        if (it != end(_camera_node_modifiers)) {
            it->second.reset(ptr_camera_modifire);
        } else {
            _camera_node_modifiers.insert(make_pair(ptr_camera_modifire->getType(), ptr_camera_modifire));
        }
    }

    const PtrICamera& CameraNode::getCamera() const noexcept
    {
        return _ptr_camera;
    }

    PtrCameraNode CameraNode::make(const string_view name, Scene::Node* parent)
    {
        return make_shared<CameraNode>(name, parent);
    }

    PtrCameraNode CameraNode::make(
        const string_view   name,
        Scene::Node*        parent,
        const PtrICamera&   ptr_camera
    )
    {
        return make_shared<CameraNode>(name, parent, ptr_camera);
    }

    PtrCameraNode CameraNode::make(
        const string_view                   name,
        Scene::Node*                        parent,
        const PerspectiveCamera::Builder&   camera_builder
    )
    {
        return make_shared<CameraNode>(name, parent, camera_builder);
    }

    PtrCameraNode CameraNode::make(const PerspectiveCamera::Builder& camera_builder)
    {
        return make_shared<CameraNode>(camera_builder);
    }

    PtrCameraNode CameraNode::make(const PtrICamera& ptr_camera)
    {
        return make_shared<CameraNode>(ptr_camera);
    }

    void CameraNode::update(float delta_time, const Transform& world_transform)
    {
        for (auto s{begin(_camera_node_modifiers)}, e{end(_camera_node_modifiers)}; s != e; s++) {
            s->second->update(this, delta_time);
        }

        Scene::Node::update(delta_time, world_transform);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ICameraNodeModifier::ICameraNodeModifier(const string_view name) :
        _name(name)
    {}

    ICameraNodeModifier::~ICameraNodeModifier()
    {}

    void ICameraNodeModifier::setName(const string_view name)
    {
        _name = name;
    }

    string& ICameraNodeModifier::getName() noexcept
    {
        return _name;
    }

    const string& ICameraNodeModifier::getName() const noexcept
    {
        return _name;
    }
}
