cmake_minimum_required(VERSION 3.27)

set(PBRLIB_BACKEND_UTILS_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/memory.cpp
    CACHE INTERNAL ""
)

set(PBRLIB_BACKEND_UTILS_H
    ${CMAKE_CURRENT_SOURCE_DIR}/enum_cast.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/generate_color.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/memory.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/align_size.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/generate_color.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/paths.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/versions.hpp
    CACHE INTERNAL ""
)

source_group("backend\\utils" FILES
    "${PBRLIB_BACKEND_UTILS_SRC}"
    "${PBRLIB_BACKEND_UTILS_H}"
)