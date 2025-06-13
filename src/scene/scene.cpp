#include <pbrlib/scene/scene.hpp>
#include <pbrlib/input/input_stay.hpp>

#include <pbrlib/engine.hpp>

#include <backend/renderer/frame_graph/frame_graph.hpp>

#include <backend/scene/material_manager.hpp>
#include <backend/scene/assimp_importer.hpp>

#include <backend/logger/logger.hpp>

#include <backend/profiling.hpp>

#include <pbrlib/exceptions.hpp>

namespace pbrlib
{
    SceneItem::SceneItem(const std::string_view name, Scene* ptr_scene) : 
        _ptr_scene(ptr_scene)
    {
        if (!_ptr_scene)
            throw exception::InvalidArgument("ptr_scene");

        _handle = _ptr_scene->_registry.create();

        addComponent<component::Tag>(name.empty() ? "SceneItem" : name);
        addComponent<component::Transform>();
    }

    SceneItem::SceneItem(SceneItem&& item) :
        _ptr_scene          (item._ptr_scene),
        _update_callback    (item._update_callback),
        _children           (std::move(item._children))
    {
        std::swap(_handle, item._handle);
    }

    SceneItem::~SceneItem()
    {
        if (_handle != entt::null) 
        {
            _ptr_scene->_registry.destroy(_handle);
        }
    }

    SceneItem& SceneItem::operator = (SceneItem&& item)
    {
        _ptr_scene          = item._ptr_scene;
        _update_callback    = item._update_callback;
        _children           = std::move(item._children);

        std::swap(_handle, item._handle);
        return *this;
    }

    void SceneItem::updateCallback(const UpdateCallback& callback)
    {
        _update_callback = callback;
    }

    SceneItem& SceneItem::addItem(std::string_view name)
    {
        _children.push_back(SceneItem(name, _ptr_scene));
        return _children.back();
    }

    void SceneItem::update (
        const InputStay&    input_stay, 
        float               delta_time, 
        const math::mat4&   world_transform
    )
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        if (_update_callback)
            _update_callback(*this, input_stay, delta_time, world_transform);

        const auto& local_transform = getComponent<component::Transform>().transform;
        const auto  transform       = world_transform * local_transform;

        for (auto& child: _children)
            child.update(input_stay, delta_time, transform);
    }

    void SceneItem::visit(SceneVisitor* ptr_visitor)
    {
        ptr_visitor->process(this);
        for (auto& child: _children)
            child.visit(ptr_visitor);
    }
}

namespace pbrlib 
{
    Scene::Scene(std::string_view name)
    { 
        _root = SceneItem(name, this);
    }

    Scene::Scene(Scene&& scene)
    {
        std::swap(_root, scene._root);
        std::swap(_registry, scene._registry);
    }

    Scene& Scene::operator = (Scene&& scene)
    {
        std::swap(_root, scene._root);
        std::swap(_registry, scene._registry);

        return *this;
    }

    std::string_view Scene::name() const
    {
        return _root->getComponent<component::Tag>().name;
    }

    void Scene::update(const InputStay& input_stay, float delta_time)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        _root->update(input_stay, delta_time, math::mat4(1.0f));
    }

    SceneItem& Scene::addItem(std::string_view name)
    {
        return _root->addItem(name);
    }

    bool Scene::import(const std::filesystem::path& filename, Engine& engine)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        backend::log::info("[importer] load model: '{}'", filename.string());

        return backend::AssimpImporter()
            .device(engine._ptr_device.get())
            .scene(this)
            .filename(filename)
            .materialManager(engine._ptr_material_manager.get())
            .meshManager(engine._ptr_mesh_manager.get())
            .import();
    }    
}
