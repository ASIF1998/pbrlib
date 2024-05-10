cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQURED ON)

project(PBRLib)

option(PBRLIB_ENABLE_TESTS "" OFF)
option(PBRLIB_TEST_CONSOLE "" OFF)

set(PRLIB_ENGINE_VERSION_MAJOR 0)
set(PRLIB_ENGINE_VERSION_MINOR 0)
set(PRLIB_ENGINE_VERSION_PATCH 1)

set(PRLIB_VULKAN_VERSION_MAJOR 1)
set(PRLIB_VULKAN_VERSION_MINOR 3)
set(PRLIB_VULKAN_VERSION_PATCH 0)

configure_file(include/pbrlib/core.hpp.in ../include/pbrlib/core.hpp)

file(GLOB_RECURSE PBRLIB_SOURCES CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE PBRLIB_HEADERS CONFIGURE_DEPENDS include/*.hpp)

include_directories(include)    

add_library(PBRLib STATIC ${PBRLIB_SOURCES} ${PBRLIB_HEADERS})

source_group(TREE  ${CMAKE_CURRENT_SOURCE_DIR}
    PREFIX ""
    FILES ${PBRLIB_SOURCES} ${PBRLIB_HEADERS}
)           

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
    SDL3::SDL3
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

if (PBRLIB_TEST_CONSOLE)
    add_executable(test_console "test_console/main.cpp")
    target_link_libraries(test_console PRIVATE PBRLib)
endif()