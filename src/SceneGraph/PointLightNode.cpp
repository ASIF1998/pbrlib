//
//  PointLightNode.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 17/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include "PointLightNode.hpp"

namespace pbrlib
{
    PointLightNode::PointLightNode(const string_view name, Scene::Node* parent) :
        Scene::Node (name, parent),
        _ptr_light  (nullptr)
    {}

    PointLightNode::PointLightNode(
        const string_view           name,
        Scene::Node*                parent,
        const PointLight::Builder&  light_builder
    ) :
        Scene::Node (name, parent),
        _ptr_light  (light_builder.buildPtr())
    {}

    PointLightNode::PointLightNode(
        const string_view   name,
        Scene::Node*        parent,
        const PtrPointLight&   ptr_light
    ) :
        Scene::Node (name, parent),
        _ptr_light  (ptr_light)
    {}

    PointLightNode::PointLightNode(const PointLight::Builder& light_builder) :
        Scene::Node (),
        _ptr_light  (light_builder.buildPtr())
    {}

    PointLightNode::PointLightNode(const PtrPointLight& ptr_light) :
        Scene::Node (),
        _ptr_light  (ptr_light)
    {}

    PointLightNode::~PointLightNode()
    {}

    void PointLightNode::setLight(const PointLight::Builder& light_builder)
    {
        _ptr_light = light_builder.buildPtr();
    }

    void PointLightNode::setLight(const PtrPointLight& ptr_light)
    {
        _ptr_light = ptr_light;
    }

    void PointLightNode::addPointLightNodeModifier(IPointLightNodeModifier* ptr_light_node_modifire)
    {
        auto it = _point_light_node_modifier.find(ptr_light_node_modifire->getType());

        if (it != end(_point_light_node_modifier)) {
            it->second.reset(ptr_light_node_modifire);
        } else {
            _point_light_node_modifier.insert(make_pair(ptr_light_node_modifire->getType(), ptr_light_node_modifire));
        }
    }

    PtrPointLight& PointLightNode::getLight() noexcept
    {
        return _ptr_light;
    }

    const PtrPointLight& PointLightNode::getLight() const noexcept
    {
        return _ptr_light;
    }

    void PointLightNode::update(float delta_time, const Transform& world_transform)
    {
        for (auto s{(begin(_point_light_node_modifier))}, e{(end(_point_light_node_modifier))}; s != e; s++) {
            s->second->update(this, delta_time);
        }

        Scene::Node::update(delta_time, world_transform);
    }

    PtrPointLightNode PointLightNode::make(
        const string_view   name,
        Scene::Node*        parent
    )
    {
        return make_shared<PointLightNode>(name, parent);
    }

    PtrPointLightNode PointLightNode::make(
        const string_view           name,
        Scene::Node*                parent,
        const PointLight::Builder&  light_builder
    )
    {
        return make_shared<PointLightNode>(name, parent, light_builder);
    }

    PtrPointLightNode PointLightNode::make(
        const string_view       name,
        Scene::Node*            parent,
        const PtrPointLight&    ptr_light
    )
    {
        return make_shared<PointLightNode>(name, parent, ptr_light);
    }

    PtrPointLightNode PointLightNode::make(const PointLight::Builder& light_builder)
    {
        return make_shared<PointLightNode>(light_builder);
    }

    PtrPointLightNode PointLightNode::make(const PtrPointLight& ptr_light)
    {
        return make_shared<PointLightNode>(ptr_light);
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    IPointLightNodeModifier::IPointLightNodeModifier(const string_view name) :
        _name(name)
    {}

    IPointLightNodeModifier::~IPointLightNodeModifier()
    {}

    void IPointLightNodeModifier::setName(const string_view name)
    {
        _name = name;
    }

    string& IPointLightNodeModifier::getName() noexcept
    {
        return _name;
    }

    const string& IPointLightNodeModifier::getName() const noexcept
    {
        return _name;
    }
}
