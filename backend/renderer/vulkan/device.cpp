#include <pbrlib/window.hpp>

#include <backend/utils/versions.hpp>
#include <backend/logger/logger.hpp>

#include <backend/utils/vulkan.hpp>
#include <backend/renderer/vulkan/device.hpp>
#include <backend/renderer/vulkan/config.hpp>

#include <backend/renderer/vulkan/shader_compiler.hpp>

#include <backend/renderer/vulkan/buffer.hpp>

#include <backend/shaders/gpu_cpu_constants.h>

#include <SDL3/SDL_vulkan.h>

#include <vma/vk_mem_alloc.h>

#include <stdexcept>
#include <array>
#include <format>

#include <ranges>

namespace pbrlib::backend::vk
{
    Device::~Device()
    {
        if (_device_handle == VK_NULL_HANDLE)
            return;

        vkDeviceWaitIdle(_device_handle);

#ifdef PBRLIB_ENABLE_PROPFILING
        TracyVkDestroy(_tracy_ctx);
#endif

        vkDestroyFence(_device_handle, _submit_fence_handle, nullptr);
        vkDestroyCommandPool(_device_handle, _command_pool_for_general_queue, nullptr);
        vmaDestroyAllocator(_vma_allocator_handle);

        shader::finalizeCompiler();
    }

    void Device::init()
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        createInstance(config::enable_vulkan_debug_print);
        getPhysicalDevice();
        createDevice();

        loadFunctions();

        shader::initCompiler();

        createGpuAllocator();

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

        VK_CHECK(vkCreateInstance(&instance_info, nullptr, &_instance_handle.get()));
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

            if (props.properties.apiVersion >= utils::vulkanVersion())
            {
                _physical_device_handle = handle;
                _gpu_properties         = props;
                break;
            }
        }

        _gpu_properties.pNext = nullptr;

        if (_gpu_properties.properties.apiVersion < backend::utils::vulkanVersion())
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
            case VK_DRIVER_ID_NVIDIA_PROPRIETARY :
                backend::log::info("[vk-device] driver ID: NVIDIA proprietary");
                break;
            case VK_DRIVER_ID_INTEL_PROPRIETARY_WINDOWS  :
                backend::log::info("[vk-device] driver ID: INTEL proprietary Windows");
                break;
            case VK_DRIVER_ID_INTEL_OPEN_SOURCE_MESA   :
                backend::log::info("[vk-device] driver ID: INTEL open source Mesa");
                break;
            case VK_DRIVER_ID_IMAGINATION_PROPRIETARY    :
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

            if (supportGraphics && supportCompute && supportTransfer)
            {
                _general_queue.family_index = index;
                _general_queue.index        = num_queues_in_family[index]++;

                return ;
            }
        }

        throw exception::RuntimeError("[vk-device] couldn't find queue index");
    }

    bool Device::isRunFromFrameDebugger() const
    {
        static constinit std::optional<bool> is_run_from_frame_debugger;

        if (is_run_from_frame_debugger)
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

        if (isRunFromFrameDebugger())
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

        VK_CHECK(vkCreateDevice(_physical_device_handle, &device_info, nullptr, &_device_handle.get()));

        HandleDispatcher::initForDeviceResources(_device_handle);

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

        VK_CHECK(vmaCreateAllocator(&allocator_info, &_vma_allocator_handle));
    }
}

namespace pbrlib::backend::vk
{
    VmaAllocator Device::vmaAllocator() const noexcept
    {
        return _vma_allocator_handle;
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

        VK_CHECK(vkCreateCommandPool(_device_handle, &command_pool_info, nullptr, &_command_pool_for_general_queue));
    }

    CommandBuffer Device::oneTimeSubmitCommandBuffer(std::string_view name)
    {
        CommandBuffer command_buffer (*this, _command_pool_for_general_queue);

        if (!name.empty())
        {
            const VkDebugUtilsObjectNameInfoEXT name_info = 
            { 
                .sType          = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT,
                .objectType     = VK_OBJECT_TYPE_COMMAND_BUFFER,
                .objectHandle   = reinterpret_cast<uint64_t>(command_buffer.handle),
                .pObjectName    = name.data()
            };

            setName(name_info);
        }

        return command_buffer;
    }

