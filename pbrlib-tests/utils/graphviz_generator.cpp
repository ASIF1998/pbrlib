#include <pbrlib/scene/scene.hpp>

#include "graphviz_generator.hpp"

#include <format>

namespace pbrlib::testing
{
    GraphvizGenerator::GraphvizGenerator()
    {
        _src << "digraph G {" << std::endl;
    }

    void GraphvizGenerator::process(SceneItem* ptr_item)
    {
        if (!_ptr_root_item)
            _ptr_root_item = ptr_item;

        const auto& tag = ptr_item->getComponent<TagComponent>();

        for (const auto& child: ptr_item->sceneItems())
        {
            const auto& child_tag = child.getComponent<TagComponent>();

            _src << std::format("\"{}\" -> \"{}\"\n", tag.name, child_tag.name);
        }

        if (_ptr_root_item == ptr_item)
            _src << "}";
    }
}