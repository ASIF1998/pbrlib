//
//  Script.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 27/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include "Script.hpp"

namespace pbrlib
{
    Script::Script(const string_view name) :
        Component(name)
    {}

    Script::~Script()
    {}
}