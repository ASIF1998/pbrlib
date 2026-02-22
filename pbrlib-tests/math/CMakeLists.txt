cmake_minimum_required(VERSION 3.27)

set(PBRLIB_TESTS_MATH_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/mat2x2_tests.cpp  
    ${CMAKE_CURRENT_SOURCE_DIR}/mat4x4_tests.cpp  
    ${CMAKE_CURRENT_SOURCE_DIR}/vec3_tests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/aabb_tests.cpp    
    ${CMAKE_CURRENT_SOURCE_DIR}/mat3x3_tests.cpp  
    ${CMAKE_CURRENT_SOURCE_DIR}/quat_tests.cpp    
    ${CMAKE_CURRENT_SOURCE_DIR}/vec2_tests.cpp    
    ${CMAKE_CURRENT_SOURCE_DIR}/vec4_tests.cpp
    CACHE INTERNAL ""
    )
    
set(PBRLIB_TESTS_MATH_H
    ${CMAKE_CURRENT_SOURCE_DIR}/testing_types.hpp 
    CACHE INTERNAL ""
)