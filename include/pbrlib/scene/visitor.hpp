#pragma once

namespace pbrlib
{
    class SceneItem;
}

namespace pbrlib
{
    struct SceneVisitor
    {
        SceneVisitor() = default;

        SceneVisitor(SceneVisitor&& visitor)        = delete;
        SceneVisitor(const SceneVisitor& visitor)   = delete;

        virtual ~SceneVisitor() = default;

        SceneVisitor& operator = (SceneVisitor&& visitor)       = delete;
        SceneVisitor& operator = (const SceneVisitor& visitor)  = delete;

        virtual void process(SceneItem* ptr_item) = 0;
    };

    template<typename T>
    concept IsSceneVisitor = requires(T visitor)
    {
        dynamic_cast<SceneVisitor*>(&visitor);
    };
}