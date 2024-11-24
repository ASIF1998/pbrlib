#pragma once

#include <backend/renderer/vulkan/command_buffer.hpp>

#include <vma/vk_mem_alloc.h>

#include <string_view>
#include <numeric>
#include <vector>
#include <unordered_map>

namespace pbrlib::vk
{
    struct Queue final
    {
        VkQueue     handle          = VK_NULL_HANDLE;
        uint32_t    index           = std::numeric_limits<uint32_t>::max();
        uint32_t    family_index    = std::numeric_limits<uint32_t>::max();
    };

    struct VulkanFunctions final
    {
        PFN_vkSetDebugUtilsObjectNameEXT vkSetDebugUtilsObjectNameEXT = VK_NULL_HANDLE;
    };

    class Device final
    {
        std::vector<uint32_t> initQueuesIndices();

        void initInstance(bool is_debug);
        void getPhysicalDevice();
        void initDevice();
        void initGpuAllocator();
        void initCommandPools();

        void loadFunctions();

    public:
        Device() = default;

        Device(Device&& device)      = delete;
        Device(const Device& device) = delete;

        ~Device();

        Device& operator = (Device&& device)         = delete;
        Device& operator = (const Device& device)    = delete;

        void init();

        [[nodiscard]] VkInstance        instance()          const noexcept;
        [[nodiscard]] VkPhysicalDevice  physicalDevice()    const noexcept;
        [[nodiscard]] VkDevice          device()            const noexcept;

        [[nodiscard]] const VkPhysicalDeviceProperties& gpuProperties() const noexcept;

        [[nodiscard]] const Queue& graphicsQueue()  const noexcept; 
        [[nodiscard]] const Queue& computeQueue()   const noexcept; 
        [[nodiscard]] const Queue& transferQueue()  const noexcept;

        [[nodiscard]] VmaAllocator vmaAllocator() const noexcept;

        [[nodiscard]] CommandBuffer oneTimeSubmitCommandBuffer(const Queue& queue, std::string_view name = "");

        void setName(const VkDebugUtilsObjectNameInfoEXT& name_info) const;

    private:
        VkInstance          _instance_handle        = VK_NULL_HANDLE;
        VkPhysicalDevice    _physical_device_handle = VK_NULL_HANDLE;
        VkDevice            _device_handle          = VK_NULL_HANDLE;

        VkPhysicalDeviceProperties _gpu_properties = { };

        Queue _graphics_queue;
        Queue _compute_queue;
        Queue _transfer_queue;

        VmaAllocator _vma_allocator_handle = VK_NULL_HANDLE;

        std::unordered_map<uint32_t, VkCommandPool> _command_pools_handles;

        VulkanFunctions _functions;
    };
}