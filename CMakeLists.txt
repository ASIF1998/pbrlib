cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQURED ON)

project(PBRLib)

option(PBRLIB_ENABLE_TESTS "Осуществлять тестирование." ON)

set(PRLIB_ENGINE_VERSION_MAJOR 0)
set(PRLIB_ENGINE_VERSION_MINOR 0)
set(PRLIB_ENGINE_VERSION_PATCH 1)

set(PRLIB_VULKAN_VERSION_MAJOR 1)
set(PRLIB_VULKAN_VERSION_MINOR 2)
set(PRLIB_VULKAN_VERSION_PATCH 0)

configure_file(src/core.hpp.in ../src/core.hpp)

set(PBRLIB_MATH_HEADERS
    ${PROJECT_SOURCE_DIR}/src/math/matrix2x2.hpp
    ${PROJECT_SOURCE_DIR}/src/math/matrix2x2.inl
    ${PROJECT_SOURCE_DIR}/src/math/matrix3x3.hpp
    ${PROJECT_SOURCE_DIR}/src/math/matrix3x3.inl
    ${PROJECT_SOURCE_DIR}/src/math/matrix4x4.hpp
    ${PROJECT_SOURCE_DIR}/src/math/matrix4x4.inl
    ${PROJECT_SOURCE_DIR}/src/math/quaternion.hpp
    ${PROJECT_SOURCE_DIR}/src/math/quaternion.inl
    ${PROJECT_SOURCE_DIR}/src/math/vec2.hpp
    ${PROJECT_SOURCE_DIR}/src/math/vec2.inl
    ${PROJECT_SOURCE_DIR}/src/math/vec3.hpp
    ${PROJECT_SOURCE_DIR}/src/math/vec3.inl
    ${PROJECT_SOURCE_DIR}/src/math/vec4.hpp
    ${PROJECT_SOURCE_DIR}/src/math/vec4.inl
)

set(PBRLIB_MEMORY_SOURCE
    ${PROJECT_SOURCE_DIR}/src/Memory/MemoryUtils.cpp
)

set(PBRLIB_MEMORY_HEADERS
    ${PROJECT_SOURCE_DIR}/src/Memory/IMemoryBlock.hpp
    ${PROJECT_SOURCE_DIR}/src/Memory/MemoryArena.hpp
    ${PROJECT_SOURCE_DIR}/src/Memory/MemoryArena.inl
    ${PROJECT_SOURCE_DIR}/src/Memory/MemoryBlock.hpp
    ${PROJECT_SOURCE_DIR}/src/Memory/MemoryBlock.inl
    ${PROJECT_SOURCE_DIR}/src/Memory/MemoryUtils.hpp
    ${PROJECT_SOURCE_DIR}/src/Memory/STLAlignedAllocator.hpp
    ${PROJECT_SOURCE_DIR}/src/Memory/STLAlignedAllocator.inl
    ${PROJECT_SOURCE_DIR}/src/Memory/STLArenaLinearAllocator.hpp
    ${PROJECT_SOURCE_DIR}/src/Memory/STLArenaLinearAllocator.inl
)

set(PBRLIB_MOVING_SOURCE 
    ${PROJECT_SOURCE_DIR}/src/Moving/Transform.cpp
)

set(PBRLIB_MOVING_HEADERS
    ${PROJECT_SOURCE_DIR}/src/Moving/Transform.hpp
)

set(PBRLIB_WINDOW_SOURCE
    ${PROJECT_SOURCE_DIR}/src/Rendering/Window.cpp
)

set(PBRLIB_WINDOW_HEADERS
    ${PROJECT_SOURCE_DIR}/src/Rendering/Window.hpp
)

set(PBRLIB_CAMERAS_SOURCE
    ${PROJECT_SOURCE_DIR}/src/Rendering/Cameras/CameraBase.cpp
    ${PROJECT_SOURCE_DIR}/src/Rendering/Cameras/PerspectiveCamera.cpp
)   

set(PBRLIB_CAMERAS_HEADERS
    ${PROJECT_SOURCE_DIR}/src/Rendering/Cameras/CameraBase.hpp
    ${PROJECT_SOURCE_DIR}/src/Rendering/Cameras/PerspectiveCamera.hpp
)   

