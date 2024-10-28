//
//  Scene.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 17/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <pbrlib/scene/scene.hpp>
#include <pbrlib/scene/component.hpp>

namespace pbrlib
{
    SceneItem::SceneItem(
        const std::string_view  name,
        SceneItem*              ptr_parent 
    ) :
        _ptr_parent                 (ptr_parent),
        _world_transform_is_current (false),
        _world_aabb_is_current      (false),
        _name                       (name)
    {}

    SceneItem::~SceneItem()
    {}

    // AABB& SceneItem::getWorldAABB() noexcept
    // {
    //     return _world_bbox;
    // }

    // const AABB& SceneItem::getWorldAABB() const noexcept
    // {
    //     return _world_bbox;
    // }

    SceneItem* SceneItem::getParent() noexcept
    {
        return _ptr_parent;
    }

    const SceneItem* SceneItem::getParent() const noexcept
    {
        return _ptr_parent;
    }

    std::shared_ptr<const SceneItem> SceneItem::getChild(size_t i) const
    {
        assert(i < _children.size());
        return _children[i];
    }

    Transform& SceneItem::getLocalTransform() noexcept
    {
        return _local_transform;
    }

    const Transform& SceneItem::getLocalTransform() const noexcept
    {
        return _local_transform;
    }

    Transform& SceneItem::getWorldTransform() noexcept
    {
        return _world_transform;
    }

    const Transform& SceneItem::getWorldTransform() const noexcept
    {
        return _world_transform;
    }

    std::string& SceneItem::getName() noexcept
    {
        return _name;
    }

    const std::string& SceneItem::getName() const noexcept
    {
        return _name;
    }

    size_t SceneItem::getChildrenCount() const
    {
        return _children.size();
    }

    void SceneItem::setParent(SceneItem* ptr_parent) noexcept
    {
        _ptr_parent = ptr_parent;
    }

    void SceneItem::setChildren(std::span<std::shared_ptr<SceneItem>> children)
    {
        _children = std::vector(std::begin(children), std::end(children));
    }

    void SceneItem::setLocalTransform(const Transform& transform)
    {
        _local_transform = transform;
    }

    void SceneItem::setWorldTransform(const Transform& transform)
    {
        _world_transform_is_current = true;
        _world_transform            = transform;
    }

    // void SceneItem::setWorldAABB(const AABB& bbox)
    // {
    //     _world_aabb_is_current  = true;
    //     _world_bbox             = bbox;
    // }

    void SceneItem::setName(const std::string_view name)
    {
        _name = name;
    }

    bool SceneItem::worldTransformIsCurrent() const noexcept
    {
        return _world_transform_is_current;
    }

    void SceneItem::worldTransformIsCurrent(bool current) noexcept
    {
        _world_transform_is_current = current;
    }

    bool SceneItem::worldAABBIsCurrent() const noexcept
    {
        return _world_aabb_is_current;
    }

    void SceneItem::worldAABBIsCurrent(bool current) noexcept
    {
        _world_aabb_is_current = current;
    }

    void SceneItem::addComponent(std::shared_ptr<ComponentBase> ptr_component)
    {
        auto type   = ptr_component->getType();
        auto it     = _components.find(type);

        if (it != end(_components))
            it->second = ptr_component;
        else
            _components.insert(make_pair(type, ptr_component));
    }

    void SceneItem::addChild(std::shared_ptr<SceneItem> child)
    {
        child->setParent(this);

        for (size_t i = 0, size = _children.size(); i < size; i++) 
        {
            if (!_children[i]) 
            {
                _children[i] = child;
                return;
            }
        }

        _children.push_back(child);
    }

    std::shared_ptr<SceneItem> SceneItem::addChild(const std::string_view name)
    {
        auto child = std::make_shared<SceneItem>(name, this);

        for (size_t i = 0, size = _children.size(); i < size; i++) 
        {
            if (!_children[i]) 
            {
                _children[i] = child;
                return _children[i];
            }
        }

        _children.push_back(child);
        return _children.back();
    }

    void SceneItem::detachChild(std::shared_ptr<const SceneItem> ptr_item)
    {
        for (size_t i = 0, size = _children.size(); i < size; i++) {
            if (_children[i] == ptr_item)
                _children[i] = nullptr;
        }
    }

    void SceneItem::detachChild(const std::string_view name)
    {
        for (size_t i = 0, size = _children.size(); i < size; i++) 
        {
            if (_children[i]->_name == name)
                _children[i] = nullptr;
        }
    }

    void SceneItem::update(const InputStay* ptr_input_stay, float delta_time, const Transform& transform)
    {
        for (auto [_, ptr_component]: _components)
            ptr_component->update(ptr_input_stay, this, delta_time);

        if (!_world_transform_is_current)
            _world_transform = transform;

        if (!_children.empty()) 
        {
            Transform children_world_transform = _world_transform * _local_transform;

            for (size_t i = 0, size = _children.size(); i < size; i++)
                _children[i]->update(ptr_input_stay, delta_time, children_world_transform);

            // if (!_world_aabb_is_current) 
            // {
            //     _world_bbox = _children[0]->getWorldAABB();

            //     for (size_t i = 1, size = _children.size(); i < size; i++)
            //         _world_bbox = AABB::aabbUnion(_world_bbox, _children[i]->getWorldAABB());
            // }
        }
    }

