#include <backend/renderer/vulkan/device.hpp>
#include <backend/renderer/vulkan/check.hpp>

#include <backend/profiling.hpp>

namespace pbrlib::backend::vk::priv
{
    class UnmapGuard final
    {
    public:
        explicit UnmapGuard(VmaAllocator allocator_handle, VmaAllocation allocation_handle) noexcept :
            _allocator_handle   (allocator_handle),
            _allocation_handle  (allocation_handle)
        { }

        UnmapGuard(UnmapGuard&& unmap_guard)        = delete;
        UnmapGuard(const UnmapGuard& unmap_guard)   = delete;

        ~UnmapGuard()
        {
            vmaUnmapMemory(_allocator_handle, _allocation_handle);
        }

        UnmapGuard& operator = (UnmapGuard&& unmap_guard)       = delete;
        UnmapGuard& operator = (const UnmapGuard& unmap_guard)  = delete;

    private:
        VmaAllocator    _allocator_handle   = VK_NULL_HANDLE;
        VmaAllocation   _allocation_handle  = VK_NULL_HANDLE;
    };

    template<typename Callback>
    requires std::invocable<Callback, std::span<uint8_t>>
    void map(VmaAllocator allocator_handle, VmaAllocation allocation_handle, VkDeviceSize buffer_size, Callback&& callback)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        uint8_t* ptr_data = nullptr;
        VK_CHECK(vmaMapMemory(
            allocator_handle,
            allocation_handle,
            reinterpret_cast<void**>(&ptr_data)
        ));

        const UnmapGuard unmap_guard (allocator_handle, allocation_handle);

        callback(std::span(ptr_data, static_cast<size_t>(buffer_size)));
    }
}

namespace pbrlib::backend::vk
{
    template<typename Callback>
    requires std::invocable<Callback, std::span<uint8_t>>
    inline void Buffer::map(Callback&& callback)
    {
        priv::map(_device.vmaAllocator(), handle.context<VmaAllocation>(), size, std::forward<Callback>(callback));
    }

    template<typename Callback>
    requires std::invocable<Callback, std::span<const uint8_t>>
    inline void Buffer::map(Callback&& callback) const
    {
        priv::map(_device.vmaAllocator(), handle.context<VmaAllocation>(), size, std::forward<Callback>(callback));
    }
}
