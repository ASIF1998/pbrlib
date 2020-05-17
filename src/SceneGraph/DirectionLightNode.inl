//
//  DirectionLightNode.inl
//  PBRLib
//
//  Created by Асиф Мамедов on 17/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

namespace pbrlib
{
    template<typename DirLightNodeModifier>
    inline DirLightNodeModifier& DirectionLightNode::getDirLightNodeModifier()
    {
        return *dynamic_cast<DirLightNodeModifier*>(_dir_light_node_modifiers.at(typeid(DirLightNodeModifier)).get());
    }

    template<typename DirLightNodeModifier>
    inline const DirLightNodeModifier& DirectionLightNode::getDirLightNodeModifier() const
    {
        return *dynamic_cast<DirLightNodeModifier*>(_dir_light_node_modifiers.at(typeid(DirLightNodeModifier)).get());
    }
}