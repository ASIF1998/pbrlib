#include <pbrlib/window.hpp>

#include <backend/utils/versions.hpp>
#include <backend/logger/logger.hpp>

#include <backend/renderer/vulkan/check.hpp>
#include <backend/renderer/vulkan/device.hpp>
#include <backend/renderer/vulkan/config.hpp>

#include <backend/renderer/vulkan/buffer.hpp>

#include <backend/renderer/vulkan/sync.hpp>

#include <backend/shaders/gpu_cpu_constants.h>

#include <SDL3/SDL_vulkan.h>

#include <ranges>

namespace pbrlib::backend::vk
{
    Device::~Device()
    {
        if (_device_handle != VK_NULL_HANDLE) [[likely]]
            vkDeviceWaitIdle(_device_handle);

        serializeGlobalPipelineCache();
    }

    void Device::init()
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        createInstance(config::enable_vulkan_debug_print);
        getPhysicalDevice();
        createDevice();
        createGlobalPipelineCache();
        createGpuAllocator();

        ResourceDestroyer::initForDeviceResources(
            _instance_handle,
            &_instance_functions,
            _device_handle,
            &_device_functions,
            _allocator_handle
        );

        createCommandPools();
        createDescriptorPool();
        createTracyContext();
    }
}

namespace pbrlib::backend::vk
{
    void Device::createInstance(bool is_debug)
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

        if (is_debug) [[unlikely]]
        {
            constexpr std::array layers
            {
                "VK_LAYER_LUNARG_api_dump",
                "VK_LAYER_KHRONOS_validation"
            };

            instance_info.enabledLayerCount     = static_cast<uint32_t>(layers.size());
            instance_info.ppEnabledLayerNames   = layers.data();
        }


        const auto extensions = instanceExtensions();

        instance_info.enabledExtensionCount     = static_cast<uint32_t>(extensions.size());
        instance_info.ppEnabledExtensionNames   = extensions.data();

        VK_CHECK(vkCreateInstance(&instance_info, nullptr, &_instance_handle.handle()));

        loadInstanceFunctions();

        if (is_debug) [[unlikely]]
            setupDebugUtilsMessenger();
    }

    std::vector<const char*> Device::instanceExtensions()
    {
#if defined (PBRLIB_OS_APPLE)
        const std::vector extensions
        {
            VK_KHR_SURFACE_EXTENSION_NAME,
            "VK_EXT_metal_surface",
            VK_EXT_DEBUG_UTILS_EXTENSION_NAME
        };

        return extensions;
#elif defined (PBRLIB_OS_WINDOWS)
        const std::vector extensions
        {
            VK_KHR_SURFACE_EXTENSION_NAME,
            "VK_KHR_win32_surface",
            VK_EXT_DEBUG_UTILS_EXTENSION_NAME
        };

        return extensions;
#else
        static_assert(false, "Linux platform is not yet supported");
#endif
    }

    VkInstance Device::instance() const noexcept
    {
        return _instance_handle;
    }
}

