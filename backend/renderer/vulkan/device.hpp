#pragma once

#include <backend/renderer/vulkan/command_buffer.hpp>
#include <backend/renderer/vulkan/surface.hpp>

#include <backend/profiling.hpp>

#include <vma/vk_mem_alloc.h>

#include <numeric>

#include <string_view>

#include <vector>
#include <unordered_map>

#include <optional>

namespace pbrlib::backend
{
    class Window;
}

namespace pbrlib::backend::vk
{
    class Buffer;
}

namespace pbrlib::backend::vk
{
    struct Queue final
    {
        VkQueue     handle          = VK_NULL_HANDLE;
        uint32_t    index           = std::numeric_limits<uint32_t>::max();
        uint32_t    family_index    = std::numeric_limits<uint32_t>::max();
    };

    struct Functions final
    {
        PFN_vkSetDebugUtilsObjectNameEXT vkSetDebugUtilsObjectNameEXT = VK_NULL_HANDLE;

        PFN_vkCmdDebugMarkerBeginEXT    vkCmdDebugMarkerBeginEXT    = VK_NULL_HANDLE;
        PFN_vkCmdDebugMarkerEndEXT      vkCmdDebugMarkerEndEXT      = VK_NULL_HANDLE;
    };

    struct DescriptorImageInfo final
    {
        VkImageView     view_handle             = VK_NULL_HANDLE;
        VkSampler       sampler_handle          = VK_NULL_HANDLE;
        VkDescriptorSet set_handle              = VK_NULL_HANDLE;
        VkImageLayout   expected_image_layout   = VK_IMAGE_LAYOUT_UNDEFINED;
        uint32_t        binding                 = 0;
        uint32_t        array_element           = 0;
    };

    struct DescriptorBufferInfo final
    {
        const vk::Buffer&   buffer;
        VkDescriptorSet     set_handle      = VK_NULL_HANDLE;
        uint32_t            offset          = 0;
        uint32_t            size            = 0;
        uint32_t            binding         = 0;
        uint32_t            array_element   = 0;
    };

    class Device final
    {
        void getGeneralQueueIndex();

        void createInstance(bool is_debug);
        void getPhysicalDevice();
        void createDevice();
        void createGpuAllocator();
        void createCommandPools();

        void loadFunctions();

        void createDescriptorPool();

        bool isRunFromFrameDebugger() const;

        void createTracyContext();

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

        [[nodiscard]] VkDescriptorPool descriptorPool() const noexcept;

        [[nodiscard]] const VkPhysicalDeviceProperties2& gpuProperties() const noexcept;

        [[nodiscard]] const Queue& queue()  const noexcept;

        [[nodiscard]] VmaAllocator vmaAllocator() const noexcept;

        [[nodiscard]] CommandBuffer oneTimeSubmitCommandBuffer(std::string_view name = "");

        [[nodiscard]] VkDescriptorSet allocateDescriptorSet(VkDescriptorSetLayout desc_set_layout_handle, std::string_view name = "") const;

        [[nodiscard]] const Functions& vulkanFunctions() const noexcept;

        void setName(const VkDebugUtilsObjectNameInfoEXT& name_info) const;

        void submit(const CommandBuffer& command_buffer);

        void writeDescriptorSet(const DescriptorImageInfo& descriptor_image_info)   const;
        void writeDescriptorSet(const DescriptorBufferInfo& descriptor_buffer_info) const;

#ifdef PBRLIB_ENABLE_PROPFILING
        [[nodiscard]] auto tracyContext() const noexcept
        {
            return _tracy_ctx;
        }
#endif

    private:
        VkInstance          _instance_handle        = VK_NULL_HANDLE;
        VkPhysicalDevice    _physical_device_handle = VK_NULL_HANDLE;
        VkDevice            _device_handle          = VK_NULL_HANDLE;

        VkPhysicalDeviceProperties2 _gpu_properties = { };

        Queue           _general_queue;
        VkCommandPool   _command_pool_for_general_queue = VK_NULL_HANDLE;
        VkFence         _submit_fence_handle            = VK_NULL_HANDLE;

        VmaAllocator _vma_allocator_handle = VK_NULL_HANDLE;

        Functions _functions;

        VkDescriptorPool _descriptor_pool_handle = VK_NULL_HANDLE;

#ifdef PBRLIB_ENABLE_PROPFILING
        TracyVkCtx _tracy_ctx = nullptr;
#endif
    };
}