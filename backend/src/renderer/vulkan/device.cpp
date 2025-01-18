#include <pbrlib/rendering/window.hpp>

#include <backend/utils/versions.hpp>
#include <backend/logger/logger.hpp>

#include <backend/utils/vulkan.hpp>
#include <backend/renderer/vulkan/device.hpp>

#include <SDL3/SDL_vulkan.h>

#define VMA_IMPLEMENTATION
#include <vma/vk_mem_alloc.h>

#include <stdexcept>
#include <array>
#include <format>

namespace pbrlib::vk::settings
{   
    constexpr bool vulkan_set_obj_name = false;
}

namespace pbrlib::vk
{
    Device::~Device()
    {
        if (_descriptor_pool_handle != VK_NULL_HANDLE)
            vkDestroyDescriptorPool(_device_handle, _descriptor_pool_handle, nullptr);

        _surface = std::nullopt;

        vkDestroyCommandPool(_device_handle, _command_pool_for_general_queue, nullptr);

        if (_vma_allocator_handle != VK_NULL_HANDLE)
            vmaDestroyAllocator(_vma_allocator_handle);

        if (_device_handle != VK_NULL_HANDLE)
            vkDestroyDevice(_device_handle, nullptr);

        if (_instance_handle != VK_NULL_HANDLE) 
           vkDestroyInstance(_instance_handle, nullptr);
    }

    void Device::init(const Window* ptr_window)
    {
        initInstance(false);
        getPhysicalDevice();
        initDevice();

        if (ptr_window)
            _surface = std::make_optional<Surface>(this, ptr_window);

        loadFunctions();

        initGpuAllocator();

        initCommandPools();

        createDescriptorPool();
    }
}

namespace pbrlib::vk
{
    void Device::initInstance(bool is_debug)
    {
        VkApplicationInfo app_info = { };
        app_info.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        app_info.apiVersion         = utils::vulkanVersion();
        app_info.applicationVersion = 0;
        app_info.engineVersion      = utils::engineVersion();
        app_info.pEngineName        = "pbrlib";

        VkInstanceCreateInfo instance_info = { };
        instance_info.sType             = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instance_info.pApplicationInfo  = &app_info;

        if (is_debug) 
        {
            constexpr std::array layers {
                "VK_LAYER_LUNARG_api_dump",
                "VK_LAYER_KHRONOS_validation"
            };

            instance_info.enabledLayerCount     = static_cast<uint32_t>(layers.size());
            instance_info.ppEnabledLayerNames   = layers.data();
        }

        constexpr std::array extensions {
            VK_KHR_SURFACE_EXTENSION_NAME,
            "VK_KHR_win32_surface",
            VK_EXT_DEBUG_UTILS_EXTENSION_NAME
        };

        instance_info.enabledExtensionCount     = static_cast<uint32_t>(extensions.size());
        instance_info.ppEnabledExtensionNames   = extensions.data();

        VK_CHECK(vkCreateInstance(&instance_info, nullptr, &_instance_handle));
    }

    VkInstance Device::instance() const noexcept
    {
        return _instance_handle;
    }
}

namespace pbrlib::vk
{
    void Device::getPhysicalDevice()
    {
        uint32_t count = 0;

        VK_CHECK(vkEnumeratePhysicalDevices(
            _instance_handle,
            &count, nullptr
        ));

        std::vector<VkPhysicalDevice> handles (count);

        VK_CHECK(vkEnumeratePhysicalDevices(
            _instance_handle,
            &count, handles.data()
        ));

        _physical_device_handle = handles[0];
        vkGetPhysicalDeviceProperties(handles[0], &_gpu_properties);

        for (auto handle: handles) 
        {
            VkPhysicalDeviceProperties props = { };
            vkGetPhysicalDeviceProperties(handle, &props);

            if (props.apiVersion >= utils::vulkanVersion())
            {
                _physical_device_handle = handle;
                _gpu_properties         = props;
                break;
            }
        }

        if (_gpu_properties.apiVersion < utils::vulkanVersion())
        {
            constexpr auto major = VK_VERSION_MAJOR(utils::vulkanVersion());
            constexpr auto minor = VK_VERSION_MINOR(utils::vulkanVersion());
            constexpr auto patch = VK_VERSION_PATCH(utils::vulkanVersion());

            throw std::runtime_error(std::format("[Vulkan] Vulkan API version less {}.{}.{}", major, minor, patch));
        }

        switch(_gpu_properties.deviceType)
        {
            case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
                pbrlib::log::engine::info("Device type: Discrete GPU");
                break;
            case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
                pbrlib::log::engine::info("Device type: Integrated GPU");
                break;
            case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
                pbrlib::log::engine::info("Device type: Virtual GPU");
                break;
            case VK_PHYSICAL_DEVICE_TYPE_CPU:
                pbrlib::log::engine::info("Device type: CPU");
                break;
            default:
                throw std::runtime_error("[Vulkan] Couldn't find gpu");
        }

        pbrlib::log::engine::info("Device name: {}", _gpu_properties.deviceName);
    }

