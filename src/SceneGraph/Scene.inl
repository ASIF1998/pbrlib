//
//  Scene.inl
//  PBRLib
//
//  Created by Асиф Мамедов on 15/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

namespace pbrlib
{
    inline Scene::Node::Node(
        const string_view   name,
        const Transform&    local_transform,
        Node*               ptr_parent 
    ) :
        _ptr_parent                 (ptr_parent),
        _local_transform            (local_transform),
        _world_transform_is_current (false),
        _name                       (name)
    {}

    inline Scene::Node::Node(
        const string_view   name,
        const Transform&    local_transform,
        const Transform&    world_transform,
        Node*               ptr_parent 
    ) :
        _ptr_parent                 (ptr_parent),
        _local_transform            (local_transform),
        _world_transform            (world_transform),
        _world_transform_is_current (true),
        _name                       (name)
    {}

    inline Scene::Node::~Node()
    {}

    inline AABB& Scene::Node::getWorldAABB() noexcept
    {
        return _world_bbox;
    }

    inline const AABB& Scene::Node::getWorldAABB() const noexcept
    {
        return _world_bbox;
    }

    inline Scene::Node* Scene::Node::getParent() noexcept
    {
        return _ptr_parent;
    }

    inline const Scene::Node* Scene::Node::getParent() const noexcept
    {
        return _ptr_parent;
    }

    inline Scene::PtrNode& Scene::Node::getChild(size_t i)
    {
        return _ptr_children[i];
    }

    inline const Scene::PtrNode& Scene::Node::getChild(size_t i) const
    {
        return _ptr_children[i];
    }

    inline vector<Scene::PtrNode>& Scene::Node::getChildren() noexcept
    {
        return _ptr_children;
    }

    inline const vector<Scene::PtrNode>& Scene::Node::getChildren() const noexcept
    {
        return _ptr_children;
    }

    inline Transform& Scene::Node::getLocalTransform() noexcept
    {
        return _local_transform;
    }

    inline const Transform& Scene::Node::getLocalTransform() const noexcept
    {
        return _local_transform;
    }

    inline Transform& Scene::Node::getWorldTransform() noexcept
    {
        return _world_transform;
    }

    inline const Transform& Scene::Node::getWorldTransform() const noexcept
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

    template<typename NodeModifierType>
    inline NodeModifierType& Scene::Node::getNodeModifier()
    {
        return *dynamic_cast<NodeModifierType*>(_node_modifiers.at(typeid(NodeModifierType)).get());
    }

    template<typename NodeModifierType>
    inline const NodeModifierType& Scene::Node::getNodeModifier() const
    {
        return *dynamic_cast<NodeModifierType*>(_node_modifiers.at(typeid(NodeModifierType)).get());
    }

    inline void Scene::Node::setParent(Node* ptr_node) noexcept
    {
        _ptr_parent = ptr_node;
    }

    inline void Scene::Node::setChildren(vector<PtrNode>&& children)
    {
        swap(_ptr_children, children);
    }

    inline void Scene::Node::setChildren(const vector<PtrNode>& children)
    {
        _ptr_children = children;
    }

    inline void Scene::Node::setLocalTransform(const Transform& transform)
    {
        _local_transform = transform;
    }

    inline void Scene::Node::setWorldTransform(const Transform& transform)
    {
        _world_transform_is_current = true;
        _world_transform            = transform;
    }

    inline void Scene::Node::setWorldAABB(const AABB& bbox)
    {
        _world_bbox = bbox;
    }

    inline void Scene::Node::setNodeModifier(NodeModifier* ptr_node_modifier)
    {
        auto it = _node_modifiers.find(ptr_node_modifier->getType());

        if (it != end(_node_modifiers)) {
            it->second.reset(ptr_node_modifier);
        } else {
            _node_modifiers.insert(make_pair(ptr_node_modifier->getType(), ptr_node_modifier));
        }
    }

    inline void Scene::Node::setName(const string_view name)
    {
        _name = name;
    }

    inline bool Scene::Node::isWorldTransformCurrent() const noexcept
    {
        return _world_transform_is_current;
    }

    inline void Scene::Node::isWorldTransformCurrent(bool is_current) noexcept
    {
        _world_transform_is_current = is_current;
    }

    inline void Scene::Node::addChild(PtrNode&& child)
    {
        _ptr_children.push_back(move(child));
    }

    inline void Scene::Node::addChild(const PtrNode& child)
    {
        _ptr_children.push_back(child);
    }

    inline void Scene::Node::update(float delta_time, const Transform& transform)
    {
        for (auto s{begin(_node_modifiers)}, e{end(_node_modifiers)}; s != e; s++) {
            s->second->update(this, delta_time);
        }

        if (!_world_transform_is_current) {
            _world_transform = transform;
        }

        if (!_ptr_children.empty()) {
            Transform children_world_transform = _world_transform * _local_transform;

            _ptr_children[0]->update(delta_time, children_world_transform);
            _world_bbox = _ptr_children[0]->getWorldAABB();

            for (size_t i{1}; i < _ptr_children.size(); i++) {
                _ptr_children[i]->update(delta_time, children_world_transform);
                _world_bbox = AABB::aabbUnion(_world_bbox, _ptr_children[i]->getWorldAABB());
            }
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inline NodeModifier::NodeModifier(const string_view name) :
        _name(name)
    {}

    inline void NodeModifier::setName(const string_view name)
    {
        _name = name;
    }

    inline string& NodeModifier::getName() noexcept
    {
        return _name;
    }

    inline const string& NodeModifier::getName() const noexcept
    {
        return _name;
    }

    inline NodeModifier::~NodeModifier()
    {}
}

