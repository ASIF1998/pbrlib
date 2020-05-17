//
//  Scene.inl
//  PBRLib
//
//  Created by Асиф Мамедов on 15/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

namespace pbrlib
{
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

    template<typename NodeModifierType>
    inline bool Scene::Node::hasNodeModifier() const
    {
        auto it = _node_modifiers.find(typeid(NodeModifierType));
        return it != end(_node_modifiers);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template<typename NodeModifierType>
    inline type_index INodeModifier::getTypeIndex()
    {
        return typeid(NodeModifierType);
    }
}

