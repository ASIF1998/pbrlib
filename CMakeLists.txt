cmake_minimum_required(VERSION 3.27)

if (APPLE)
    set(CMAKE_OSX_ARCHITECTURES arm64)
endif()

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build static libraries" FORCE)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(pbrlib)

#####################################################
#   Project options (cmake -DPBRLIB_*=ON/OFF)
#####################################################
option(PBRLIB_ENABLE_TESTS "Build pbrlib tests (pbrlib-tests)" OFF)
option(PBRLIB_BUILD_SAMPLES "Build sample applications" OFF)
option(PBRLIB_SETUP_CI "Enable CI-only defines" OFF)

set(PBRLIB_PATH_TO_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

#####################################################
# Generate compile_commands.json for other tools
#####################################################
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#####################################################
#   Setup core.hpp
#####################################################
set(PBRLIB_ENGINE_VERSION_MAJOR 0)
set(PBRLIB_ENGINE_VERSION_MINOR 0)
set(PBRLIB_ENGINE_VERSION_PATCH 1)

set(PBRLIB_VULKAN_VERSION_MAJOR 1)
set(PBRLIB_VULKAN_VERSION_MINOR 3)
set(PBRLIB_VULKAN_VERSION_PATCH 0)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

configure_file(backend/utils/private/info.hpp.in ../backend/utils/private/info.hpp)

#####################################################
#   Create pbrlib library
#####################################################
add_subdirectory(backend)
add_subdirectory(include/pbrlib)
add_subdirectory(src)

add_library(pbrlib STATIC 
    ${PBRLIB_BACKEND_SRC} 
    ${PBRLIB_BACKEND_H}
    ${PBRLIB_BACKEND_INPUT_H}
    ${PBRLIB_BACKEND_LOGGER_H}
    ${PBRLIB_BACKEND_RENDERER_SRC}
    ${PBRLIB_BACKEND_RENDERER_H}
    ${PBRLIB_BACKEND_FRAME_GRAPH_SRC}
    ${PBRLIB_BACKEND_FRAME_GRAPH_H}
    ${PBRLIB_BACKEND_FRAME_GRAPH_BUILDERS_SRC}
    ${PBRLIB_BACKEND_FRAME_GRAPH_BUILDERS_H}
    ${PBRLIB_BACKEND_FILTERS_SRC}
    ${PBRLIB_BACKEND_FILTERS_H}
    ${PBRLIB_BACKEND_VULKAN_SRC}
    ${PBRLIB_BACKEND_VULKAN_H}
    ${PBRLIB_BACKEND_SCENE_SRC}
    ${PBRLIB_BACKEND_SCENE_H}
    ${PBRLIB_BACKEND_UTILS_SRC}
    ${PBRLIB_BACKEND_UTILS_H}

    ${PBRLIB_H}
    ${PBRLIB_SRC}
    ${PBRLIB_INPUT_H}
    ${PBRLIB_INPUT_SRC}
    ${PBRLIB_SCENE_H}
    ${PBRLIB_SCENE_SRC}
    ${PBRLIB_MATH_H}
    ${PBRLIB_MATH_SRC}
    ${PBRLIB_UTILS_H}
    ${PBRLIB_LOGGER_H}

    ${PBRLIB_SHADERS}
)

#####################################################
#   Add project defenitions
#####################################################
if (PBRLIB_BUILD_SAMPLES)
    target_compile_definitions(pbrlib PRIVATE PBRLIB_ENABLE_PROFILING)
    target_compile_definitions(pbrlib PRIVATE PBRLIB_ENABLE_DEVELOPER_MODE)
endif()

if (PBRLIB_SETUP_CI)
    target_compile_definitions(pbrlib PRIVATE PBRLIB_RUN_FROM_CI)
endif()

if (APPLE)
    target_compile_definitions(pbrlib PRIVATE PBRLIB_OS_APPLE)
elseif(WIN32)
    target_compile_definitions(pbrlib PRIVATE PBRLIB_OS_WINDOWS)
else()
    target_compile_definitions(pbrlib PRIVATE PBRLIB_OS_LINUX)
endif()

#####################################################
#   Source grouping
#####################################################
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} 
    PREFIX ""
    FILES
    ${PBRLIB_BACKEND_SRC} 
    ${PBRLIB_BACKEND_H}
    ${PBRLIB_BACKEND_INPUT_H}
    ${PBRLIB_BACKEND_LOGGER_H}
    ${PBRLIB_BACKEND_RENDERER_SRC}
    ${PBRLIB_BACKEND_RENDERER_H}
    ${PBRLIB_BACKEND_FRAME_GRAPH_SRC}
    ${PBRLIB_BACKEND_FRAME_GRAPH_H}
    ${PBRLIB_BACKEND_FRAME_GRAPH_BUILDERS_SRC}
    ${PBRLIB_BACKEND_FRAME_GRAPH_BUILDERS_H}
    ${PBRLIB_BACKEND_FILTERS_SRC}
    ${PBRLIB_BACKEND_FILTERS_H}
    ${PBRLIB_BACKEND_VULKAN_SRC}
    ${PBRLIB_BACKEND_VULKAN_H}
    ${PBRLIB_BACKEND_SCENE_SRC}
    ${PBRLIB_BACKEND_SCENE_H}
    ${PBRLIB_BACKEND_UTILS_SRC}
    ${PBRLIB_BACKEND_UTILS_H}

    ${PBRLIB_H}
    ${PBRLIB_SRC}
    ${PBRLIB_INPUT_H}
    ${PBRLIB_INPUT_SRC}
    ${PBRLIB_SCENE_H}
    ${PBRLIB_SCENE_SRC}
    ${PBRLIB_MATH_H}
    ${PBRLIB_MATH_SRC}
    ${PBRLIB_UTILS_H}
    ${PBRLIB_LOGGER_H}

    ${PBRLIB_SHADERS}
)

