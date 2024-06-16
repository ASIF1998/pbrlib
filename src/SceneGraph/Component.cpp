//
//  Component.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 27/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <pbrlib/SceneGraph/Component.hpp>

namespace pbrlib
{
    ComponentBase::ComponentBase(const string_view name) :
        _name(name)
    {}

    ComponentBase::~ComponentBase()
    {}

    string& ComponentBase::getName() noexcept
    {
        return _name;
    }

    const string& ComponentBase::getName() const noexcept
    {
        return _name;
    }
}
