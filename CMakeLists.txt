cmake_minimum_required(VERSION 3.27)

if (APPLE)
    set(CMAKE_OSX_ARCHITECTURES arm64)
endif()

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build static libraries" FORCE)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQURED ON)

project(pbrlib)

option(PBRLIB_ENABLE_TESTS "" OFF)
option(PBRLIB_TEST_CONSOLE "" OFF)
option(PBRLIB_SETUP_CI "" OFF)

set(PRLIB_PATH_TO_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

if (PBRLIB_TEST_CONSOLE)
    add_compile_definitions(PBRLIB_ENABLE_PROPFILING)
    add_compile_definitions(PBRLIB_ENABLE_DEVELOPER_MODE)
endif()

if (PBRLIB_SETUP_CI)
    add_compile_definitions(PBRLIB_RUN_FROM_CI)
endif()

if (APPLE)
    add_compile_definitions(PBRLIB_OS_APPLE)
elseif(WIN32)
    add_compile_definitions(PBRLIB_OS_WINDOWS)
else()
    add_compile_definitions(PBRLIB_OS_LINUX)
endif()

#####################################################
#   Setup core.hpp
#####################################################
set(PRLIB_ENGINE_VERSION_MAJOR 0)
set(PRLIB_ENGINE_VERSION_MINOR 0)
set(PRLIB_ENGINE_VERSION_PATCH 1)

set(PRLIB_VULKAN_VERSION_MAJOR 1)
set(PRLIB_VULKAN_VERSION_MINOR 3)
set(PRLIB_VULKAN_VERSION_PATCH 0)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

configure_file(backend/utils/private/info.hpp.in ../backend/utils/private/info.hpp)

#####################################################
#   Create pbrlib library
#####################################################
file(GLOB_RECURSE PBRLIB_SOURCES CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE PBRLIB_BACKEND_SOURCES CONFIGURE_DEPENDS backend/*.cpp)

add_library(pbrlib STATIC ${PBRLIB_SOURCES} ${PBRLIB_BACKEND_SOURCES})

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
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)

include_directories(third_party/assimp/include)
add_subdirectory(third_party/assimp)

#####################################################
#   SDL
#####################################################
add_subdirectory(third_party/SDL)

#####################################################
#   STB
#####################################################
include_directories(third_party/stb)

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
if (PBRLIB_TEST_CONSOLE)
    option(TRACY_ENABLE "" ON)
    option(TRACY_ON_DEMAND "" ON)
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
#   test console
#####################################################
if (PBRLIB_TEST_CONSOLE)
    add_executable(test-console "test-console/main.cpp")
    target_link_libraries(test-console PRIVATE pbrlib)
endif()