set(PBRLIB_GEOMETRY_SOURCE
    ${PROJECT_SOURCE_DIR}/src/Rendering/Geometry/AABB.cpp
    ${PROJECT_SOURCE_DIR}/src/Rendering/Geometry/MeshAssimp.cpp
    ${PROJECT_SOURCE_DIR}/src/Rendering/Geometry/MeshManager.cpp
)

set(PBRLIB_GEOMETRY_HEADERS
    ${PROJECT_SOURCE_DIR}/src/Rendering/Geometry/AABB.hpp
    ${PROJECT_SOURCE_DIR}/src/Rendering/Geometry/AABB.inl
    ${PROJECT_SOURCE_DIR}/src/Rendering/Geometry/Mesh.hpp
    ${PROJECT_SOURCE_DIR}/src/Rendering/Geometry/Mesh.inl
    ${PROJECT_SOURCE_DIR}/src/Rendering/Geometry/MeshAssimp.hpp
    ${PROJECT_SOURCE_DIR}/src/Rendering/Geometry/MeshManager.hpp
)   

set(PBRLIB_LIGHTS_SOURCE
    ${PROJECT_SOURCE_DIR}/src/Rendering/Lights/DirectionLight.cpp
    ${PROJECT_SOURCE_DIR}/src/Rendering/Lights/PointLight.cpp
    ${PROJECT_SOURCE_DIR}/src/Rendering/Lights/SpotLight.cpp
)   

set(PBRLIB_LIGHTS_HEADERS
    ${PROJECT_SOURCE_DIR}/src/Rendering/Lights/DirectionLight.hpp
    ${PROJECT_SOURCE_DIR}/src/Rendering/Lights/PointLight.hpp
    ${PROJECT_SOURCE_DIR}/src/Rendering/Lights/SpotLight.hpp
)   

set(PBRLIB_MATERIAL_SOURCE
    ${PROJECT_SOURCE_DIR}/src/Rendering/Material/Material.cpp
    ${PROJECT_SOURCE_DIR}/src/Rendering/Material/MaterialManager.cpp
)

set(PBRLIB_MATERIAL_HEADERS
    ${PROJECT_SOURCE_DIR}/src/Rendering/Material/Material.hpp
    ${PROJECT_SOURCE_DIR}/src/Rendering/Material/Material.inl   
    ${PROJECT_SOURCE_DIR}/src/Rendering/Material/MaterialManager.hpp
)

set(PBRLIB_SUB_PASSES_SOURCE
    ${PROJECT_SOURCE_DIR}/src/Rendering/Renderers/SubPasses/GBufferPass.cpp
    ${PROJECT_SOURCE_DIR}/src/Rendering/Renderers/SubPasses/PBRPass.cpp
)

set(PBRLIB_SUB_PASSES_HEADERS
    ${PROJECT_SOURCE_DIR}/src/Rendering/Renderers/SubPasses/GBufferPass.hpp
    ${PROJECT_SOURCE_DIR}/src/Rendering/Renderers/SubPasses/Pass.hpp
    ${PROJECT_SOURCE_DIR}/src/Rendering/Renderers/SubPasses/PBRPass.hpp
    ${PROJECT_SOURCE_DIR}/src/Rendering/Renderers/SubPasses/PBRPAss.inl
)

set(PBR_RENDERERS_SOURCE
    ${PROJECT_SOURCE_DIR}/src/Rendering/Renderers/PBR.cpp
)

set(PBR_RENDERERS_HEADERS
    ${PROJECT_SOURCE_DIR}/src/Rendering/Renderers/IRenderer.h
    ${PROJECT_SOURCE_DIR}/src/Rendering/Renderers/PBR.hpp
)

set(PBRLIB_GLSL
    ${PROJECT_SOURCE_DIR}/src/Rendering/Renderers/Shaders/brdf.h
    ${PROJECT_SOURCE_DIR}/src/Rendering/Renderers/Shaders/CameraData.h
    ${PROJECT_SOURCE_DIR}/src/Rendering/Renderers/Shaders/DirectionLightData.h
    ${PROJECT_SOURCE_DIR}/src/Rendering/Renderers/Shaders/PointLightData.h
    ${PROJECT_SOURCE_DIR}/src/Rendering/Renderers/Shaders/SpotLightData.h
    ${PROJECT_SOURCE_DIR}/src/Rendering/Renderers/Shaders/NumLights.h
    ${PROJECT_SOURCE_DIR}/src/Rendering/Renderers/Shaders/GBuffer.glsl.frag
    ${PROJECT_SOURCE_DIR}/src/Rendering/Renderers/Shaders/GBuffer.glsl.vert
    ${PROJECT_SOURCE_DIR}/src/Rendering/Renderers/Shaders//MaterialData.h
    ${PROJECT_SOURCE_DIR}/src/Rendering/Renderers/Shaders/PBR.glsl.comp
)

