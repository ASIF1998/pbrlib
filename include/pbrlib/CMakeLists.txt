cmake_minimum_required(VERSION 3.27)

set(PBRLIB_H
    ${CMAKE_CURRENT_SOURCE_DIR}/components.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/engine.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/event_system.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/transforms.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/window.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/camera.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/config.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/event_system.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/exceptions.hpp
    CACHE INTERNAL ""
)

set(PBRLIB_INPUT_H
    ${CMAKE_CURRENT_SOURCE_DIR}/input/input_stay.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/input/key_code.hpp
    CACHE INTERNAL ""
)

set(PBRLIB_LOGGER_H
    ${CMAKE_CURRENT_SOURCE_DIR}/logger/logger.hpp
    CACHE INTERNAL ""
)

set(PBRLIB_SCENE_H
    ${CMAKE_CURRENT_SOURCE_DIR}/scene/scene.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/scene/scene.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/scene/visitor.hpp
    CACHE INTERNAL ""
)

set(PBRLIB_UTILS_H
    ${CMAKE_CURRENT_SOURCE_DIR}/utils/combine_hash.hpp
    CACHE INTERNAL ""
)

source_group("include\\pbrlib" FILES "${PBRLIB_H}")
source_group("include\\pbrlib\\input" FILES "${PBRLIB_INPUT_H}")
source_group("include\\pbrlib\\logger" FILES "${PBRLIB_LOGGER_H}")
source_group("include\\pbrlib\\scene" FILES "${PBRLIB_SCENE_H}")
source_group("include\\pbrlib\\utils" FILES "${PBRLIB_UTILS_H}")

add_subdirectory(math)