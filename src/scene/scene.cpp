#include <pbrlib/scene/scene.hpp>
#include <pbrlib/input/input_stay.hpp>

#include <pbrlib/engine.hpp>

#include <backend/renderer/frame_graph/frame_graph.hpp>

#include <backend/scene/mesh_manager.hpp>

#include <backend/logger/logger.hpp>

#include <backend/profiling.hpp>

#include <backend/components.hpp>

#include <pbrlib/exceptions.hpp>

#include <pbrlib/event_system.hpp>
#include <backend/events.hpp>

#include <stack>

namespace pbrlib
{
    SceneItem::SceneItem(const std::string_view name, SceneItem* ptr_parent, Scene* ptr_scene) :
        _ptr_scene  (ptr_scene),
        _ptr_parent (ptr_parent)
    {
        if (!_ptr_scene)
            throw exception::InvalidArgument("[scene-item] ctor: ptr_scene is null");

        _handle = _ptr_scene->_registry.create();

        addComponent<components::Tag>(name.empty() ? "SceneItem" : name);
        addComponent<components::Transform>();
    }

    SceneItem::SceneItem(SceneItem&& item) :
        _ptr_scene          (item._ptr_scene),
        _update_callback    (item._update_callback),
        _ptr_parent         (item._ptr_parent),
        _children           (std::move(item._children))
    {
        std::swap(_handle, item._handle);
    }

    SceneItem::~SceneItem()
    {
        if (_handle != entt::null) [[likely]]
            _ptr_scene->_registry.destroy(_handle);
    }

    SceneItem& SceneItem::operator = (SceneItem&& item)
    {
        _ptr_scene          = item._ptr_scene;
        _update_callback    = item._update_callback;
        _ptr_parent         = item._ptr_parent;
        _children           = std::move(item._children);

        std::swap(_handle, item._handle);
        return *this;
    }

    void SceneItem::update(const UpdateCallback& callback)
    {
        _update_callback = callback;
    }

    SceneItem& SceneItem::addItem(std::string_view name)
    {
        _children.push_back(SceneItem(name, this, _ptr_scene));
        _ptr_scene->_items.emplace(name, &_children.back());
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

        const auto& local_transform = getComponent<components::Transform>().transform;
        const auto  transform       = world_transform * local_transform;

        if (auto ptr_mesh_manager = _ptr_scene->_ptr_mesh_manager) [[likely]]
            ptr_mesh_manager->updateItemTransform(this, transform);

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
        _root = SceneItem(name, nullptr, this);
        _items.emplace(name, &_root.value());
    }

    Scene::Scene(Scene&& scene)
    {
        std::swap(_root, scene._root);
        std::swap(_registry, scene._registry);
        std::swap(_items, scene._items);

        _ptr_mesh_manager = scene._ptr_mesh_manager;
    }

    Scene& Scene::operator = (Scene&& scene)
    {
        std::swap(_root, scene._root);
        std::swap(_registry, scene._registry);
        std::swap(_items, scene._items);

        _ptr_mesh_manager = scene._ptr_mesh_manager;

        return *this;
    }

    std::string_view Scene::name() const
    {
        return _root->getComponent<components::Tag>().name;
    }

    void Scene::update(const InputStay& input_stay, float delta_time)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        if (!_ptr_mesh_manager) [[unlikely]]
            backend::log::warning("[scene] no mesh manager");

        _root->update(input_stay, delta_time, math::mat4(1.0f));
    }

    SceneItem& Scene::addItem(std::string_view name)
    {
        auto& item = _root->addItem(name);
        _items.emplace(name, &item);
        return item;
    }

    SceneItem* Scene::item(std::string_view name)
    {
        if (auto it = _items.find(name); it != std::end(_items)) [[likely]]
            return it->second;

        return nullptr;
    }

    const SceneItem* Scene::item(std::string_view name) const
    {
        if (auto it = _items.find(name); it != std::end(_items)) [[likely]]
            return it->second;

        return nullptr;
    }

    SceneItem& Scene::createInstance (
        std::string_view    item_name,
        std::string_view    instance_name,
        const math::mat4&   transform
    )
    {
        static std::stack<SceneItem*> items;

        auto ptr_item = item(item_name);

        if (!ptr_item) [[unlikely]]
            throw exception::InvalidArgument(std::format("[scene] item '{}' not exist", item_name));

        if (!ptr_item->_ptr_parent) [[unlikely]]
            throw exception::InvalidArgument("[scene] root node cannot be instantiated");

        auto ptr_parent = ptr_item->_ptr_parent;
        if (!items.empty())
            ptr_parent = items.top();

        auto& instance_node = ptr_parent->addItem(instance_name);

        auto&       dst_transform = instance_node.getComponent<components::Transform>();
        const auto& src_transform = ptr_item->getComponent<components::Transform>();

        dst_transform.transform = src_transform.transform * transform;

        if (ptr_item->hasComponent<backend::components::Renderable>())
        {
            instance_node.addComponent<backend::components::Renderable>();
            _ptr_mesh_manager->addInstance(ptr_item, &instance_node);
        }

        items.push(&instance_node);

        for (const auto& child: ptr_item->_children)
        {
            const auto& child_tag           = child.getComponent<components::Tag>();
            const auto  child_instance_tag  = std::format("[{}] - {}", instance_name, child_tag.name);

            createInstance(child_tag.name, child_instance_tag, math::mat4(1.0f));
        }

        items.pop();

        return instance_node;
    }

    void Scene::import (
        Engine&                         engine,
        const std::filesystem::path&    filename,
        const math::mat4&               transform
    )
    {
        EventSystem::emmit(backend::events::AssimpImporter {
            .ptr_device             = engine._ptr_device.get(),
            .ptr_scene              = this,
            .filename               = filename,
            .transform              = transform,
            .ptr_material_manager   = engine._ptr_material_manager.get(),
            .ptr_mesh_manager       = engine._ptr_mesh_manager.get()
        });
    }

    void Scene::meshManager(backend::MeshManager* ptr_mesh_manager) noexcept
    {
        _ptr_mesh_manager = ptr_mesh_manager;
    }
}
