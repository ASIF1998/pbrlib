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
    class Component
    {
    public:
        Component(const string_view name = "Component");
        virtual ~Component();

        string&         getName() noexcept;
        const string&   getName() const noexcept;

        virtual type_index getType() const = 0;

    protected:
        string _name;
    };

    namespace ComponentUtil
    {
        template<typename T>
        inline type_index getTypeIndex()
        {
            return typeid(T);
        }
    }
}

#endif /* Component_hpp */
