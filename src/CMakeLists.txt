cmake_minimum_required(VERSION 3.27)

set(PBRLIB_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/components.cpp   
    ${CMAKE_CURRENT_SOURCE_DIR}/event_system.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/window.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/camera.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/engine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/exceptions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/transforms.cpp
    CACHE INTERNAL ""
)

set(PBRLIB_INPUT_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/input/input_stay.cpp
    CACHE INTERNAL ""
)

set(PBRLIB_MATH_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/math/aabb.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/math/casts.cpp
    CACHE INTERNAL ""
)

set(PBRLIB_SCENE_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/scene/scene.cpp
    CACHE INTERNAL ""
)

source_group("src" FILES "${PBRLIB_SRC}")
source_group("src\\input" FILES "${PBRLIB_INPUT_SRC}")
source_group("src\\math" FILES "${PBRLIB_MATH_SRC}")
source_group("src\\scene" FILES "${PBRLIB_SCENE_SRC}")