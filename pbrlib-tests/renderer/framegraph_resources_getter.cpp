#include "framegraph_resources_getter.hpp"

#include <backend/renderer/frame_graph/frame_graph.hpp>

#include <pbrlib/engine.hpp>

namespace pbrlib::testing
{
    FrameGraphResourcesGetter::FrameGraphResourcesGetter(Engine& engine) :
        _ptr_frame_graph(engine._ptr_frame_graph.get())
    {}

    backend::vk::Device& FrameGraphResourcesGetter::device()
    {
        return _ptr_frame_graph->_device;
    }

    backend::vk::Image* FrameGraphResourcesGetter::image(std::string_view name)
    {
        return &_ptr_frame_graph->_images.at(name.data());
    }

    const backend::MaterialManager* FrameGraphResourcesGetter::materialManager() const
    {
        return _ptr_frame_graph->_render_context.ptr_material_manager;
    }

    const backend::MeshManager* FrameGraphResourcesGetter::meshManager() const
    {
        return _ptr_frame_graph->_render_context.ptr_mesh_manager;
    }
}