namespace pbrlib::backend::vk
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

        VkPhysicalDeviceDriverProperties driver_properties
        {
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DRIVER_PROPERTIES
        };

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

            if (props.properties.apiVersion >= utils::vulkanVersion()) [[likely]]
            {
                _physical_device_handle = handle;
                _gpu_properties         = props;
                break;
            }
        }

        _gpu_properties.pNext = nullptr;

        if (_gpu_properties.properties.apiVersion < backend::utils::vulkanVersion()) [[unlikely]]
        {
            constexpr auto major = VK_VERSION_MAJOR(backend::utils::vulkanVersion());
            constexpr auto minor = VK_VERSION_MINOR(backend::utils::vulkanVersion());
            constexpr auto patch = VK_VERSION_PATCH(backend::utils::vulkanVersion());

            throw pbrlib::exception::RuntimeError(std::format("[vk-device] vulkan api version less {}.{}.{}", major, minor, patch));
        }

        switch(_gpu_properties.properties.deviceType)
        {
            case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
                backend::log::info("[vk-device] type: discrete GPU");
                break;
            case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
                backend::log::info("[vk-device] type: integrated GPU");
                break;
            case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
                backend::log::info("[vk-device] type: virtual GPU");
                break;
            case VK_PHYSICAL_DEVICE_TYPE_CPU:
                backend::log::info("[vk-device] type: CPU");
                break;
            default:
                throw exception::RuntimeError("[vk-device] couldn't find gpu");
        }

        backend::log::info("[vk-device] name: {}", _gpu_properties.properties.deviceName);
        backend::log::info("[vk-device] driver name: {}", driver_properties.driverName);
        backend::log::info("[vk-device] driver info: {}", driver_properties.driverInfo);

        switch (driver_properties.driverID)
        {
            case VK_DRIVER_ID_AMD_PROPRIETARY:
                backend::log::info("[vk-device] driver ID: AMD proprietary");
                break;
            case VK_DRIVER_ID_AMD_OPEN_SOURCE:
                backend::log::info("[vk-device] driver ID: AMD open source");
                break;
            case VK_DRIVER_ID_MESA_RADV:
                backend::log::info("[vk-device] driver ID: Mesa RADV");
                break;
            case VK_DRIVER_ID_NVIDIA_PROPRIETARY:
                backend::log::info("[vk-device] driver ID: NVIDIA proprietary");
                break;
            case VK_DRIVER_ID_INTEL_PROPRIETARY_WINDOWS:
                backend::log::info("[vk-device] driver ID: INTEL proprietary Windows");
                break;
            case VK_DRIVER_ID_INTEL_OPEN_SOURCE_MESA:
                backend::log::info("[vk-device] driver ID: INTEL open source Mesa");
                break;
            case VK_DRIVER_ID_IMAGINATION_PROPRIETARY:
                backend::log::info("[vk-device] driver ID: Imagination proprietary");
                break;
            default:
                backend::log::info("[vk-device] driver ID: Undefined");
                break;
        }
    }

    VkPhysicalDevice Device::physicalDevice() const noexcept
    {
        return _physical_device_handle;
    }

    const VkPhysicalDeviceLimits& Device::limits() const noexcept
    {
        return _gpu_properties.properties.limits;
    }

    [[nodiscard]]
    const uint8_t Device::workGroupSize() const noexcept
    {
        return PBRLIB_WORK_GROUP_SIZE;
    }
}

