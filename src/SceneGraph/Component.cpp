//
//  Component.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 27/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include "Component.hpp"

namespace pbrlib
{
    Component::Component(const string_view name) :
        _name(name)
    {}

    Component::~Component()
    {}

    string& Component::getName() noexcept
    {
        return _name;
    }

    const string& Component::getName() const noexcept
    {
        return _name;
    }
}
