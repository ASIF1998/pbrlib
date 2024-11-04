namespace pbrlib
{
    template<typename Component, typename ...Args>
    void SceneItem::addComponent(Args... args)
    {
        _ptr_scene->_registry.emplace_or_replace<Component>(_handle, std::forward<Args>(args)...);
    }

    template<typename Component>
    Component& SceneItem::getComponent()
    {
        return _ptr_scene->_registry.get<Component>(_handle);
    }

    template<typename Component>
    const Component& SceneItem::getComponent() const
    {
        return _ptr_scene->_registry.get<Component>(_handle);
    }

    template<typename Component>
    bool SceneItem::hasComponent() const
    {
        return _ptr_scene->_registry.any_of<Component>(_handle);
    }
}