namespace pbrlib::backend::vk
{
    void Device::getGeneralQueueIndex()
    {
        uint32_t family_count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(_physical_device_handle, &family_count, nullptr);

        std::vector<VkQueueFamilyProperties> families (family_count);
        vkGetPhysicalDeviceQueueFamilyProperties(_physical_device_handle, &family_count, families.data());

        std::vector<uint32_t> num_queues_in_family (families.size(), 0);

        constexpr auto queue_flags = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT;

        for (const auto index: std::views::iota(0u, families.size()))
        {
            const auto supportGraphics  = (families[index].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0;
            const auto supportCompute   = (families[index].queueFlags & VK_QUEUE_COMPUTE_BIT) != 0;
            const auto supportTransfer  = (families[index].queueFlags & VK_QUEUE_TRANSFER_BIT) != 0;

            if (supportGraphics && supportCompute && supportTransfer) [[likely]]
            {
                _general_queue.family_index = index;
                _general_queue.index        = num_queues_in_family[index]++;

                return ;
            }
        }

        throw exception::RuntimeError("[vk-device] couldn't find queue index");
        std::unreachable();
    }

    bool Device::isRunFromFrameDebugger() const
    {
        static constinit std::optional<bool> is_run_from_frame_debugger;

        if (is_run_from_frame_debugger) [[unlikely]]
            return is_run_from_frame_debugger.value();

        is_run_from_frame_debugger = false;

        uint32_t num_properties = 0u;

        vkEnumerateDeviceExtensionProperties(
            _physical_device_handle,
            nullptr,
            &num_properties, nullptr
        );

        std::vector<VkExtensionProperties> extension_properties (num_properties);

        vkEnumerateDeviceExtensionProperties(
            _physical_device_handle,
            nullptr,
            &num_properties, extension_properties.data()
        );

        for (size_t i = 0; i < extension_properties.size() && !is_run_from_frame_debugger.value(); ++i)
        {
            if (!std::strcmp(extension_properties[i].extensionName, VK_EXT_DEBUG_MARKER_EXTENSION_NAME))
                is_run_from_frame_debugger = true;
        }

        return is_run_from_frame_debugger.value();
    }

    void Device::createDevice()
    {
        getGeneralQueueIndex();

        constexpr float priority = 1.0f;

        const VkDeviceQueueCreateInfo queue_info
        {
            .sType              = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .queueFamilyIndex   = _general_queue.family_index,
            .queueCount         = 1,
            .pQueuePriorities   = &priority
        };

        std::vector extensions
        {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        if (isRunFromFrameDebugger()) [[unlikely]]
            extensions.push_back(VK_EXT_DEBUG_MARKER_EXTENSION_NAME);

        VkPhysicalDevice16BitStorageFeatures physical_device_16_bit_storage_features =
        {
            .sType                      = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_16BIT_STORAGE_FEATURES,
            .storageBuffer16BitAccess   = VK_TRUE
        };

        VkPhysicalDeviceVulkan12Features vulkan_1_2_features =
        {
            .sType                                          = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES,
            .pNext                                          = &physical_device_16_bit_storage_features,
            .storageBuffer8BitAccess                        = VK_TRUE,
            .uniformAndStorageBuffer8BitAccess              = VK_TRUE,
            .shaderFloat16                                  = VK_TRUE,
            .shaderInt8                                     = VK_TRUE,
            .descriptorBindingUniformBufferUpdateAfterBind  = VK_TRUE,
            .descriptorBindingSampledImageUpdateAfterBind   = VK_TRUE,
            .descriptorBindingStorageImageUpdateAfterBind   = VK_TRUE,
            .descriptorBindingStorageBufferUpdateAfterBind  = VK_TRUE,
            .runtimeDescriptorArray                         = VK_TRUE,
            .separateDepthStencilLayouts                    = VK_TRUE,
            .bufferDeviceAddress                            = VK_TRUE
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

        VK_CHECK(vkCreateDevice(
            _physical_device_handle,
            &device_info,
            nullptr,
            &_device_handle.handle()
        ));

        loadDeviceFunctions();

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

namespace pbrlib::backend::vk
{
    void Device::createGpuAllocator()
    {
        const VmaAllocatorCreateInfo allocator_info =
        {
            .flags              =  VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT,
            .physicalDevice     = _physical_device_handle,
            .device             = _device_handle,
            .instance           = _instance_handle,
            .vulkanApiVersion   = backend::utils::vulkanVersion()
        };

        VK_CHECK(vmaCreateAllocator(&allocator_info, &_allocator_handle.handle()));
    }
}

namespace pbrlib::backend::vk
{
    VmaAllocator Device::vmaAllocator() const noexcept
    {
        return _allocator_handle;
    }
}

namespace pbrlib::backend::vk
{
    void Device::createCommandPools()
    {
        const VkCommandPoolCreateInfo command_pool_info =
        {
            .sType              = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .flags              = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
            .queueFamilyIndex   = _general_queue.family_index
        };

        VK_CHECK(vkCreateCommandPool(
            _device_handle,
            &command_pool_info,
            nullptr,
            &_command_pool_for_general_queue.handle()
        ));
    }

    CommandBuffer Device::oneTimeSubmitCommandBuffer(std::string_view name)
    {
        CommandBuffer command_buffer (*this, _command_pool_for_general_queue);

        if (!name.empty()) [[likely]]
        {
            const VkDebugUtilsObjectNameInfoEXT name_info =
            {
                .sType          = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT,
                .objectType     = VK_OBJECT_TYPE_COMMAND_BUFFER,
                .objectHandle   = reinterpret_cast<uint64_t>(command_buffer.handle.handle()),
                .pObjectName    = name.data()
            };

            setName(name_info);
        }

        return command_buffer;
    }

    void Device::submit(const CommandBuffer& command_buffer)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        if (_submit_fence_handle == VK_NULL_HANDLE) [[unlikely]]
        {
            constexpr VkFenceCreateInfo fence_create_info
            {
                .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO
            };

            _submit_fence_handle = create(_device_handle, fence_create_info);
        }

        submit(command_buffer, VK_NULL_HANDLE, VK_NULL_HANDLE, _submit_fence_handle);
        sync(_device_handle, _submit_fence_handle);
    }

    void Device::submit (
        const CommandBuffer&    command_buffer,
        VkSemaphore             wait_semaphore_handle,
        VkSemaphore             signal_semaphore_handle,
        VkFence                 fence_handle
    )
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

#ifdef PBRLIB_ENABLE_PROFILING
        TracyVkCollect(_tracy_ctx_handle.handle(), command_buffer.handle);
#endif

        vkEndCommandBuffer(command_buffer.handle);

        const VkCommandBufferSubmitInfo command_buffer_info
        {
            .sType          = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
            .commandBuffer  = command_buffer.handle
        };

        VkSubmitInfo2KHR submit_info
        {
            .sType                  = VK_STRUCTURE_TYPE_SUBMIT_INFO_2_KHR,
            .commandBufferInfoCount = 1,
            .pCommandBufferInfos    = &command_buffer_info
        };

        constexpr auto make_semaphore_info = [] (VkSemaphore semaphore_handle)
        {
            const VkSemaphoreSubmitInfo submit_info
            {
                .sType      = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
                .semaphore  = semaphore_handle,
                .stageMask  = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT,
            };

            return submit_info;
        };

        const auto wait_semaphore_info = make_semaphore_info(wait_semaphore_handle);
        if (wait_semaphore_handle != VK_NULL_HANDLE)
        {
            submit_info.pWaitSemaphoreInfos     = &wait_semaphore_info;
            submit_info.waitSemaphoreInfoCount  = 1;
        }

        const auto signal_semaphore_info = make_semaphore_info(signal_semaphore_handle);
        if (signal_semaphore_handle != VK_NULL_HANDLE)
        {
            submit_info.pSignalSemaphoreInfos       = &signal_semaphore_info;
            submit_info.signalSemaphoreInfoCount    = 1;
        }

        VK_CHECK(vkQueueSubmit2(_general_queue.handle, 1, &submit_info, fence_handle));
    }
}

namespace pbrlib::backend::vk
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
            log::info("[vk-function-loader] {}", function_name);
        else
            log::error("[vk-function-loader] {}", function_name);

        return ptr_function;
    }

    template<typename VulkanFunctionType>
    VulkanFunctionType loadFunction(VkInstance instance_handle, const std::string_view function_name)
    {
        auto ptr_function = reinterpret_cast<VulkanFunctionType>(
            vkGetInstanceProcAddr(
                instance_handle,
                function_name.data()
            )
        );

        if (ptr_function)
            log::info("[vk-function-loader] {}", function_name);
        else
            log::error("[vk-function-loader] {}", function_name);

        return ptr_function;
    }

    void Device::loadDeviceFunctions()
    {
        if (config::enable_vulkan_set_obj_name)
            _device_functions.vkSetDebugUtilsObjectNameEXT = loadFunction<PFN_vkSetDebugUtilsObjectNameEXT>(_device_handle, "vkSetDebugUtilsObjectNameEXT");

        if (isRunFromFrameDebugger()) [[unlikely]]
        {
            _device_functions.vkCmdDebugMarkerBeginEXT = loadFunction<PFN_vkCmdDebugMarkerBeginEXT>(_device_handle, "vkCmdDebugMarkerBeginEXT");
            _device_functions.vkCmdDebugMarkerEndEXT   = loadFunction<PFN_vkCmdDebugMarkerEndEXT>(_device_handle, "vkCmdDebugMarkerEndEXT");
        }
    }

    void Device::loadInstanceFunctions()
    {
        if constexpr (config::enable_vulkan_debug_print)
        {
            _instance_functions.vkCreateDebugUtilsMessengerEXT  = loadFunction<PFN_vkCreateDebugUtilsMessengerEXT>(_instance_handle, "vkCreateDebugUtilsMessengerEXT");
            _instance_functions.vkDestroyDebugUtilsMessengerEXT = loadFunction<PFN_vkDestroyDebugUtilsMessengerEXT>(_instance_handle, "vkDestroyDebugUtilsMessengerEXT");
        }
    }

    const DeviceFunctions& Device::deviceFunctions() const noexcept
    {
        return _device_functions;
    }

    const InstanceFunctions& Device::instanceFunctions() const noexcept
    {
        return _instance_functions;
    }
}

namespace pbrlib::backend::vk
{
    void Device::setName(const VkDebugUtilsObjectNameInfoEXT& name_info) const
    {
        if (_device_functions.vkSetDebugUtilsObjectNameEXT) [[likely]]
            _device_functions.vkSetDebugUtilsObjectNameEXT(_device_handle, &name_info);
    }
}

namespace pbrlib::backend::vk
{
    void Device::createDescriptorPool()
    {
        constexpr uint32_t image_count = 10000;

        constexpr std::array pool_sizes
        {
            VkDescriptorPoolSize {.type = VK_DESCRIPTOR_TYPE_SAMPLER, .descriptorCount = 1000},
            VkDescriptorPoolSize {.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, .descriptorCount = image_count},
            VkDescriptorPoolSize {.type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, .descriptorCount = image_count},
            VkDescriptorPoolSize {.type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, .descriptorCount = image_count},
            VkDescriptorPoolSize {.type = VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, .descriptorCount = 1000},
            VkDescriptorPoolSize {.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, .descriptorCount = 1000},
            VkDescriptorPoolSize {.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, .descriptorCount = 1000},
            VkDescriptorPoolSize {.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, .descriptorCount = 1000},
            VkDescriptorPoolSize {.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, .descriptorCount = 1000},
            VkDescriptorPoolSize {.type = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, .descriptorCount = 1000}
        };

        constexpr auto flags =
                VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT
            |   VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT_EXT;

        const VkDescriptorPoolCreateInfo pool_info =
        {
            .sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
            .flags              = flags,
            .maxSets            = 1000,
            .poolSizeCount      = static_cast<uint32_t>(pool_sizes.size()),
            .pPoolSizes         = pool_sizes.data()
        };

        VK_CHECK(vkCreateDescriptorPool(
            _device_handle,
            &pool_info,
            nullptr,
            &_descriptor_pool_handle.handle()
        ));
    }