target_include_directories(pbrlib PUBLIC include)
target_include_directories(pbrlib PUBLIC "")

if (MSVC)
    target_compile_options(pbrlib PRIVATE /W3 /WX /MP)
endif()

if (PBRLIB_SETUP_CI AND MSVC)
    target_compile_options(pbrlib PRIVATE /showIncludes)
endif()

#####################################################
#   Vulkan
#####################################################
if (APPLE)
    add_subdirectory(third_party/MoltenVK)
else()
    find_package(Vulkan REQUIRED)
    set(Vulkan_SDK_LIBS ${Vulkan_INCLUDE_DIRS}/../Lib)
endif()

#####################################################
#   assimp
#####################################################
set(ASSIMP_BUILD_TESTS   OFF CACHE BOOL "" FORCE)
add_subdirectory(third_party/assimp)
target_link_libraries(pbrlib PUBLIC assimp::assimp)

#####################################################
#   SDL
#####################################################
add_subdirectory(third_party/SDL)

#####################################################
#   STB
#####################################################
target_include_directories(pbrlib PUBLIC third_party/stb)

#####################################################
#   glslang
#####################################################
add_subdirectory(third_party/glslang)

#####################################################
#   spdlog
#####################################################
add_subdirectory(third_party/spdlog)

#####################################################
#   VulkanMemoryAllocator
#####################################################
add_subdirectory(third_party/VulkanMemoryAllocator)

#####################################################
#   entt
#####################################################
add_subdirectory(third_party/entt)

#####################################################
#   cpptrace
#####################################################
add_subdirectory(third_party/cpptrace)

#####################################################
#   tracy
#####################################################
if (PBRLIB_BUILD_SAMPLES)
    option(TRACY_ENABLE "Build Tracy profiler client into the project" ON)
    option(TRACY_ON_DEMAND "Connect to Tracy profiler only when server is running" ON)
    add_subdirectory(third_party/tracy)

    target_link_libraries(pbrlib PUBLIC Tracy::TracyClient)
endif()

#####################################################
#   add libs
#####################################################
target_link_libraries(pbrlib PUBLIC
    glslang
    OSDependent
    MachineIndependent
    GenericCodeGen
    SPIRV
    SPIRV-Headers
    glslang-default-resource-limits
    assimp
    EnTT::EnTT
    spdlog::spdlog_header_only
    VulkanMemoryAllocator
    SDL3::SDL3
    cpptrace::cpptrace
)

if (APPLE)
    target_link_libraries(pbrlib PUBLIC MoltenVK)
else()
    target_include_directories(pbrlib PUBLIC
        ${Vulkan_INCLUDE_DIRS}
    )
    target_link_libraries(pbrlib PUBLIC ${Vulkan_LIBRARIES})
endif()

#####################################################
#   tests
#####################################################
if (PBRLIB_ENABLE_TESTS)
    add_subdirectory(pbrlib-tests)
endif()

#####################################################
#   samples
#####################################################
if (PBRLIB_BUILD_SAMPLES)
    add_subdirectory(samples)
endif()