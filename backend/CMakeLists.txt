cmake_minimum_required(VERSION 3.27)

set(PBRLIB_BACKEND_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/profiling.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/exceptions.cpp
    CACHE INTERNAL ""
)

set(PBRLIB_BACKEND_H
    ${CMAKE_CURRENT_SOURCE_DIR}/events.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/profiling.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/components.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/exceptions.hpp
    CACHE INTERNAL ""
)

set(PBRLIB_BACKEND_INPUT_H
    ${CMAKE_CURRENT_SOURCE_DIR}/input/sdl_keycode_map.hpp
    CACHE INTERNAL ""
)

set(PBRLIB_BACKEND_LOGGER_H
    ${CMAKE_CURRENT_SOURCE_DIR}/logger/console.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/logger/logger.hpp
    CACHE INTERNAL ""
)

add_subdirectory(renderer)
add_subdirectory(scene)
add_subdirectory(utils)
add_subdirectory(shaders)
