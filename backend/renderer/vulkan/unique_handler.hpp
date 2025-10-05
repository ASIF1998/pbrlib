#pragma once

#include <vulkan/vulkan.h>

#include <functional>
#include <tuple>

namespace pbrlib::backend::vk
{
    class HandleDispatcher final
    {
    public:
        HandleDispatcher(HandleDispatcher&& dispatcher)         = delete;
        HandleDispatcher(const HandleDispatcher& dispatcher)    = delete;

        HandleDispatcher& operator = (HandleDispatcher&& dispatcher)        = delete;
        HandleDispatcher& operator = (const HandleDispatcher& dispatcher)   = delete;

        static void destroy(VkDevice device_handle)                                                     noexcept;
        static void destroy(VkShaderModule shader_module_handle)                                        noexcept;
        static void destroy(VkCommandBuffer command_buffer_handle, VkCommandPool command_pool_handle)   noexcept;

        static void init(VkDevice device_handle);

    private:
        static VkDevice _device_handle;
    };
}

namespace pbrlib::backend::vk
{
    template<typename Handle, typename... FinalizationContext>
    concept VulkanHandle = requires(Handle handle, FinalizationContext... context)
    {
        HandleDispatcher::destroy(handle, context...);
    };

    template<typename Handle, typename... FinalizationContext>
    requires VulkanHandle<Handle, FinalizationContext...>
    class UniqueHandle final
    {
    public:
        UniqueHandle() = default;
        explicit UniqueHandle(Handle handle, FinalizationContext... context) noexcept;

        UniqueHandle(UniqueHandle&& handler) noexcept;
        UniqueHandle(const UniqueHandle& handler) = delete;

        ~UniqueHandle();

        UniqueHandle& operator = (UniqueHandle&& handler) noexcept;
        UniqueHandle& operator = (const UniqueHandle& handler) = delete;

        explicit operator   bool()      const noexcept;
        operator            Handle()    const noexcept;

        Handle get() const noexcept;

        void swap(UniqueHandle& rsh) noexcept;

    private:
        Handle                              handle_     = VK_NULL_HANDLE;
        std::tuple<FinalizationContext...>  context_    = std::make_tuple<FinalizationContext...>(VK_NULL_HANDLE...);
    };
}

namespace pbrlib::backend::vk
{
    using DeviceHandle          = UniqueHandle<VkDevice>;
    using ShaderModuleHandle    = UniqueHandle<VkShaderModule>;
    using CommandBufferHandle   = UniqueHandle<VkCommandBuffer, VkCommandPool>;
}

#include <backend/renderer/vulkan/unique_handler.inl>