set(PBRLIB_SPV
    ${PROJECT_SOURCE_DIR}/src/Rendering/Renderers/SubPasses/spv/GBuffer.glsl.frag.spv.h 
    ${PROJECT_SOURCE_DIR}/src/Rendering/Renderers/SubPasses/spv/GBuffer.glsl.vert.spv.h
    ${PROJECT_SOURCE_DIR}/src/Rendering/Renderers/SubPasses/spv/PBRPass.glsl.comp.spv.h
)

set(PBRLIB_VULKAN_WRAPPER_SOURCE
    ${PROJECT_SOURCE_DIR}/src/Rendering/VulkanWrapper/Buffer.cpp
    ${PROJECT_SOURCE_DIR}/src/Rendering/VulkanWrapper/CommandBuffer.cpp   
    ${PROJECT_SOURCE_DIR}/src/Rendering/VulkanWrapper/CommandPool.cpp
    ${PROJECT_SOURCE_DIR}/src/Rendering/VulkanWrapper/ComputePipeline.cpp
    ${PROJECT_SOURCE_DIR}/src/Rendering/VulkanWrapper/DescriptorPool.cpp
    ${PROJECT_SOURCE_DIR}/src/Rendering/VulkanWrapper/DescriptorSet.cpp
    ${PROJECT_SOURCE_DIR}/src/Rendering/VulkanWrapper/Device.cpp
    ${PROJECT_SOURCE_DIR}/src/Rendering/VulkanWrapper/DeviceMemory.cpp
    ${PROJECT_SOURCE_DIR}/src/Rendering/VulkanWrapper/DeviceQueue.cpp
    ${PROJECT_SOURCE_DIR}/src/Rendering/VulkanWrapper/Framebuffer.cpp
    ${PROJECT_SOURCE_DIR}/src/Rendering/VulkanWrapper/GPUTextureManager.cpp
    ${PROJECT_SOURCE_DIR}/src/Rendering/VulkanWrapper/GraphicsPipeline.cpp
    ${PROJECT_SOURCE_DIR}/src/Rendering/VulkanWrapper/Image.cpp
    ${PROJECT_SOURCE_DIR}/src/Rendering/VulkanWrapper/Instance.cpp
    ${PROJECT_SOURCE_DIR}/src/Rendering/VulkanWrapper/PhysicalDevice.cpp
    ${PROJECT_SOURCE_DIR}/src/Rendering/VulkanWrapper/PipelineLayout.cpp
    ${PROJECT_SOURCE_DIR}/src/Rendering/VulkanWrapper/RenderPass.cpp
    ${PROJECT_SOURCE_DIR}/src/Rendering/VulkanWrapper/Sampler.cpp
    ${PROJECT_SOURCE_DIR}/src/Rendering/VulkanWrapper/ShaderModule.cpp
    ${PROJECT_SOURCE_DIR}/src/Rendering/VulkanWrapper/Surface.cpp
    ${PROJECT_SOURCE_DIR}/src/Rendering/VulkanWrapper/Swapchain.cpp
)

