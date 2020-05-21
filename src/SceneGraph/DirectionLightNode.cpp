//
//  DirectionLightNode.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 17/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include "DirectionLightNode.hpp"

namespace pbrlib
{
    DirectionLightNode::DirectionLightNode(const string_view name, Scene::Node* parent) :
        Scene::Node (name, parent),
        _ptr_light  (nullptr)
    {}

    DirectionLightNode::DirectionLightNode(
        const string_view               name,
        Scene::Node*                    parent,
        const DirectionLight::Builder&  light_builder
    ) :
        Scene::Node (name, parent),
        _ptr_light  (light_builder.buildPtr())
    {}

    DirectionLightNode::DirectionLightNode(
        const string_view           name,
        Scene::Node*                parent,
        const PtrDirectionLight&    ptr_light
    ) :
        Scene::Node (name, parent),
        _ptr_light  (ptr_light)
    {}

    DirectionLightNode::DirectionLightNode(const DirectionLight::Builder& light_builder) :
        Scene::Node (),
        _ptr_light  (light_builder.buildPtr())
    {}

    DirectionLightNode::DirectionLightNode(const PtrDirectionLight& ptr_light) :
        Scene::Node (),
        _ptr_light  (ptr_light)
    {}

    void DirectionLightNode::setLight(const DirectionLight::Builder& light_builder)
    {
        _ptr_light = light_builder.buildPtr();
    }

    void DirectionLightNode::setLight(const PtrDirectionLight& ptr_light)
    {
        _ptr_light = ptr_light;
    }

    void DirectionLightNode::addDirLightNodeModifier(IDirectionLightNodeModifier* ptr_light_node_modifire)
    {
        auto it = _dir_light_node_modifiers.find(ptr_light_node_modifire->getType());

        if (it != end(_dir_light_node_modifiers)) {
            it->second.reset(ptr_light_node_modifire);
        } else {
            _dir_light_node_modifiers.insert(make_pair(ptr_light_node_modifire->getType(), ptr_light_node_modifire));
        }
    }

    PtrDirectionLight& DirectionLightNode::getLight() noexcept
    {
        return _ptr_light;
    }

    const PtrDirectionLight& DirectionLightNode::getLight() const noexcept
    {
        return _ptr_light;
    }

    void DirectionLightNode::update(float delta_time, const Transform& world_transform)
    {
        for (auto s{begin(_dir_light_node_modifiers)}, e{end(_dir_light_node_modifiers)}; s != e; s++) {
            s->second->update(this, delta_time);
        }

        Scene::Node::update(delta_time, world_transform);
    }

    PtrDirectionLightNode DirectionLightNode::make(const string_view name, Scene::Node* parent)
    {
        return make_shared<DirectionLightNode>(name, parent);
    }

    PtrDirectionLightNode DirectionLightNode::make(
        const string_view               name,
        Scene::Node*                    parent,
        const DirectionLight::Builder&  light_builder
    )
    {
        return make_shared<DirectionLightNode>(name, parent, light_builder);
    }

    PtrDirectionLightNode DirectionLightNode::make(
        const string_view           name,
        Scene::Node*                parent,
        const PtrDirectionLight&    ptr_light
    )
    {
        return make_shared<DirectionLightNode>(name, parent, ptr_light);
    }

    PtrDirectionLightNode DirectionLightNode::make(const DirectionLight::Builder& light_builder)
    {
        return make_shared<DirectionLightNode>(light_builder);
    }

    PtrDirectionLightNode DirectionLightNode::make(const PtrDirectionLight& ptr_light)
    {
        return make_shared<DirectionLightNode>(ptr_light);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    IDirectionLightNodeModifier::IDirectionLightNodeModifier(const string_view name) :
        _name(name)
    {}

    IDirectionLightNodeModifier::~IDirectionLightNodeModifier()
    {}

    void IDirectionLightNodeModifier::setName(const string_view name) 
    {
        _name = name;
    }

    string& IDirectionLightNodeModifier::getName() noexcept
    {
        return _name;
    }

    const string& IDirectionLightNodeModifier::getName() const noexcept
    {
        return _name;
    }
}
