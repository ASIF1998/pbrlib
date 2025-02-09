#pragma once

namespace pbrlib
{
    class SceneItem;
}

namespace pbrlib
{
    struct RenderableComponent
    {
        const SceneItem* ptr_item = nullptr;
    };
}