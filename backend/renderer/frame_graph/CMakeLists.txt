cmake_minimum_required(VERSION 3.27)

set(PBRLIB_BACKEND_FRAME_GRAPH_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/compound_render_pass.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/frame_graph.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/render_pass.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gbuffer_generator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ssao.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/physically_based_lighting.cpp
    CACHE INTERNAL ""
)

set(PBRLIB_BACKEND_FRAME_GRAPH_H
    ${CMAKE_CURRENT_SOURCE_DIR}/compound_render_pass.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/frame_graph.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/render_pass.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gbuffer_generator.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ssao.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/physically_based_lighting.hpp
    CACHE INTERNAL ""
)

set(PBRLIB_BACKEND_FRAME_GRAPH_BUILDERS_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/builders/gbuffer_generator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/builders/ssao.cpp
    CACHE INTERNAL ""
)

set(PBRLIB_BACKEND_FRAME_GRAPH_BUILDERS_H
    ${CMAKE_CURRENT_SOURCE_DIR}/builders/gbuffer_generator.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/builders/ssao.hpp
    CACHE INTERNAL ""
)

add_subdirectory(filters)
