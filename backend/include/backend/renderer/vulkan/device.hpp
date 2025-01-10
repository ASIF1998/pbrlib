#pragma once

#include <backend/renderer/vulkan/command_buffer.hpp>
#include <backend/renderer/vulkan/surface.hpp>
#include <backend/renderer/vulkan/descriptor_set.hpp>

#include <vma/vk_mem_alloc.h>

#include <string_view>
#include <numeric>
#include <vector>
#include <unordered_map>
#include <optional>

namespace pbrlib
{
    class Window;
}

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
        void getGeneralQueueIndex();

        void initInstance(bool is_debug);
        void getPhysicalDevice();
        void initDevice();
        void initGpuAllocator();
        void initCommandPools();

        void loadFunctions();

        void createDescriptorPool();

    public:
        Device() = default;

        Device(Device&& device)      = delete;
        Device(const Device& device) = delete;

        ~Device();

        Device& operator = (Device&& device)         = delete;
        Device& operator = (const Device& device)    = delete;

        void init(const Window* ptr_window);

        [[nodiscard]] VkInstance        instance()          const noexcept;
        [[nodiscard]] VkPhysicalDevice  physicalDevice()    const noexcept;
        [[nodiscard]] VkDevice          device()            const noexcept;

        [[nodiscard]] VkDescriptorPool descriptorPool() const noexcept;

        [[nodiscard]] const VkPhysicalDeviceProperties& gpuProperties() const noexcept;

        [[nodiscard]] const Queue& queue()  const noexcept;

        [[nodiscard]] VmaAllocator vmaAllocator() const noexcept;

        [[nodiscard]] CommandBuffer oneTimeSubmitCommandBuffer(const Queue& queue, std::string_view name = "");

        [[nodiscard]] DescriptorSet allocate(VkDescriptorSetLayout set_layout_handle);

        void setName(const VkDebugUtilsObjectNameInfoEXT& name_info) const;

    private:
        VkInstance          _instance_handle        = VK_NULL_HANDLE;
        VkPhysicalDevice    _physical_device_handle = VK_NULL_HANDLE;
        VkDevice            _device_handle          = VK_NULL_HANDLE;

        VkPhysicalDeviceProperties _gpu_properties = { };

        Queue           _general_queue;
        VkCommandPool   _command_pool_for_general_queue = VK_NULL_HANDLE;

        VmaAllocator _vma_allocator_handle = VK_NULL_HANDLE;

        VulkanFunctions _functions;

        std::optional<Surface> _surface;

        VkDescriptorPool _descriptor_pool_handle = VK_NULL_HANDLE;
    };
}