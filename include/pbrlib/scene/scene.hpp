#pragma once

#include <pbrlib/components.hpp>
#include <pbrlib/scene/visitor.hpp>

#include <entt/entt.hpp>

#include <memory>
#include <list>
#include <map>

#include <string>
#include <string_view>

#include <functional>

#include <optional>

#include <filesystem>

namespace pbrlib
{
    struct  InputStay;
    class   Scene;
    class   SceneItem;
    class   Engine;
}

namespace pbrlib::backend
{
    class MeshManager;
}

namespace pbrlib
{
    class SceneItem final
    {
        friend class Scene;

        explicit SceneItem (
            std::string_view    name,
            SceneItem*          ptr_parent,
            Scene*              ptr_scene
        );

        void update (
            const InputStay&    input_stay,
            float               delta_time,
            const math::mat4&   world_transform
        );

        void visit(SceneVisitor* ptr_visitor);

    public:
        using UpdateCallback = std::function<void (
            SceneItem&          item,
            const InputStay&    input_stay,
            float               delta_time,
            const math::mat4&   world_transform
        )>;

        SceneItem(SceneItem&& item);
        SceneItem(const SceneItem& item) = delete;

        ~SceneItem();

        SceneItem& operator = (SceneItem&& item);
        SceneItem& operator = (const SceneItem& item) = delete;

        template<typename Component, typename ...Args>
        Component& addComponent(Args... args);

        template<typename Component>
        [[nodiscard]] Component& getComponent();

        template<typename Component>
        [[nodiscard]] const Component& getComponent() const;

        template<typename Component>
        [[nodiscard]] bool hasComponent() const;

        void update(const UpdateCallback& callback);

        [[nodiscard]] SceneItem& addItem(std::string_view name);

    protected:
        entt::entity        _handle     = entt::null;
        Scene*              _ptr_scene  = nullptr;
        UpdateCallback      _update_callback;

        SceneItem* _ptr_parent = nullptr;

        std::list<SceneItem> _children;
    };

    class Scene final
    {
        friend class SceneItem;
        friend class Engine;

        void meshManager(backend::MeshManager* ptr_mesh_manager) noexcept;

    public:
        explicit Scene(std::string_view name);

        Scene(Scene&& scene);
        Scene(const Scene& scene) = delete;

        Scene& operator = (Scene&& scene);
        Scene& operator = (const Scene& scene) = delete;

        [[nodiscard]] std::string_view name() const;

        [[nodiscard]] SceneItem& addItem(std::string_view name);

        void import (
            Engine&                         engine,
            const std::filesystem::path&    filename,
            const math::mat4&               transform = math::mat4(1.0f)
        );

        void update(const InputStay& input_stay, float delta_time);

        SceneItem*          item(std::string_view name);
        const SceneItem*    item(std::string_view name) const;

        SceneItem& createInstance (
            std::string_view    item_name,
            std::string_view    instance_name,
            const math::mat4&   transform
        );

        template<IsSceneVisitor T>
        void visit(const std::unique_ptr<T>& ptr_visitor)
        {
            if (ptr_visitor && _root) [[likely]]
                _root->visit(ptr_visitor.get());
        }

    private:
        entt::registry              _registry;
        std::optional<SceneItem>    _root;

        std::map<std::string, SceneItem*, std::less<void>> _items;

        backend::MeshManager* _ptr_mesh_manager = nullptr;
    };
}

#include "scene.inl"