    void SceneItem::_getVisibleList(VisibleList& out_visible_list)
    {
        /**
         * TODO: Добавить отсечение по усечённому конусу.
        */

        for (size_t i = 0, num_child = _children.size(); i < num_child; i++) 
        {
            out_visible_list.push_back(_children[i]);
            _children[i]->_getVisibleList(out_visible_list);
        }
    }

    std::unique_ptr<SceneItem> SceneItem::make(const std::string_view name, SceneItem* ptr_parent)
    {
        return make_unique<SceneItem>(name, ptr_parent);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Scene::Scene(
    //     const std::string_view  name, 
    //     const Device*           ptr_device,
    //     const DeviceQueue*      ptr_device_queue,
    //     const CommandPool*      ptr_command_pool,    
    //     uint32_t                device_local_memory_type_index,
    //     uint32_t                host_local_memory_type_index
    // ) :
    //     _ptr_root           (nullptr),
    //     _ptr_camera         (nullptr),
    //     _texture_manager    (
    //         ptr_device, 
    //         ptr_command_pool,
    //         device_local_memory_type_index, 
    //         host_local_memory_type_index, 
    //         ptr_device_queue,
    //         VK_IMAGE_TILING_OPTIMAL, 
    //         VK_SAMPLE_COUNT_1_BIT
    //     ),
    //     _mesh_manager       (ptr_device, ptr_device_queue->getFamilyIndex(), host_local_memory_type_index),
    //     _name               (name)
    // {}

    void Scene::setRoot(std::shared_ptr<SceneItem> ptr_root)
    {
        _ptr_root = ptr_root;
    }

    void Scene::setName(const std::string_view name)
    {
        _name = name;
    }

    std::shared_ptr<SceneItem> Scene::getRoot() noexcept
    {
        return _ptr_root;
    }

    std::shared_ptr<const SceneItem> Scene::getRoot() const noexcept
    {
        return _ptr_root;
    }

    std::string& Scene::getName() noexcept
    {
        return _name;
    }

    const std::string& Scene::getName() const noexcept
    {
        return _name;
    }

    // MaterialManager& Scene::getMaterialManager() noexcept
    // {
    //     return _material_manager;
    // }

    // const MaterialManager& Scene::getMaterialManager() const noexcept
    // {
    //     return _material_manager;
    // }

    // GPUTextureManager& Scene::getTextureManager() noexcept
    // {
    //     return _texture_manager;
    // }

    // const GPUTextureManager& Scene::getTextureManager() const noexcept 
    // {
    //     return _texture_manager;
    // }

    // MeshManager& Scene::getMeshManager() noexcept
    // {
    //     return _mesh_manager;
    // }

    // const MeshManager& Scene::getMeshManager() const noexcept
    // {
    //     return _mesh_manager;
    // }

    VisibleList Scene::getVisibleList()
    {
        VisibleList visible_list;

        if (_ptr_root) 
        {
            visible_list.push_back(_ptr_root);
            _ptr_root->_getVisibleList(visible_list);
        }

        return visible_list;
    }

    const VisibleList Scene::getVisibleList() const
    {
        VisibleList visible_list;

        if (_ptr_root) 
        {
            visible_list.push_back(_ptr_root);
            _ptr_root->_getVisibleList(visible_list);
        }

        return visible_list;
    }

    // std::shared_ptr<SceneItem> Scene::makePointLight(
    //     const PointLight::Builder&  light_builder,
    //     const std::string_view      name
    // )
    // {
    //     std::shared_ptr ptr_item = SceneItem::make(name);

    //     ptr_item->addComponent(light_builder.buildPtr());
    //     _point_lights.push_back(ptr_item);
    //     return _point_lights.back();
    // }

    // std::shared_ptr<SceneItem> Scene::makeSpotLight(
    //     const SpotLight::Builder&   light_builder,
    //     const std::string_view      name
    // )
    // {
    //     std::shared_ptr ptr_item = SceneItem::make(name);

    //     ptr_item->addComponent(light_builder.buildPtr());
    //     _spot_lights.push_back(ptr_item);
    //     return _spot_lights.back();
    // }

    // std::shared_ptr<SceneItem> Scene::makeDirectionLight(
    //     const DirectionLight::Builder&  light_builder,
    //     const std::string_view          name
    // )
    // {
    //     std::shared_ptr ptr_item = SceneItem::make(name);

    //     ptr_item->addComponent(light_builder.buildPtr());
    //     _dir_lights.push_back(ptr_item);
    //     return _dir_lights.back();
    // }

    // std::shared_ptr<SceneItem> Scene::makeCamera(
    //     const PerspectiveCamera::Builder&   camera_builder,
    //     const std::string_view              name
    // )    
    // {
    //     _ptr_camera = SceneItem::make(name);
    //     _ptr_camera->addComponent(camera_builder.buildPtr());
    //     return _ptr_camera;
    // }

    void Scene::update(const InputStay* ptr_input_stay, float delta_time)
    {
        const Transform t;

        if (_ptr_camera)
            _ptr_camera->update(ptr_input_stay, delta_time, t);

        if (_ptr_root)
            _ptr_root->update(ptr_input_stay, delta_time, t);
    }
}
