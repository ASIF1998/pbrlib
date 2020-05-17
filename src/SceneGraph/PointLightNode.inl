//
//  PointLightNode.inl
//  PBRLib
//
//  Created by Асиф Мамедов on 17/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

namespace pbrlib
{
    template<typename PointLightNodeModifier>
    inline PointLightNodeModifier& PointLightNode::getPointLightNodeModifier()
    {
        return *dynamic_cast<PointLightNodeModifier*>(_point_light_node_modifier.at(typeid(PointLightNodeModifier)).get());
    }

    template<typename PointLightNodeModifier>
    inline const PointLightNodeModifier& PointLightNode::getPointLightNodeModifier() const
    {
        return *dynamic_cast<PointLightNodeModifier*>(_point_light_node_modifier.at(typeid(PointLightNodeModifier)).get());
    }

    template<typename PointLightNodeModifier>
    inline bool PointLightNode::hasPointLightNodeModifier() const
    {
        auto it = _point_light_node_modifier.find(typeid(PointLightNodeModifier));
        return it != end(_point_light_node_modifier);
    }
}