    VkDescriptorPool Device::descriptorPool() const noexcept
    {
        return _descriptor_pool_handle;
    }

    DescriptorSetHandle Device::allocateDescriptorSet(VkDescriptorSetLayout desc_set_layout_handle, std::string_view name) const
    {
        VkDescriptorSet descriptor_set_handle = VK_NULL_HANDLE;

        const VkDescriptorSetAllocateInfo allocate_info =
        {
            .sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
            .descriptorPool     = _descriptor_pool_handle,
            .descriptorSetCount = 1,
            .pSetLayouts        = &desc_set_layout_handle
        };

        VK_CHECK(vkAllocateDescriptorSets(_device_handle, &allocate_info, &descriptor_set_handle));

        if (!name.empty()) [[likely]]
        {
            const VkDebugUtilsObjectNameInfoEXT name_info
            {
                .sType          = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT,
                .objectType     = VK_OBJECT_TYPE_DESCRIPTOR_SET,
                .objectHandle   = reinterpret_cast<uint64_t>(descriptor_set_handle),
                .pObjectName    = name.data()
            };

            setName(name_info);
        }

        return DescriptorSetHandle(descriptor_set_handle, _descriptor_pool_handle.handle());
    }

    vk::SamplerHandle Device::createLinearSampler()
    {
        constexpr VkSamplerCreateInfo sampler_create_info
        {
            .sType          = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
            .magFilter      = VK_FILTER_LINEAR,
            .minFilter      = VK_FILTER_LINEAR
        };

        vk::SamplerHandle sampler_handle;

        VK_CHECK(vkCreateSampler (
            _device_handle,
            &sampler_create_info,
            nullptr,
            &sampler_handle.handle()
        ));

        return sampler_handle;
    }