set(PBRLIB_VULKAN_WRAPPER_HEADERS
    ${PROJECT_SOURCE_DIR}/src/Rendering/VulkanWrapper/Buffer.hpp
    ${PROJECT_SOURCE_DIR}/src/Rendering/VulkanWrapper/Buffer.inl 
    ${PROJECT_SOURCE_DIR}/src/Rendering/VulkanWrapper/CommandBuffer.hpp   
    ${PROJECT_SOURCE_DIR}/src/Rendering/VulkanWrapper/CommandPool.hpp
    ${PROJECT_SOURCE_DIR}/src/Rendering/VulkanWrapper/ComputePipeline.hpp
    ${PROJECT_SOURCE_DIR}/src/Rendering/VulkanWrapper/DescriptorPool.hpp
    ${PROJECT_SOURCE_DIR}/src/Rendering/VulkanWrapper/DescriptorSet.hpp
    ${PROJECT_SOURCE_DIR}/src/Rendering/VulkanWrapper/Device.hpp
    ${PROJECT_SOURCE_DIR}/src/Rendering/VulkanWrapper/DeviceMemory.hpp
    ${PROJECT_SOURCE_DIR}/src/Rendering/VulkanWrapper/DeviceQueue.hpp
    ${PROJECT_SOURCE_DIR}/src/Rendering/VulkanWrapper/Framebuffer.hpp
    ${PROJECT_SOURCE_DIR}/src/Rendering/VulkanWrapper/GPUTextureManager.hpp
    ${PROJECT_SOURCE_DIR}/src/Rendering/VulkanWrapper/GraphicsPipeline.hpp
    ${PROJECT_SOURCE_DIR}/src/Rendering/VulkanWrapper/Image.hpp
    ${PROJECT_SOURCE_DIR}/src/Rendering/VulkanWrapper/Image.inl
    ${PROJECT_SOURCE_DIR}/src/Rendering/VulkanWrapper/Instance.hpp
    ${PROJECT_SOURCE_DIR}/src/Rendering/VulkanWrapper/PhysicalDevice.hpp
    ${PROJECT_SOURCE_DIR}/src/Rendering/VulkanWrapper/PipelineLayout.hpp
    ${PROJECT_SOURCE_DIR}/src/Rendering/VulkanWrapper/RenderPass.hpp
    ${PROJECT_SOURCE_DIR}/src/Rendering/VulkanWrapper/Sampler.hpp
    ${PROJECT_SOURCE_DIR}/src/Rendering/VulkanWrapper/ShaderModule.hpp
    ${PROJECT_SOURCE_DIR}/src/Rendering/VulkanWrapper/Surface.hpp
    ${PROJECT_SOURCE_DIR}/src/Rendering/VulkanWrapper/Swapchain.hpp
)

set(PBRLIB_SCENE_GRAPH_SOURCE
    ${PROJECT_SOURCE_DIR}/src/SceneGraph/Component.cpp
    ${PROJECT_SOURCE_DIR}/src/SceneGraph/MeshNode.cpp
    ${PROJECT_SOURCE_DIR}/src/SceneGraph/Scene.cpp
    ${PROJECT_SOURCE_DIR}/src/SceneGraph/SceneView.cpp
    ${PROJECT_SOURCE_DIR}/src/SceneGraph/Script.cpp
)

set(PBRLIB_SCENE_GRAPH_HEADERS
    ${PROJECT_SOURCE_DIR}/src/SceneGraph/Component.hpp
    ${PROJECT_SOURCE_DIR}/src/SceneGraph/MeshNode.hpp
    ${PROJECT_SOURCE_DIR}/src/SceneGraph/Scene.hpp
    ${PROJECT_SOURCE_DIR}/src/SceneGraph/Scene.inl
    ${PROJECT_SOURCE_DIR}/src/SceneGraph/SceneView.hpp
    ${PROJECT_SOURCE_DIR}/src/SceneGraph/Script.hpp
)

set(PBRLIB_UTIL_HEADERS
    ${PROJECT_SOURCE_DIR}/src/Util/enumCast.hpp
)

add_library(PBRLib
    "src/PBRLibResources.hpp"
    "src/PBRLibResources.inl"
    "src/PBRLibResources.cpp"
    ${PBRLIB_MATH_HEADERS}
    ${PBRLIB_MEMORY_SOURCE}
    ${PBRLIB_MEMORY_HEADERS}
    ${PBRLIB_MOVING_SOURCE}
    ${PBRLIB_MOVING_HEADERS}
    ${PBRLIB_WINDOW_SOURCE}
    ${PBRLIB_WINDOW_HEADERS}
    ${PBRLIB_CAMERAS_SOURCE}
    ${PBRLIB_CAMERAS_HEADERS}
    ${PBRLIB_GEOMETRY_SOURCE}
    ${PBRLIB_GEOMETRY_HEADERS}
    ${PBRLIB_LIGHTS_SOURCE}
    ${PBRLIB_LIGHTS_HEADERS}
    ${PBRLIB_MATERIAL_SOURCE}
    ${PBRLIB_MATERIAL_HEADERS}
    ${PBRLIB_SUB_PASSES_SOURCE}
    ${PBRLIB_SUB_PASSES_HEADERS}
    ${PBR_RENDERERS_SOURCE}
    ${PBR_RENDERERS_HEADERS}
    ${PBRLIB_GLSL}
    ${PBRLIB_SPV}
    ${PBRLIB_VULKAN_WRAPPER_SOURCE}
    ${PBRLIB_VULKAN_WRAPPER_HEADERS}
    ${PBRLIB_SCENE_GRAPH_SOURCE}
    ${PBRLIB_SCENE_GRAPH_HEADERS}
    ${PBRLIB_UTIL_HEADERS}
)

