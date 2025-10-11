#include <algorithm>

#include <pbrlib/exceptions.hpp>

namespace pbrlib::backend::vk
{
    template<typename Handle, typename... FinalizationContext>
    requires VulkanHandle<Handle, FinalizationContext...>
    UniqueHandle<Handle, FinalizationContext...>::UniqueHandle(Handle handle, FinalizationContext... context) noexcept :
        handle_     (handle),
        context_    (std::make_tuple(std::forward<FinalizationContext>(context)...))
    { }

    template<typename Handle, typename... FinalizationContext>
    requires VulkanHandle<Handle, FinalizationContext...>
    UniqueHandle<Handle, FinalizationContext...>::UniqueHandle(UniqueHandle&& handler) noexcept
    {
        swap(handler);
    }

    template<typename Handle, typename... FinalizationContext>
    requires VulkanHandle<Handle, FinalizationContext...>
    UniqueHandle<Handle, FinalizationContext...>::~UniqueHandle()
    {
        std::apply([this] (const auto... handles) 
        {
            HandleDispatcher::destroy(handle_, handles...);
        }, context_);
    }

    template<typename Handle, typename... FinalizationContext>
    requires VulkanHandle<Handle, FinalizationContext...>
    UniqueHandle<Handle, FinalizationContext...>& UniqueHandle<Handle, FinalizationContext...>::operator = (UniqueHandle&& handler) noexcept
    {
        swap(handler);
        return *this;
    }

    template<typename Handle, typename... FinalizationContext>
    requires VulkanHandle<Handle, FinalizationContext...>
    UniqueHandle<Handle, FinalizationContext...>::operator bool() const noexcept
    {
        return handle_ != VK_NULL_HANDLE;
    }
    
    template<typename Handle, typename... FinalizationContext>
    requires VulkanHandle<Handle, FinalizationContext...>
    UniqueHandle<Handle, FinalizationContext...>::operator Handle() const noexcept
    {
        return handle_;
    }

    template<typename Handle, typename... FinalizationContext>
    requires VulkanHandle<Handle, FinalizationContext...>
    Handle& UniqueHandle<Handle, FinalizationContext...>::get() noexcept
    {
        return handle_;
    }

    template<typename Handle, typename... FinalizationContext>
    requires VulkanHandle<Handle, FinalizationContext...>
    const Handle& UniqueHandle<Handle, FinalizationContext...>::get() const noexcept
    {
        return handle_;
    }

    template<typename Handle, typename... FinalizationContext>
    requires VulkanHandle<Handle, FinalizationContext...>
    void UniqueHandle<Handle, FinalizationContext...>::swap(UniqueHandle<Handle, FinalizationContext...>& rsh) noexcept
    {
        std::swap(handle_, rsh.handle_);
        std::swap(context_, rsh.context_);
    }
}