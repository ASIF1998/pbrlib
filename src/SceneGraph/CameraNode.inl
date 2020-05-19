//
//  CameraNode.inl
//  PBRLib
//
//  Created by Асиф Мамедов on 18/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

namespace pbrlib
{
    template<typename CameraNodeModifier>
    inline CameraNodeModifier& CameraNode::getCameraNodeModifier()
    {
        return *dynamic_cast<CameraNodeModifier*>(_camera_node_modifiers.at(typeid(CameraNodeModifier)).get());
    }

    template<typename CameraNodeModifier>
    inline const CameraNodeModifier& CameraNode::getCameraNodeModifier() const
    {
        return *dynamic_cast<CameraNodeModifier*>(_camera_node_modifiers.at(typeid(CameraNodeModifier)).get());
    }

    template<typename CameraNodeModifier>
    inline bool CameraNode::hasCameraNodeModifier() const
    {
        auto it = _camera_node_modifiers.find(typeid(CameraNodeModifier));
        return it != end(_camera_node_modifiers);
    }
}
