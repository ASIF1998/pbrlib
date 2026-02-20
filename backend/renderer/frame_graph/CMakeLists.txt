cmake_minimum_required(VERSION 3.27)

set(PBRLIB_BACKEND_FRAME_GRAPH_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/compound_render_pass.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/frame_graph.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/render_pass.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gbuffer_generator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ssao.cpp
    CACHE INTERNAL ""
)

set(PBRLIB_BACKEND_FRAME_GRAPH_H
    ${CMAKE_CURRENT_SOURCE_DIR}/compound_render_pass.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/frame_graph.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/render_pass.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gbuffer_generator.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ssao.hpp
    CACHE INTERNAL ""
)

source_group("backend\\renderer\\frame_graph" FILES
    "${PBRLIB_BACKEND_FRAME_GRAPH_SRC}"
    "${PBRLIB_BACKEND_FRAME_GRAPH_H}"
)

add_subdirectory(builders)
add_subdirectory(filters)