    void Device::submit(const CommandBuffer& command_buffer)
    {
        PBRLIB_PROFILING_ZONE_SCOPED;

        if (_submit_fence_handle == VK_NULL_HANDLE)
        {
            constexpr VkFenceCreateInfo fence_create_info 
            { 
                .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO
            };

            VK_CHECK(vkCreateFence(_device_handle, &fence_create_info, nullptr, &_submit_fence_handle));
        }

#ifdef PBRLIB_ENABLE_PROPFILING
        TracyVkCollect(_tracy_ctx, command_buffer.handle);
#endif

        vkEndCommandBuffer(command_buffer.handle);

        const VkCommandBufferSubmitInfo command_buffer_info 
        { 
            .sType          = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
            .commandBuffer  = command_buffer.handle
        };

        const VkSubmitInfo2KHR submit_info
        { 
            .sType                  = VK_STRUCTURE_TYPE_SUBMIT_INFO_2_KHR,
            .commandBufferInfoCount = 1,
            .pCommandBufferInfos    = &command_buffer_info
        };

        VK_CHECK(vkQueueSubmit2(_general_queue.handle, 1, &submit_info, _submit_fence_handle));
        VK_CHECK(vkWaitForFences(_device_handle, 1, &_submit_fence_handle, VK_TRUE, std::numeric_limits<uint64_t>::max()));
        VK_CHECK(vkResetFences(_device_handle, 1, &_submit_fence_handle));
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

    void Device::loadFunctions()
    {
        if (config::enable_vulkan_set_obj_name)
            _functions.vkSetDebugUtilsObjectNameEXT = loadFunction<PFN_vkSetDebugUtilsObjectNameEXT>(_device_handle, "vkSetDebugUtilsObjectNameEXT");

        if (isRunFromFrameDebugger())
        {
            _functions.vkCmdDebugMarkerBeginEXT = loadFunction<PFN_vkCmdDebugMarkerBeginEXT>(_device_handle, "vkCmdDebugMarkerBeginEXT");
            _functions.vkCmdDebugMarkerEndEXT   = loadFunction<PFN_vkCmdDebugMarkerEndEXT>(_device_handle, "vkCmdDebugMarkerEndEXT");
        }
    }

    const Functions& Device::vulkanFunctions() const noexcept
    {
        return _functions;
    }
}

namespace pbrlib::backend::vk
{
    void Device::setName(const VkDebugUtilsObjectNameInfoEXT& name_info) const
    {
        if (_functions.vkSetDebugUtilsObjectNameEXT)
            _functions.vkSetDebugUtilsObjectNameEXT(_device_handle, &name_info);
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

        VK_CHECK(vkCreateDescriptorPool(_device_handle, &pool_info, nullptr, &_descriptor_pool_handle.get()));
    }

    VkDescriptorPool Device::descriptorPool() const noexcept
    {
        return _descriptor_pool_handle;
    }

    vk::DescriptorSetHandle Device::allocateDescriptorSet(VkDescriptorSetLayout desc_set_layout_handle, std::string_view name) const
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

        if (!name.empty())
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

        return vk::DescriptorSetHandle(descriptor_set_handle, _descriptor_pool_handle.get());
    }

    void Device::writeDescriptorSet(const DescriptorImageInfo& descriptor_image_info) const
    {
        if (descriptor_image_info.view_handle == VK_NULL_HANDLE) [[unlikely]]
            throw exception::InvalidArgument("[device] descriptor_image_info.view_handle is null");
        
        if (descriptor_image_info.set_handle == VK_NULL_HANDLE) [[unlikely]]
            throw exception::InvalidArgument("[device] descriptor_image_info.set_handle is null");
        
        if (descriptor_image_info.expected_image_layout == VK_IMAGE_LAYOUT_UNDEFINED) [[unlikely]]
            throw exception::InvalidArgument("[device] descriptor_image_info.expected_image_layout is undefined");

        const auto descriptor_type = descriptor_image_info.sampler_handle == VK_NULL_HANDLE 
            ?   VK_DESCRIPTOR_TYPE_STORAGE_IMAGE 
            :   VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

        const VkDescriptorImageInfo image_info
        {
            .sampler        = descriptor_image_info.sampler_handle,
            .imageView      = descriptor_image_info.view_handle,
            .imageLayout    = descriptor_image_info.expected_image_layout 
        };

        const VkWriteDescriptorSet write_info 
        {
            .sType              = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstSet             = descriptor_image_info.set_handle,
            .dstBinding         = descriptor_image_info.binding,
            .dstArrayElement    = descriptor_image_info.array_element,
            .descriptorCount    = 1,
            .descriptorType     = descriptor_type,
            .pImageInfo         = &image_info
        };

        vkUpdateDescriptorSets (
            _device_handle,
            1, &write_info,
            0, nullptr
        );
    }

    void Device::writeDescriptorSet(const DescriptorBufferInfo& descriptor_buffer_info) const
    {
        if (descriptor_buffer_info.buffer.handle == VK_NULL_HANDLE) [[unlikely]]
            throw exception::InvalidArgument("[device] descriptor_buffer_info.buffer.handle is null");
        
        if (descriptor_buffer_info.set_handle == VK_NULL_HANDLE) [[unlikely]]
            throw exception::InvalidArgument("[device] descriptor_buffer_info.set_handle is null");

        const VkDescriptorBufferInfo buffer_info
        {
            .buffer	= descriptor_buffer_info.buffer.handle,
            .offset	= descriptor_buffer_info.offset,
            .range	= descriptor_buffer_info.size
        };

        const auto descriptor_type = descriptor_buffer_info.buffer.usage & VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT
            ?   VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER
            :   VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;

        const VkWriteDescriptorSet write_info
        {
            .sType              = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstSet             = descriptor_buffer_info.set_handle,
            .dstBinding         = descriptor_buffer_info.binding,
            .dstArrayElement    = descriptor_buffer_info.array_element,
            .descriptorCount    = 1,
            .descriptorType     = descriptor_type,
            .pBufferInfo        = &buffer_info
        };

        vkUpdateDescriptorSets (
            _device_handle,
            1, &write_info,
            0, nullptr
        );
    }
}

namespace pbrlib::backend::vk
{
    void Device::createTracyContext()
    {
#ifdef PBRLIB_ENABLE_PROPFILING
        auto tracy_setup_command_buffer = oneTimeSubmitCommandBuffer("tracy-setup");
        _tracy_ctx = TracyVkContextCalibrated(
            _physical_device_handle, 
            _device_handle, 
            _general_queue.handle, 
            tracy_setup_command_buffer.handle, 
            VK_NULL_HANDLE,
            VK_NULL_HANDLE
        );

        if (!_tracy_ctx)
            throw exception::InitializeError("[vk-device] failed create tracy context for profiling");
#endif
    }
}
