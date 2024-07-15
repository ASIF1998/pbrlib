cmake_minimum_required(VERSION 3.27)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQURED ON)

project(PBRLib)

option(PBRLIB_ENABLE_TESTS "" OFF)
option(PBRLIB_TEST_CONSOLE "" OFF)

set(PRLIB_PATH_TO_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
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

configure_file(include/pbrlib/core.hpp.in ../include/pbrlib/core.hpp)

#####################################################
#   Create PBRLib library
#####################################################
file(GLOB_RECURSE PBRLIB_SOURCES CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE PBRLIB_HEADERS CONFIGURE_DEPENDS include/*.hpp)

add_library(PBRLib STATIC ${PBRLIB_SOURCES} ${PBRLIB_HEADERS})
target_include_directories(PBRLib PUBLIC include)

#####################################################
#   Vulkan
#####################################################
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

#####################################################
#   assimp
#####################################################
include_directories(third_party/assimp/include)
add_subdirectory(third_party/assimp)

#####################################################
#   SDL
#####################################################
include_directories(third_party/SDL/include)
add_subdirectory(third_party/SDL)

#####################################################
#   STB
#####################################################
include_directories(third_party/stb)

#####################################################
#   glslang
#####################################################
set(glslang_ROOT ${CMAKE_CURRENT_SOURCE_DIR}"/third_party/glslang")

set(glslang_INCLUDE_DIRS ${glslang_ROOT}/include)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(glslang_LIBS_DIR ${glslang_ROOT}/Windows/Debug)

    set(glslang_LIBS_NAME 
            GenericCodeGend.lib
            glslang-default-resource-limitsd.lib
            glslangd.lib
            MachineIndependentd.lib
            OSDependentd.lib
            SPIRV-Tools-optd.lib
            SPIRV-Toolsd.lib
            SPIRVd.lib
            SPVRemapperd.lib
    )
else()
    set(glslang_LIBS_DIR ${glslang_ROOT}/Windows/Release)

    set(glslang_LIBS_NAME 
            GenericCodeGen.lib
            glslang-default-resource-limits.lib
            glslang.lib
            MachineIndependent.lib
            OSDependent.lib
            SPIRV-Tools-opt.lib
            SPIRV-Tools.lib
            SPIRV.lib
            SPVRemapper.lib
    )
endif()

foreach(library ${glslang_LIBS_NAME})
    set(glslang_LIBS ${glslang_LIBS} ${glslang_LIBS_DIR}/${library})
endforeach()

#####################################################
#   spdlog
#####################################################
add_subdirectory(third_party/spdlog)

#####################################################
#   VulkanMemoryAllocator
#####################################################
add_subdirectory(third_party/VulkanMemoryAllocator)

#####################################################
#   add libs
#####################################################
target_include_directories(PBRLib PUBLIC 
    ${Vulkan_INCLUDE_DIRS}
    ${glslang_INCLUDE_DIRS}
)

target_link_libraries(PBRLib PUBLIC
    ${Vulkan_LIBRARIES}
    ${glslang_LIBS}
    assimp
    SDL3::SDL3
    spdlog::spdlog_header_only
    VulkanMemoryAllocator
)

if (APPLE)
    message(STATUS 
        "При открытии проекта XCode необходимо установить следующие переменные окружения:\n"
        "   1) DYLD_LIBRARY_PATH    ${Vulkan_LIBRARIES}\n"
        "   2) VK_ICD_FILENAMES     /usr/local/Cellar/vulkansdk/macOS/share/vulkan/icd.d/MoltenVK_icd.json\n"
        "   3) VK_LAYER_PATH        /usr/local/Cellar/vulkansdk/macOS/share/vulkan/explicit_layer.d"
    )
endif()

#####################################################
#   tests
#####################################################
if (PBRLIB_ENABLE_TESTS) 
    add_subdirectory(PBRLibTests)
endif()

#####################################################
#   test console
#####################################################
if (PBRLIB_TEST_CONSOLE)
    add_executable(test_console "test_console/main.cpp")
    target_link_libraries(test_console PRIVATE PBRLib)
endif()