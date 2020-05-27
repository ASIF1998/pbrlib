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
    inline TComponent& Scene::Node::getComponent()
    {
        return *dynamic_cast<TComponent*>(_components.at(typeid(TComponent)).get());
    }

    template<typename TComponent>
    inline const TComponent& Scene::Node::getComponent() const
    {
        return *dynamic_cast<TComponent*>(_components.at(typeid(TComponent)).get());
    }

    template<typename TScript>
    inline TScript& Scene::Node::getScript()
    {
        return *dynamic_cast<TScript*>(_scripts.at(typeid(TScript)).get());
    }

    template<typename TScript>
    inline const TScript& Scene::Node::getScript() const
    {
        return *dynamic_cast<TScript*>(_scripts.at(typeid(TScript)).get());
    }

    template<typename TComponent>
    bool Scene::Node::hasComponent() const
    {
        auto it = _components.find(typeid(TComponent));
        return it != end(_components);
    }

    template<typename TScript>
    bool Scene::Node::hasScript() const
    {
        auto it = _scripts.find(typeid(TScript));
        return it != end(_scripts);
    }
}

