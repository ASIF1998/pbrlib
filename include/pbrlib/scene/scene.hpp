#pragma once

#include <pbrlib/transform.hpp>

#include <pbrlib/scene/visitor.hpp>

#include <entt/entt.hpp>

#include <memory>
#include <list>

#include <string>
#include <string_view>

#include <functional>

#include <optional>

#include <filesystem>

namespace pbrlib
{
    struct  InputStay;
    class   Scene;
    class   Engine;
}

namespace pbrlib
{
    struct TagComponent
    {
        explicit TagComponent(std::string_view name);
    
        std::string name;
    };

    struct TransformComponent
    {
        Transform transform;
    };

    class SceneItem final
    {
        friend class Scene;

        using SceneItems = std::list<SceneItem>;

        explicit SceneItem(std::string_view name, Scene* ptr_scene);

        void update(
            const InputStay&    input_stay, 
            float               delta_time, 
            const Transform&    world_transform
        );

        void visit(SceneVisitor* ptr_visitor);

    public:
        using UpdateCallback = std::function<void (
            SceneItem*          ptr_item, 
            const InputStay&    input_stay, 
            float               delta_time, 
            const Transform&    world_transform
        )>;

    public:
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

        void updateCallback(const UpdateCallback& callback);

        [[nodiscard]] SceneItem& addItem(std::string_view name);

    protected:
        entt::entity        _handle     = entt::null;
        Scene*              _ptr_scene  = nullptr;
        UpdateCallback      _update_callback;
        
        SceneItems _children;  
    };

    class Scene final
    {
        friend class SceneItem;

    public:
        explicit Scene(std::string_view name);

        Scene(Scene&& scene);
        Scene(const Scene& scene) = delete;

        Scene& operator = (Scene&& scene);
        Scene& operator = (const Scene& scene) = delete;

        [[nodiscard]] std::string_view name() const;
        
        [[nodiscard]] SceneItem& addItem(std::string_view name);

        [[maybe_unused]] bool import(const std::filesystem::path& filename, Engine* ptr_engine);

        void update(const InputStay& input_stay, float delta_time);

        template<IsSceneVisitor T>
        void visit(const std::unique_ptr<T>& ptr_visitor)
        {
            if (ptr_visitor && _root)
                _root->visit(ptr_visitor.get());
        }

    private:
        entt::registry              _registry;
        std::optional<SceneItem>    _root;
    };
}

#include "scene.inl"