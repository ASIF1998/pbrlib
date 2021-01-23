//
//  Scene.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 17/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include "Scene.hpp"

#include "Component.hpp"
#include "Script.hpp"

namespace pbrlib
{
    Scene::Node::Node(
        const string_view   name,
        Node*               ptr_parent 
    ) :
        _ptr_parent                 (ptr_parent),
        _world_transform_is_current (false),
        _world_aabb_is_current      (false),
        _name                       (name)
    {}

    Scene::Node::~Node()
    {}

    AABB& Scene::Node::getWorldAABB() noexcept
    {
        return _world_bbox;
    }

    const AABB& Scene::Node::getWorldAABB() const noexcept
    {
        return _world_bbox;
    }

    Scene::Node* Scene::Node::getParent() noexcept
    {
        return _ptr_parent;
    }

    const Scene::Node* Scene::Node::getParent() const noexcept
    {
        return _ptr_parent;
    }

    Scene::PtrNode& Scene::Node::getChild(size_t i)
    {
        return _ptr_children[i];
    }

    const Scene::PtrNode& Scene::Node::getChild(size_t i) const
    {
        return _ptr_children[i];
    }

    vector<Scene::PtrNode>& Scene::Node::getChildren() noexcept
    {
        return _ptr_children;
    }

    const vector<Scene::PtrNode>& Scene::Node::getChildren() const noexcept
    {
        return _ptr_children;
    }

    Transform& Scene::Node::getLocalTransform() noexcept
    {
        return _local_transform;
    }

    const Transform& Scene::Node::getLocalTransform() const noexcept
    {
        return _local_transform;
    }

    Transform& Scene::Node::getWorldTransform() noexcept
    {
        return _world_transform;
    }

    const Transform& Scene::Node::getWorldTransform() const noexcept
    {
        return _world_transform;
    }

    string& Scene::Node::getName() noexcept
    {
        return _name;
    }

    const string& Scene::Node::getName() const noexcept
    {
        return _name;
    }

    void Scene::Node::setParent(Node* ptr_node) noexcept
    {
        _ptr_parent = ptr_node;
    }

    void Scene::Node::setChildren(vector<PtrNode>&& children)
    {
        swap(_ptr_children, children);
    }

    void Scene::Node::setChildren(const vector<PtrNode>& children)
    {
        _ptr_children = children;
    }

    void Scene::Node::setLocalTransform(const Transform& transform)
    {
        _local_transform = transform;
    }

    void Scene::Node::setWorldTransform(const Transform& transform)
    {
        _world_transform_is_current = true;
        _world_transform            = transform;
    }

    void Scene::Node::setWorldAABB(const AABB& bbox)
    {
        _world_aabb_is_current = true;
        _world_bbox = bbox;
    }

    void Scene::Node::setName(const string_view name)
    {
        _name = name;
    }

    bool Scene::Node::worldTransformIsCurrent() const noexcept
    {
        return _world_transform_is_current;
    }

    void Scene::Node::worldTransformIsCurrent(bool current) noexcept
    {
        _world_transform_is_current = current;
    }

    bool Scene::Node::worldAABBIsCurrent() const noexcept
    {
        return _world_aabb_is_current;
    }

    void Scene::Node::worldAABBIsCurrent(bool current) noexcept
    {
        _world_aabb_is_current = current;
    }

    void Scene::Node::addComponent(const PtrComponent& ptr_component)
    {
        auto type = ptr_component->getType();
        auto it = _components.find(type);

        if (it != end(_components)) {
            it->second = ptr_component;
        } else {
            _components.insert(make_pair(type, ptr_component));
        }
    }

    void Scene::Node::addScript(const PtrScript& ptr_script)
    {
        auto type = ptr_script->getType();
        auto it = _scripts.find(type);

        if (it != end(_scripts)) {
            it->second = ptr_script;
        } else {
            _scripts.insert(make_pair(type, ptr_script));
        }
    }

    void Scene::Node::addChild(PtrNode&& child)
    {
        child->setParent(this);

        for (size_t i{0}, size{_ptr_children.size()}; i < size; i++) {
            if (!_ptr_children[i]) {
                swap(_ptr_children[i], child);
                return ;
            }
        }

        _ptr_children.push_back(move(child));
    }

    void Scene::Node::addChild(const PtrNode& child)
    {
        child->setParent(this);

        for (size_t i{0}, size{_ptr_children.size()}; i < size; i++) {
            if (!_ptr_children[i]) {
                _ptr_children[i] = child;
                return ;
            }
        }

        _ptr_children.push_back(child);
    }

    Scene::PtrNode& Scene::Node::addChild(const string_view node_name)
    {
        PtrNode child = make_shared<Node>(node_name, this);

        for (size_t i{0}, size{_ptr_children.size()}; i < size; i++) {
            if (!_ptr_children[i]) {
                _ptr_children[i] = child;
                return _ptr_children[i];
            }
        }

        _ptr_children.push_back(child);
        return _ptr_children.back();
    }

    void Scene::Node::detachChild(const PtrNode& ptr_node)
    {
        for (size_t i{0}, size{_ptr_children.size()}; i < size; i++) {
            if (_ptr_children[i] == ptr_node) {
                _ptr_children[i] = nullptr;
            }
        }
    }


    void Scene::Node::detachChild(const string_view name)
    {
        for (size_t i{0}, size{_ptr_children.size()}; i < size; i++) {
            if (_ptr_children[i]->_name == name) {
                _ptr_children[i] = nullptr;
            }
        }
    }

