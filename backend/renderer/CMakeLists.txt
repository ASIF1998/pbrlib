cmake_minimum_required(VERSION 3.27)

set(PBRLIB_BACKEND_RENDERER_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/canvas.cpp
    CACHE INTERNAL ""
)

set(PBRLIB_BACKEND_RENDERER_H
    ${CMAKE_CURRENT_SOURCE_DIR}/canvas.hpp
    CACHE INTERNAL ""
)

add_subdirectory(frame_graph)
add_subdirectory(vulkan)