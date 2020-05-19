//
//  MeshNode.inl
//  PBRLib
//
//  Created by Асиф Мамедов on 19/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

namespace pbrlib
{
    template<typename MeshNodeModifier>
    inline MeshNodeModifier& MeshNode::getMeshNodeModifier()
    {
        return *dynamic_cast<MeshNodeModifier*>(_mesh_node_modifiers.at(typeid(MeshNodeModifier)).get());
    }

    template<typename MeshNodeModifier>
    inline const MeshNodeModifier& MeshNode::getMeshNodeModifier() const
    {
        return *dynamic_cast<MeshNodeModifier*>(_mesh_node_modifiers.at(typeid(MeshNodeModifier)).get());
    }

    template<typename MeshNodeModifier>
    inline bool MeshNode::hasMeshNodeModifier() const
    {
        auto it = _mesh_node_modifiers.find(typeid(MeshNodeModifier));
        return it != end(_mesh_node_modifiers);
    }
}