    vk::SamplerHandle Device::createNearestSampler()
    {
        constexpr VkSamplerCreateInfo sampler_create_info
        {
            .sType          = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
            .magFilter      = VK_FILTER_NEAREST,
            .minFilter      = VK_FILTER_NEAREST
        };

        vk::SamplerHandle sampler_handle;

        VK_CHECK(vkCreateSampler (
            _device_handle,
            &sampler_create_info,
            nullptr,
            &sampler_handle.handle()
        ));

        return sampler_handle;
    }
}

namespace pbrlib::backend::vk
{
    void Device::createTracyContext()
    {
#ifdef PBRLIB_ENABLE_PROFILING
        auto tracy_setup_command_buffer = oneTimeSubmitCommandBuffer("tracy-setup");
        auto tracy_ctx_handle = TracyVkContext(
            _physical_device_handle,
            _device_handle,
            _general_queue.handle,
            tracy_setup_command_buffer.handle
        );

        if (!tracy_ctx_handle) [[unlikely]]
            throw exception::InitializeError("[vk-device] failed create tracy context for profiling");

        _tracy_ctx_handle = TracyCtxHandle(tracy_ctx_handle);
#endif
    }
}

namespace pbrlib::backend::vk
{
    VKAPI_ATTR VkBool32 VKAPI_CALL debugUtilsMessageCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT      severity,
        VkDebugUtilsMessageTypeFlagsEXT             type,
        const VkDebugUtilsMessengerCallbackDataEXT* ptr_callback_data,
        void*                                       ptr_user_data
    )
    {
        if (!ptr_callback_data || !ptr_callback_data->pMessage) [[unlikely]]
            return VK_FALSE;

        constexpr auto prefix = [] (VkDebugUtilsMessageTypeFlagsEXT type)
        {
            if (type & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT)
                return "validation";

            if (type & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT)
                return "performance";

            return "general";
        };

        std::string header = std::format("[{}]", prefix(type));

        if (ptr_callback_data->pMessageIdName)
            header += std::format("[{}]", ptr_callback_data->pMessageIdName);
        else
            header += "[no-id]";

        if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
            log::error("[vk-device][{}] {}", header, ptr_callback_data->pMessage);
        else if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
            log::warning("[vk-device][{}] {}", header, ptr_callback_data->pMessage);
        else
            log::info("[vk-device][{}] {}", header, ptr_callback_data->pMessage);

        return severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT ? VK_TRUE : VK_FALSE;
    }

