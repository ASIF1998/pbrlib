//
//  Component.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 27/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <pbrlib/scene/component.hpp>

namespace pbrlib
{
    ComponentBase::ComponentBase(const std::string_view name) :
        _name(name)
    {}

    ComponentBase::~ComponentBase()
    {}

    std::string& ComponentBase::getName() noexcept
    {
        return _name;
    }

    const std::string& ComponentBase::getName() const noexcept
    {
        return _name;
    }
}
