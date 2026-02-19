cmake_minimum_required(VERSION 3.27)

set(PBRLIB_TESTS_RENDERER_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/frame_graph_tests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/fxaa_tests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/camera_tests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/framegraph_resources_getter.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/gbuffer_generator_tests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ssao_tests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/canvas_tests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/render_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vulkan_device_tests.cpp
    CACHE INTERNAL ""
    )
    
set(PBRLIB_TESTS_RENDERER_H
    ${CMAKE_CURRENT_SOURCE_DIR}/render_test.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/framegraph_resources_getter.hpp
    CACHE INTERNAL ""
)