    VkPhysicalDevice Device::physicalDevice() const noexcept
    {
        return _physical_device_handle;
    }
}

namespace pbrlib::vk
{
    void Device::getGeneralQueueIndex()
    {
        uint32_t family_count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(_physical_device_handle, &family_count, nullptr);

        std::vector<VkQueueFamilyProperties> families (family_count);
        vkGetPhysicalDeviceQueueFamilyProperties(_physical_device_handle, &family_count, families.data());

        std::vector<uint32_t> num_queues_in_family (families.size(), 0);

        constexpr auto queue_flags = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT;

        for (uint32_t index = 0; index < static_cast<uint32_t>(families.size()); ++index)
        {
            const auto supportGraphics  = (families[index].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0;
            const auto supportCompute   = (families[index].queueFlags & VK_QUEUE_COMPUTE_BIT) != 0;
            const auto supportTransfer  = (families[index].queueFlags & VK_QUEUE_TRANSFER_BIT) != 0;

            if (supportGraphics && supportCompute && supportTransfer)
            {
                _general_queue.family_index = index;
                _general_queue.index        = num_queues_in_family[index]++;

                return ;
            }
        }

        throw std::runtime_error("[Vulkan] Couldn't find queue index");
    }

    void Device::initDevice()
    {
        getGeneralQueueIndex();

        constexpr float priority = 1.0f;

        VkDeviceQueueCreateInfo queue_info = { };
        queue_info.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_info.pQueuePriorities = &priority;
        queue_info.queueCount       = 1;
        queue_info.queueFamilyIndex = _general_queue.family_index;

        std::array extensions {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        VkDeviceCreateInfo device_info = { };
        device_info.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        device_info.pQueueCreateInfos       = &queue_info;
        device_info.queueCreateInfoCount    = 1;
        device_info.enabledExtensionCount   = static_cast<uint32_t>(extensions.size());
        device_info.ppEnabledExtensionNames = extensions.data();

        VK_CHECK(vkCreateDevice(_physical_device_handle, &device_info, nullptr, &_device_handle));

        vkGetDeviceQueue(_device_handle, _general_queue.family_index, _general_queue.index, &_general_queue.handle);
    }

    VkDevice Device::device() const noexcept
    {
        return _device_handle;
    }

    const Queue& Device::queue() const noexcept
    {
        return _general_queue;
    }

    const VkPhysicalDeviceProperties& Device::gpuProperties() const noexcept
    {
        return _gpu_properties;
    }
}

namespace pbrlib::vk
{
    void Device::initGpuAllocator()
    {
        VmaAllocatorCreateInfo allocator_info = { };
        allocator_info.instance         = _instance_handle;
        allocator_info.physicalDevice   = _physical_device_handle;
        allocator_info.device           = _device_handle;
        allocator_info.vulkanApiVersion = pbrlib::utils::vulkanVersion();

        VK_CHECK(vmaCreateAllocator(&allocator_info, &_vma_allocator_handle));
    }
}

namespace pbrlib::vk
{
    VmaAllocator Device::vmaAllocator() const noexcept
    {
        return _vma_allocator_handle;
    }
}

namespace pbrlib::vk
{
    void Device::initCommandPools()
    {
        VkCommandPoolCreateInfo command_pool_info = { };
        command_pool_info.sType             = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        command_pool_info.queueFamilyIndex  = _general_queue.family_index;

        VK_CHECK(vkCreateCommandPool(_device_handle, &command_pool_info, nullptr, &_command_pool_for_general_queue));
    }

    CommandBuffer Device::oneTimeSubmitCommandBuffer(const Queue& queue, std::string_view name)
    {
        VkCommandBufferAllocateInfo alloc_info = { };
        alloc_info.sType                = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        alloc_info.commandBufferCount   = 1;
        alloc_info.commandPool          = _command_pool_for_general_queue;
        alloc_info.level                = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

        CommandBuffer command_buffer;

        VK_CHECK(vkAllocateCommandBuffers(_device_handle, &alloc_info, &command_buffer.handle));

        command_buffer.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

        if (!name.empty())
        {
            VkDebugUtilsObjectNameInfoEXT name_info = { };
            name_info.sType         = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
            name_info.objectHandle  = reinterpret_cast<uint64_t>(command_buffer.handle);
            name_info.objectType    = VK_OBJECT_TYPE_COMMAND_BUFFER;
            name_info.pObjectName   = name.data();

            setName(name_info);
        }

        return command_buffer;
    }
}

namespace pbrlib::vk
{
    template<typename VulkanFunctionType>
    VulkanFunctionType loadFunction(VkDevice device_handle, const std::string_view function_name)
    {
        auto ptr_function = reinterpret_cast<VulkanFunctionType>(
            vkGetDeviceProcAddr(
                device_handle, 
                function_name.data()
            )
        );

        if (ptr_function)
            log::engine::info("[Load vulkan function]: {}", function_name);
        else 
            log::engine::error("[Load vulkan function]: {}", function_name);

        return ptr_function;
    }

    void Device::loadFunctions()
    {
        if (settings::vulkan_set_obj_name)
            _functions.vkSetDebugUtilsObjectNameEXT = loadFunction<PFN_vkSetDebugUtilsObjectNameEXT>(_device_handle, "vkSetDebugUtilsObjectNameEXT");
    }
}

namespace pbrlib::vk
{
    void Device::setName(const VkDebugUtilsObjectNameInfoEXT& name_info) const
    {
        if (_functions.vkSetDebugUtilsObjectNameEXT)
            _functions.vkSetDebugUtilsObjectNameEXT(_device_handle, &name_info);
    }
}

namespace pbrlib::vk
{
    void Device::createDescriptorPool()
    {
        constexpr std::array pool_sizes 
        {
            VkDescriptorPoolSize {.type = VK_DESCRIPTOR_TYPE_SAMPLER, .descriptorCount = 1000},
            VkDescriptorPoolSize {.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, .descriptorCount = 1000},
            VkDescriptorPoolSize {.type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, .descriptorCount = 1000},
            VkDescriptorPoolSize {.type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, .descriptorCount = 1000},
            VkDescriptorPoolSize {.type = VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, .descriptorCount = 1000},
            VkDescriptorPoolSize {.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, .descriptorCount = 1000},
            VkDescriptorPoolSize {.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, .descriptorCount = 1000},
            VkDescriptorPoolSize {.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, .descriptorCount = 1000},
            VkDescriptorPoolSize {.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, .descriptorCount = 1000},
            VkDescriptorPoolSize {.type = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, .descriptorCount = 1000}
        };

        VkDescriptorPoolCreateInfo pool_info = { };
        pool_info.sType             = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        pool_info.flags             = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        pool_info.maxSets           = 1000;
        pool_info.poolSizeCount     = static_cast<uint32_t>(pool_sizes.size());
        pool_info.pPoolSizes        = pool_sizes.data();

        VK_CHECK(vkCreateDescriptorPool(_device_handle, &pool_info, nullptr, &_descriptor_pool_handle));
    }

    VkDescriptorPool Device::descriptorPool() const noexcept
    {
        return _descriptor_pool_handle;
    }

    DescriptorSet Device::allocate(VkDescriptorSetLayout set_layout_handle)
    {
        if (set_layout_handle == VK_NULL_HANDLE)
            throw std::invalid_argument("[Device] set_layout_handle is null");

        DescriptorSet descriptor_set (this);

        VkDescriptorSetAllocateInfo allocate_info = { };
        allocate_info.sType                 = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocate_info.descriptorPool        = _descriptor_pool_handle;
        allocate_info.descriptorSetCount    = 1;
        allocate_info.pSetLayouts           = &set_layout_handle;
        
        VK_CHECK(vkAllocateDescriptorSets(_device_handle, &allocate_info, &descriptor_set.handle));

        return descriptor_set;
    }
}