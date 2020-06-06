//
//  IRenderer.h
//  PBRLib
//
//  Created by Асиф Мамедов on 02/06/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef IRenderer_h
#define IRenderer_h

#include "../../SceneGraph/Scene.hpp"

namespace pbrlib
{
    class IRenderer;

    using PtrIRenderer = shared_ptr<IRenderer>;

    class IRenderer
    {
    public:
        virtual void draw(const Scene::PtrNode& ptr_node, float delta_time) = 0;
    };
}

#endif /* IRenderer_h */
