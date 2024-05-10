//
//  Script.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 27/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef Script_hpp
#define Script_hpp

#include "Component.hpp"
#include "Scene.hpp"

namespace pbrlib
{
    class Script :
        public Component
    {
    public:
        Script(const string_view name = "Script");
        virtual ~Script();

        virtual void update(Scene::Node* ptr_node, float delta_time) = 0;
    };
}

#endif /* Script_hpp */
