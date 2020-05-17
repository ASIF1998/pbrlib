//
//  SpotLightNode.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 17/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include "SpotLightNode.hpp"

namespace pbrlib
{
    SpotLightNode::SpotLightNode(const string_view name, Scene::Node* parent) :
        Scene::Node (name, parent),
        _ptr_light  (nullptr)
    {}

    SpotLightNode::SpotLightNode(
        const string_view           name, 
        Scene::Node*                parent, 
        const SpotLight::Builder&   light_builder
    ) :
        Scene::Node (name, parent),
        _ptr_light  (light_builder.buildPtr())
    {}

    SpotLightNode::SpotLightNode(
        const string_view   name, 
        Scene::Node*        parent, 
        const PtrSpotLight& ptr_light
    ) :
        Scene::Node (name, parent),
        _ptr_light  (ptr_light)
    {}

    SpotLightNode::SpotLightNode(const SpotLight::Builder& light_builder) :
        Scene::Node (),
        _ptr_light  (light_builder.buildPtr())
    {}

    SpotLightNode::SpotLightNode(const PtrSpotLight& ptr_light) :
        Scene::Node (),
        _ptr_light  (ptr_light)
    {}

    SpotLightNode::~SpotLightNode()
    {}

    void SpotLightNode::setLight(const SpotLight::Builder& light_builder)
    {
        _ptr_light = light_builder.buildPtr();
    }

    void SpotLightNode::setLight(const PtrSpotLight& ptr_light)
    {
        _ptr_light = ptr_light;
    }

    void SpotLightNode::addSpotLightNodeModifier(ISpotLightNodeModifier* ptr_light_node_modifire)
    {
        auto it = _spot_light_node_modifier.find(ptr_light_node_modifire->getType());

        if (it != end(_spot_light_node_modifier)) {
            it->second.reset(ptr_light_node_modifire);
        } else {
            _spot_light_node_modifier.insert(make_pair(ptr_light_node_modifire->getType(), ptr_light_node_modifire));
        }
    }

    PtrSpotLight& SpotLightNode::getLight() noexcept
    {
        return _ptr_light;
    }

    const PtrSpotLight& SpotLightNode::getLight() const noexcept
    {
        return _ptr_light;
    }

    void SpotLightNode::update(float delta_time, const Transform& world_transform)
    {
        for (auto s{(begin(_spot_light_node_modifier))}, e{(end(_spot_light_node_modifier))}; s != e; s++) {
            s->second->update(this, delta_time);
        }

        Scene::Node::update(delta_time, world_transform);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ISpotLightNodeModifier::ISpotLightNodeModifier(const string_view name) :
        _name(name)
    {}

    ISpotLightNodeModifier::~ISpotLightNodeModifier()
    {}

    void ISpotLightNodeModifier::setName(const string_view name)
    {
        _name = name;
    }

    string& ISpotLightNodeModifier::getName() noexcept
    {
        return _name;
    }

    const string& ISpotLightNodeModifier::getName() const noexcept
    {
        return _name;
    }
}