    void Device::setupDebugUtilsMessenger()
    {
        if (!_instance_functions.vkCreateDebugUtilsMessengerEXT) [[unlikely]]
        {
            log::error("[vk-device] failed to load vkCreateDebugUtilsMessengerEXT, is VK_EXT_debug_utils extension enabled?");
            return ;
        }

        const VkDebugUtilsMessengerCreateInfoEXT messenger_create_info
        {
            .sType              = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
            .messageSeverity    = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
            .messageType        = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT,
            .pfnUserCallback    = debugUtilsMessageCallback
        };

        VK_CHECK(_instance_functions.vkCreateDebugUtilsMessengerEXT(
            _instance_handle,
            &messenger_create_info,
            nullptr,
            &_debug_utils_messenger_handle.handle()
        ));
    }
}

namespace pbrlib::backend::vk
{
    /// @link https://zeux.io/2019/07/17/serializing-pipeline-cache/
    struct PipelineCachePrefixHeader final
    {
        uint32_t    magic               = 0x5042524c;
        uint32_t    vendor_id           = 0;
        uint32_t    device_id           = 0;
        uint32_t    driver_version      = 0;
        uint64_t    size                = 0;
        uint8_t     uuid [VK_UUID_SIZE] = { };
    };

    static const std::string global_pipeline_cache_name = "global.pbrlib-pso-cache";