source_group("" FILES "src/PBRLibResources.hpp" "src/PBRLibResources.inl" "src/PBRLibResources.cpp")
source_group(math FILES ${PBRLIB_MATH_HEADERS})
source_group(Memory FILES ${PBRLIB_MEMORY_SOURCE} ${PBRLIB_MEMORY_HEADERS})
source_group(Moving FILES ${PBRLIB_MOVING_SOURCE} ${PBRLIB_MOVING_HEADERS})
source_group(Rendering FILES ${PBRLIB_WINDOW_SOURCE} ${PBRLIB_WINDOW_HEADERS})
source_group(Rendering\\Cameras FILES ${PBRLIB_CAMERAS_SOURCE} ${PBRLIB_CAMERAS_HEADERS})
source_group(Rendering\\Geometry FILES ${PBRLIB_GEOMETRY_SOURCE} ${PBRLIB_GEOMETRY_HEADERS})
source_group(Rendering\\Lights FILES ${PBRLIB_LIGHTS_SOURCE} ${PBRLIB_LIGHTS_HEADERS})
source_group(Rendering\\Material FILES ${PBRLIB_MATERIAL_SOURCE} ${PBRLIB_MATERIAL_HEADERS})
source_group(Rendering\\Renderers FILES ${PBR_RENDERERS_SOURCE} ${PBR_RENDERERS_HEADERS})
source_group(Rendering\\Renderers\\Shaders FILES ${PBRLIB_GLSL})
source_group(Rendering\\Renderers\\SubPasses FILES ${PBRLIB_SUB_PASSES_SOURCE} ${PBRLIB_SUB_PASSES_HEADERS})
source_group(Rendering\\Renderers\\SubPasses\\spv FILES ${PBRLIB_SPV})
source_group(Rendering\\VulkanWrapper FILES ${PBRLIB_VULKAN_WRAPPER_SOURCE} ${PBRLIB_VULKAN_WRAPPER_HEADERS})
source_group(SceneGraph FILES ${PBRLIB_SCENE_GRAPH_SOURCE} ${PBRLIB_SCENE_GRAPH_HEADERS})
source_group(Util FILES ${PBRLIB_UTIL_HEADERS})

include_directories(third_party/assimp/include)
include_directories(third_party/SDL/include)
include_directories(third_party/stb)

add_subdirectory(third_party/assimp)
add_subdirectory(third_party/SDL)

if (APPLE) 
    find_package(Vulkan)

    if (NOT Vulkan_FOUND)
        message(FATAL_ERROR 
            "Ошибка при поиске Vulkan SDK.\n"
            "Для корректной сборки проекта, нужно делать следующее:\n"
            "   1) Скачать Vulkan SDK (https://vulkan.lunarg.com/sdk/home);\n"
            "   2) Расположить содержимое в директории /usr//local/Cellar/;\n"
            "   3) Ввести в терминал следующее: \"export VULKAN_SDK=/usr/local/Cellar/vulkansdk/macOS\"."
        )
    endif()
else()
    find_package(Vulkan REQUIRED)
endif()

target_link_libraries(PBRLib PUBLIC
    ${Vulkan_LIBRARIES}
    assimp
    SDL2
    SDL2main
)

target_include_directories(PBRLib PUBLIC 
    ${Vulkan_INCLUDE_DIRS}
)

if (APPLE)
    message(STATUS 
        "При открытии проекта XCode необходимо установить следующие переменные окружения:\n"
        "   1) DYLD_LIBRARY_PATH    ${Vulkan_LIBRARIES}\n"
        "   2) VK_ICD_FILENAMES     /usr/local/Cellar/vulkansdk/macOS/share/vulkan/icd.d/MoltenVK_icd.json\n"
        "   3) VK_LAYER_PATH        /usr/local/Cellar/vulkansdk/macOS/share/vulkan/explicit_layer.d"
    )
endif()

if (PBRLIB_ENABLE_TESTS) 
    add_subdirectory(PBRLibTests)
endif()