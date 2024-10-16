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

namespace pbrlib
{
    class   SceneItem;
    struct  InputStay;
}

namespace pbrlib
{
    class ComponentBase
    {
    public:
        ComponentBase(const std::string_view name = "Component");
        virtual ~ComponentBase();

        std::string&        getName() noexcept;
        const std::string&  getName() const noexcept;

        virtual std::type_index getType() const = 0;

        virtual void update(const InputStay* ptr_input_stay, SceneItem* ptr_node, float delta_time) 
        { }

    protected:
        std::string _name;
    };

    template<typename T>
    class Component :
        public ComponentBase
    {
        std::type_index getType() const override final
        {
            return typeid(T);
        }

    public:
        Component(const std::string_view name = "Component") :
            ComponentBase(name)
        { }
    };
}

#endif /* Component_hpp */
