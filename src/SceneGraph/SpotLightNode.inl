//
//  SpotLightNode.inl
//  PBRLib
//
//  Created by Асиф Мамедов on 17/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

namespace pbrlib
{
    template<typename SpotLightNodeModifierr>
    inline SpotLightNodeModifierr& SpotLightNode::getSpotLightNodeModifier()
    {
        return *dynamic_cast<SpotLightNodeModifierr*>(_spot_light_node_modifier.at(typeid(SpotLightNodeModifierr)).get());
    }

    template<typename SpotLightNodeModifierr>
    inline const SpotLightNodeModifierr& SpotLightNode::getSpotLightNodeModifier() const
    {
        return *dynamic_cast<SpotLightNodeModifierr*>(_spot_light_node_modifier.at(typeid(SpotLightNodeModifierr)).get());
    }

    template<typename SpotLightNodeModifierr>
    inline bool SpotLightNode::hasSpotLightNodeModifier() const
    {
        auto it = _spot_light_node_modifier.find(typeid(SpotLightNodeModifierr));
        return it != end(_spot_light_node_modifier);
    }
}
