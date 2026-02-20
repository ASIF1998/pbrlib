cmake_minimum_required(VERSION 3.27)

set(PBRLIB_BACKEND_SCENE_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/assimp_importer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/material_manager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mesh_manager.cpp
    CACHE INTERNAL ""
)

set(PBRLIB_BACKEND_SCENE_H
    ${CMAKE_CURRENT_SOURCE_DIR}/assimp_importer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/material_manager.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mesh_manager.hpp
    CACHE INTERNAL ""
)