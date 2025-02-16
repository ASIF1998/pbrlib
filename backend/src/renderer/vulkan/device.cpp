#include <pbrlib/rendering/window.hpp>

#include <backend/utils/versions.hpp>
#include <backend/logger/logger.hpp>

#include <backend/utils/vulkan.hpp>
#include <backend/renderer/vulkan/device.hpp>
#include <backend/renderer/vulkan/config.hpp>

#include <backend/renderer/vulkan/shader_compiler.hpp>

#include <SDL3/SDL_vulkan.h>

#include <vma/vk_mem_alloc.h>

#include <stdexcept>
#include <array>
#include <format>

namespace pbrlib::vk
{
    Device::~Device()
    {
        if (_descriptor_pool_handle != VK_NULL_HANDLE)
            vkDestroyDescriptorPool(_device_handle, _descriptor_pool_handle, nullptr);

        vkDestroyCommandPool(_device_handle, _command_pool_for_general_queue, nullptr);

        if (_vma_allocator_handle != VK_NULL_HANDLE)
            vmaDestroyAllocator(_vma_allocator_handle);

        if (_device_handle != VK_NULL_HANDLE)
            vkDestroyDevice(_device_handle, nullptr);

        if (_instance_handle != VK_NULL_HANDLE) 
           vkDestroyInstance(_instance_handle, nullptr);

        shader::finalizeCompiler();
    }

    void Device::init()
    {
        initInstance(config::enable_vulkan_debug_print);
        getPhysicalDevice();
        initDevice();

        loadFunctions();

        shader::initCompiler();

        initGpuAllocator();

        initCommandPools();

        createDescriptorPool();
    }
}

