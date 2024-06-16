//
//  Scene.inl
//  PBRLib
//
//  Created by Асиф Мамедов on 15/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

namespace pbrlib
{
    template<typename TComponent>
    inline TComponent& SceneItem::getComponent()
    {
        return *dynamic_cast<TComponent*>(_components.at(typeid(TComponent)).get());
    }

    template<typename TComponent>
    inline const TComponent& SceneItem::getComponent() const
    {
        return *dynamic_cast<TComponent*>(_components.at(typeid(TComponent)).get());
    }

    template<typename TComponent>
    bool SceneItem::hasComponent() const
    {
        auto it = _components.find(typeid(TComponent));
        return it != end(_components);
    }
}