    bool checkPipelineCacheHeader(const Device& device, const PipelineCachePrefixHeader& header) noexcept
    {
        if (header.magic != PipelineCachePrefixHeader().magic) [[unlikely]]
            return false;

        const auto& gpu_properties = device.gpuProperties().properties;

        if (header.vendor_id != gpu_properties.vendorID) [[unlikely]]
            return false;

        if (header.device_id != gpu_properties.deviceID) [[unlikely]]
            return false;

        if (header.driver_version != gpu_properties.driverVersion) [[unlikely]]
            return false;

        if (memcmp(header.uuid, gpu_properties.pipelineCacheUUID, VK_UUID_SIZE)) [[unlikely]]
            return false;

        return true;
    }

    void Device::createGlobalPipelineCache()
    {
        std::vector<char> cache;
        if (std::ifstream pso_cache(global_pipeline_cache_name, std::ios::binary); pso_cache) [[likely]]
        {
            PipelineCachePrefixHeader header;
            pso_cache.read(reinterpret_cast<char*>(&header), static_cast<std::streamsize>(sizeof(PipelineCachePrefixHeader)));
            if (checkPipelineCacheHeader(*this, header)) [[likely]]
            {
                cache.resize(header.size);
                if (!pso_cache.read(cache.data(), header.size)) [[likely]]
                {
                    log::warning("[device] failed to read cache file: {}", global_pipeline_cache_name);
                    cache.clear();
                }
            }
        }

        const VkPipelineCacheCreateInfo pipeline_cache_create_info
        {
            .sType              = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO,
            .initialDataSize    = cache.size(),
            .pInitialData       = !cache.empty() ? cache.data() : nullptr
        };

        VK_CHECK(vkCreatePipelineCache(
            _device_handle,
            &pipeline_cache_create_info,
            nullptr,
            &_global_pipeline_cache_handle.handle()
        ));
    }

    void Device::serializeGlobalPipelineCache()
    {
        if (!_global_pipeline_cache_handle) [[unlikely]]
        {
            log::warning("[device] failed to serialize pipeline cache: handle is null");
            return ;
        }

        size_t size = 0;
        if (vkGetPipelineCacheData(_device_handle, _global_pipeline_cache_handle, &size, nullptr) != VK_SUCCESS) [[unlikely]]
        {
            log::warning("[device] failed save pipeline cache: {}", global_pipeline_cache_name);
            return;
        }

        std::vector<char> data (size);
        if (vkGetPipelineCacheData(_device_handle, _global_pipeline_cache_handle, &size, data.data()) != VK_SUCCESS) [[unlikely]]
        {
            log::warning("[device] failed save pipeline cache: {}", global_pipeline_cache_name);
            return;
        }

        const auto& gpu_properties = _gpu_properties.properties;

        PipelineCachePrefixHeader header
        {
            .vendor_id      = gpu_properties.vendorID,
            .device_id      = gpu_properties.deviceID,
            .driver_version = gpu_properties.driverVersion,
            .size           = size
        };
        memcpy(header.uuid, gpu_properties.pipelineCacheUUID, VK_UUID_SIZE);

        std::ofstream file (global_pipeline_cache_name, std::ios::binary | std::ios::trunc);
        if (file) [[likely]]
        {
            const auto ptr_header   = reinterpret_cast<const char*>(&header);
            const auto header_size  = static_cast<std::streamsize>(sizeof(PipelineCachePrefixHeader));
            file.write(ptr_header, header_size);
            file.write(data.data(), static_cast<std::streamsize>(data.size()));
        }
        else
            log::warning("[device] failed save pipeline cache: {}", global_pipeline_cache_name);
    }

    VkPipelineCache Device::globalPipelineCache() const noexcept
    {
        return _global_pipeline_cache_handle;
    }
}
