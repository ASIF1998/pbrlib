//
//  Component.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 27/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef Component_hpp
#define Component_hpp

#include <string>
#include <string_view>

#include <typeindex>

using namespace std;

namespace pbrlib
{
    class SceneItem;
}

namespace pbrlib
{
    class ComponentBase
    {
    public:
        ComponentBase(const string_view name = "Component");
        virtual ~ComponentBase();

        string&         getName() noexcept;
        const string&   getName() const noexcept;

        virtual type_index getType() const = 0;

        virtual void update(SceneItem* ptr_node, float delta_time) 
        { }

    protected:
        string _name;
    };

    template<typename T>
    class Component :
        public ComponentBase
    {
        type_index getType() const override final
        {
            return typeid(T);
        }

    public:
        Component(const string_view name = "Component") :
            ComponentBase(name)
        { }
    };
}

#endif /* Component_hpp */
