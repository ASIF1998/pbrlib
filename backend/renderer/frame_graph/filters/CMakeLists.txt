cmake_minimum_required(VERSION 3.27)

set(PBRLIB_BACKEND_FILTERS_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/bilateral_blur.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/filter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/fxaa.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/physically_based_lighting.cpp
    CACHE INTERNAL ""
)

set(PBRLIB_BACKEND_FILTERS_H
    ${CMAKE_CURRENT_SOURCE_DIR}/bilateral_blur.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/filter.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/fxaa.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/physically_based_lighting.hpp
    CACHE INTERNAL ""
)