    void Scene::Node::update(float delta_time, const Transform& transform)
    {
        for (auto s{begin(_scripts)}, e{end(_scripts)}; s != e; s++) {
            s->second->update(this, delta_time);
        }

        if (!_world_transform_is_current) {
            _world_transform = transform;
        }

        if (!_ptr_children.empty())  {
            Transform children_world_transform = _world_transform * _local_transform;
            
            for (size_t i{0}, size{_ptr_children.size()}; i < size; i++) {
                _ptr_children[i]->update(delta_time, children_world_transform);
            }

            if (!_world_aabb_is_current) {
                _world_bbox = _ptr_children[0]->getWorldAABB();

                for (size_t i{1}, size{_ptr_children.size()}; i < size; i++) {
                    _world_bbox = AABB::aabbUnion(_world_bbox, _ptr_children[i]->getWorldAABB());
                }
            }
        }
    }

    void Scene::Node::_getVisibleList(Scene::VisibleList& out_visible_list)
    {
        /**
         * TODO: Добавить отсечение по усечённому конусу.
        */

       for (size_t i{0}, num_child{_ptr_children.size()}; i < num_child; i++) {
           out_visible_list.push_back(_ptr_children[i]);
           _ptr_children[i]->_getVisibleList(out_visible_list);
       }
    }

    Scene::PtrNode Scene::Node::make(const string_view name, Node* parent)
    {
        return make_shared<Node>(name, parent);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Scene::Scene(
        const string_view       name, 
        const PtrDevice&        ptr_device,
        const PtrDeviceQueue&   ptr_device_queue,
        const PtrCommandPool&   ptr_command_pool,    
        uint32_t                device_local_memory_type_index,
        uint32_t                host_local_memory_type_index
    ) :
        _ptr_root_node      (nullptr),
        _ptr_camera_node    (nullptr),
        _texture_manager    (
            ptr_device, 
            ptr_command_pool,
            device_local_memory_type_index, 
            host_local_memory_type_index, 
            ptr_device_queue,
            VK_IMAGE_TILING_OPTIMAL, 
            VK_SAMPLE_COUNT_1_BIT
        ),
        _mesh_manager       (ptr_device, ptr_device_queue->getFamilyIndex(), host_local_memory_type_index),
        _name               (name)
    {}

    void Scene::setRootNode(const PtrNode& ptr_node)
    {
        _ptr_root_node = ptr_node;
    }

    void Scene::setRootNode(PtrNode&& ptr_node)
    {
        swap(_ptr_root_node, ptr_node);
    }

    void Scene::setName(const string_view name)
    {
        _name = name;
    }

    Scene::PtrNode& Scene::getRootNode() noexcept
    {
        return _ptr_root_node;
    }

    const Scene::PtrNode& Scene::getRootNode() const noexcept
    {
        return _ptr_root_node;
    }

    string& Scene::getName() noexcept
    {
        return _name;
    }

    const string& Scene::getName() const noexcept
    {
        return _name;
    }

    MaterialManager& Scene::getMaterialManager() noexcept
    {
        return _material_manager;
    }

    const MaterialManager& Scene::getMaterialManager() const noexcept
    {
        return _material_manager;
    }

    GPUTextureManager& Scene::getTextureManager() noexcept
    {
        return _texture_manager;
    }

    const GPUTextureManager& Scene::getTextureManager() const noexcept 
    {
        return _texture_manager;
    }

    MeshManager& Scene::getMeshManager() noexcept
    {
        return _mesh_manager;
    }

    const MeshManager& Scene::getMeshManager() const noexcept
    {
        return _mesh_manager;
    }

    Scene::VisibleList Scene::getVisibleList()
    {
        VisibleList visible_list;
        
        if (_ptr_root_node) {
            visible_list.push_back(_ptr_root_node);
            _ptr_root_node->_getVisibleList(visible_list);
        }

        return visible_list;
    }

    const Scene::VisibleList Scene::getVisibleList() const
    {
        VisibleList visible_list;
        
        if (_ptr_root_node) {
            visible_list.push_back(_ptr_root_node);
            _ptr_root_node->_getVisibleList(visible_list);
        }

        return visible_list;
    }

    Scene::PtrNode& Scene::makePointLight(
        const PointLight::Builder&   light_builder,
        const string_view            name
    )
    {
        PtrNode ptr_node = Scene::Node::make(name);

        ptr_node->addComponent(light_builder.buildPtr());
        _point_light_nodes.push_back(ptr_node);
        return _point_light_nodes.back();
    }

    Scene::PtrNode& Scene::makeSpotLight(
        const SpotLight::Builder&   light_builder,
        const string_view           name
    )
    {
        PtrNode ptr_node = Scene::Node::make(name);

        ptr_node->addComponent(light_builder.buildPtr());
        _spot_light_nodes.push_back(ptr_node);
        return _spot_light_nodes.back();
    }

    Scene::PtrNode& Scene::makeDirectionLight(
        const DirectionLight::Builder&  light_builder,
        const string_view               name
    )
    {
        PtrNode ptr_node = Scene::Node::make(name);

        ptr_node->addComponent(light_builder.buildPtr());
        _dir_light_nodes.push_back(ptr_node);
        return _dir_light_nodes.back();
    }

    Scene::PtrNode& Scene::makeCamera(
        const PerspectiveCamera::Builder&   camera_builder,
        const string_view                   name
    )    
    {
        _ptr_camera_node = Scene::Node::make(name);
        _ptr_camera_node->addComponent(camera_builder.buildPtr());
        return _ptr_camera_node;
    }

    void Scene::update(float delta_time)
    {
        const Transform t;
        
        if (_ptr_root_node) {
            _ptr_root_node->update(delta_time, t);
        }
    }
}
