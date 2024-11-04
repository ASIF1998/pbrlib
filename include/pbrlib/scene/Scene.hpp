#pragma once

#include <pbrlib/moving/transform.hpp>

#include <entt/entt.hpp>

#include <memory>
#include <list>

#include <string>
#include <string_view>

#include <functional>

namespace pbrlib
{
    struct  InputStay;
    class   Scene;
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
            const InputStay*    ptr_input_stay, 
            float               delta_time, 
            const Transform&    world_transform
        );

    public:
        using UpdateCallback = std::function<void (
            SceneItem*          ptr_scene_item, 
            const InputStay*    ptr_input_stay, 
            float               delta_time, 
            const Transform&    world_transform
        )>;

    public:
        template<typename Component, typename ...Args>
        void addComponent(Args... args);

        template<typename Component>
        [[nodiscard]] Component& getComponent();

        template<typename Component>
        [[nodiscard]] const Component& getComponent() const;

        template<typename Component>
        [[nodiscard]] bool hasComponent() const;

        void setUpdateCallback(const UpdateCallback& callback);

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

        [[nodiscard]] std::string_view name() const;
        
        [[nodiscard]] SceneItem& addItem(std::string_view name);

        void update(const InputStay* ptr_input_stay, float delta_time);

    private:
        SceneItem       _root;
        entt::registry  _registry;
    };
}

#include "scene.inl"