namespace pbrlib::vk
{
    void Device::initInstance(bool is_debug)
    {
        constexpr VkApplicationInfo app_info = 
        { 
            .sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .applicationVersion = 0,
            .pEngineName        = "pbrlib",
            .engineVersion      = utils::engineVersion(),
            .apiVersion         = utils::vulkanVersion()
        };

        VkInstanceCreateInfo instance_info = 
        { 
            .sType             = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pApplicationInfo  = &app_info
        };

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

        VkPhysicalDeviceDriverProperties driver_properties = { };
        driver_properties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DRIVER_PROPERTIES;

        _physical_device_handle = handles[0];
        _gpu_properties.sType   = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
        vkGetPhysicalDeviceProperties2(handles[0], &_gpu_properties);

        for (auto handle: handles) 
        {
            VkPhysicalDeviceProperties2 props = 
            { 
                .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2,
                .pNext = &driver_properties
            };

            vkGetPhysicalDeviceProperties2(handle, &props);

            if (props.properties.apiVersion >= utils::vulkanVersion())
            {
                _physical_device_handle = handle;
                _gpu_properties         = props;
                break;
            }
        }

        _gpu_properties.pNext = nullptr;

        if (_gpu_properties.properties.apiVersion < utils::vulkanVersion())
        {
            constexpr auto major = VK_VERSION_MAJOR(utils::vulkanVersion());
            constexpr auto minor = VK_VERSION_MINOR(utils::vulkanVersion());
            constexpr auto patch = VK_VERSION_PATCH(utils::vulkanVersion());

            throw std::runtime_error(std::format("[Vulkan] Vulkan API version less {}.{}.{}", major, minor, patch));
        }

        switch(_gpu_properties.properties.deviceType)
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

        pbrlib::log::engine::info("Device name: {}", _gpu_properties.properties.deviceName);
        pbrlib::log::engine::info("Driver name: {}", driver_properties.driverName);
        pbrlib::log::engine::info("Driver info: {}", driver_properties.driverInfo);

        switch (driver_properties.driverID)
        {
            case VK_DRIVER_ID_AMD_PROPRIETARY:
                pbrlib::log::engine::info("Driver ID: AMD proprietary");
                break;
            case VK_DRIVER_ID_AMD_OPEN_SOURCE: 
                pbrlib::log::engine::info("Driver ID: AMD open source");
                break;
            case VK_DRIVER_ID_MESA_RADV:
                pbrlib::log::engine::info("Driver ID: Mesa RADV");
                break;
            case VK_DRIVER_ID_NVIDIA_PROPRIETARY :
                pbrlib::log::engine::info("Driver ID: NVIDIA proprietary");
                break;
            case VK_DRIVER_ID_INTEL_PROPRIETARY_WINDOWS  :
                pbrlib::log::engine::info("Driver ID: INTEL proprietary Windows");
                break;
            case VK_DRIVER_ID_INTEL_OPEN_SOURCE_MESA   :
                pbrlib::log::engine::info("Driver ID: INTEL open source Mesa");
                break;
            case VK_DRIVER_ID_IMAGINATION_PROPRIETARY    :
                pbrlib::log::engine::info("Driver ID: Imagination proprietary");
                break;
            default:
                pbrlib::log::engine::info("Driver ID: Undefined");
                break;
        }
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

        const VkDeviceQueueCreateInfo queue_info = 
        { 
            .sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .queueFamilyIndex = _general_queue.family_index,
            .queueCount       = 1,
            .pQueuePriorities = &priority
        };

        std::vector extensions 
        {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        if (config::enable_vulkan_debug_marker)
            extensions.push_back(VK_EXT_DEBUG_MARKER_EXTENSION_NAME);
        
        VkPhysicalDeviceVulkan12Features vulkan_1_2_features = 
        {
            .sType                          = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES,
            .runtimeDescriptorArray         = VK_TRUE,
            .separateDepthStencilLayouts    = VK_TRUE,
            .bufferDeviceAddress            = VK_TRUE
        }; 

        VkPhysicalDeviceVulkan13Features vulkan_1_3_features = 
        {
            .sType              = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
            .pNext              = &vulkan_1_2_features,
            .synchronization2   = VK_TRUE
        };

        const VkDeviceCreateInfo device_info = 
        { 
            .sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .pNext                   = &vulkan_1_3_features,
            .queueCreateInfoCount    = 1,
            .pQueueCreateInfos       = &queue_info,
            .enabledExtensionCount   = static_cast<uint32_t>(extensions.size()),
            .ppEnabledExtensionNames = extensions.data()
        };

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

    const VkPhysicalDeviceProperties2& Device::gpuProperties() const noexcept
    {
        return _gpu_properties;
    }
}

namespace pbrlib::vk
{
    void Device::initGpuAllocator()
    {
        const VmaAllocatorCreateInfo allocator_info = 
        { 
            .physicalDevice   = _physical_device_handle,
            .device           = _device_handle,
            .instance         = _instance_handle,
            .vulkanApiVersion = pbrlib::utils::vulkanVersion()
        };

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
        const VkCommandPoolCreateInfo command_pool_info = 
        { 
            .sType             = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .queueFamilyIndex  = _general_queue.family_index
        };

        VK_CHECK(vkCreateCommandPool(_device_handle, &command_pool_info, nullptr, &_command_pool_for_general_queue));
    }

    CommandBuffer Device::oneTimeSubmitCommandBuffer(const Queue& queue, std::string_view name)
    {
        const VkCommandBufferAllocateInfo alloc_info = 
        { 
            .sType                = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .commandPool          = _command_pool_for_general_queue,
            .level                = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount   = 1
        };

        CommandBuffer command_buffer (this);

        VK_CHECK(vkAllocateCommandBuffers(_device_handle, &alloc_info, &command_buffer.handle));

        command_buffer.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

        if (!name.empty())
        {
            const VkDebugUtilsObjectNameInfoEXT name_info = 
            { 
                .sType         = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT,
                .objectType    = VK_OBJECT_TYPE_COMMAND_BUFFER,
                .objectHandle  = reinterpret_cast<uint64_t>(command_buffer.handle),
                .pObjectName   = name.data()
            };

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
        if (config::enable_vulkan_set_obj_name)
            _functions.vkSetDebugUtilsObjectNameEXT = loadFunction<PFN_vkSetDebugUtilsObjectNameEXT>(_device_handle, "vkSetDebugUtilsObjectNameEXT");

        if (config::enable_vulkan_debug_marker)
        {
            _functions.vkCmdDebugMarkerBeginEXT = loadFunction<PFN_vkCmdDebugMarkerBeginEXT>(_device_handle, "vkCmdDebugMarkerBeginEXT");
            _functions.vkCmdDebugMarkerEndEXT   = loadFunction<PFN_vkCmdDebugMarkerEndEXT>(_device_handle, "vkCmdDebugMarkerEndEXT");
        }
    }

    const VulkanFunctions& Device::vulkanFunctions() const noexcept
    {
        return _functions;
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

        const VkDescriptorPoolCreateInfo pool_info = 
        { 
            .sType             = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
            .flags             = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
            .maxSets           = 1000,
            .poolSizeCount     = static_cast<uint32_t>(pool_sizes.size()),
            .pPoolSizes        = pool_sizes.data()
        };

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

        VkDescriptorSetAllocateInfo allocate_info = 
        { 
            .sType                 = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
            .descriptorPool        = _descriptor_pool_handle,
            .descriptorSetCount    = 1,
            .pSetLayouts           = &set_layout_handle
        };
        
        VK_CHECK(vkAllocateDescriptorSets(_device_handle, &allocate_info, &descriptor_set.handle));

        return descriptor_set;
    }
}