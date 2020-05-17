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
    DirectionLightNode::DirectionLightNode() :
        _ptr_light(nullptr)
    {}

    DirectionLightNode::DirectionLightNode(const PtrDirectionLight& ptr_light) :
        _ptr_light(ptr_light)
    {}

    void DirectionLightNode::setLight(const DirectionLight::Builder& light_builder)
    {
        _ptr_light = light_builder.buildPtr();
    }

    void DirectionLightNode::setLight(const PtrDirectionLight& ptr_light)
    {
        _ptr_light = ptr_light;
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
