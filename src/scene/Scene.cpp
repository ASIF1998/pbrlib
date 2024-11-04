#include <pbrlib/scene/scene.hpp>
#include <pbrlib/input/input_stay.hpp>

namespace pbrlib
{
    TagComponent::TagComponent(std::string_view name) :
        name(name)
    { } 
}

namespace pbrlib
{
    SceneItem::SceneItem(const std::string_view name, Scene* ptr_scene) : 
        _ptr_scene(ptr_scene)
    {
        if (!_ptr_scene)
            throw std::invalid_argument("ptr_scene");

        _handle = _ptr_scene->_registry.create();

        addComponent<TagComponent>(name.empty() ? "SceneItem" : name);
        addComponent<TransformComponent>();
    }

    void SceneItem::setUpdateCallback(const UpdateCallback& callback)
    {
        _update_callback = callback;
    }

    SceneItem& SceneItem::addItem(std::string_view name)
    {
        _children.push_back(SceneItem(name, _ptr_scene));
        return _children.back();
    }

    void SceneItem::update(const InputStay* ptr_input_stay, float delta_time, const Transform& world_transform)
    {
        if (_update_callback)
            _update_callback(this, ptr_input_stay, delta_time, world_transform);

        const auto& local_transform = getComponent<TransformComponent>().transform;
        const auto  transform       = world_transform * local_transform;

        for (auto& child: _children)
            child.update(ptr_input_stay, delta_time, transform);
    }
}

namespace pbrlib 
{
    Scene::Scene(std::string_view name) :
        _root (name, this)
    { }

    std::string_view Scene::name() const
    {
        return _root.getComponent<TagComponent>().name;
    }

    void Scene::update(const InputStay* ptr_input_stay, float delta_time)
    {
        _root.update(ptr_input_stay, delta_time, Transform());
    }

    SceneItem& Scene::addItem(std::string_view name)
    {
        return _root.addItem(name);
    }
}
