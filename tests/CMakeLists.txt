cmake_minimum_required(VERSION 3.27)

#####################################################
#   Create PBRLibTests executable
#####################################################
add_subdirectory(math)
add_subdirectory(renderer)

set(PBRLIB_TESTS_SCENE_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/scene/content_tests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/scene/scene_item_tests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/scene/scene_tests.cpp
)

set(PBRLIB_TESTS_IMAGE_COMPARISON_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/image_comparison/image_comparison.cpp
)

set(PBRLIB_TESTS_IMAGE_COMPARISON_H
    ${CMAKE_CURRENT_SOURCE_DIR}/image_comparison/image_comparison.hpp
)

add_executable(pbrlib-tests 
    ${PBRLIB_TESTS_MATH_SRC} 
    ${PBRLIB_TESTS_MATH_H}
    ${PBRLIB_TESTS_RENDERER_SRC}
    ${PBRLIB_TESTS_RENDERER_H}
    ${PBRLIB_TESTS_SCENE_SRC}
    ${PBRLIB_TESTS_IMAGE_COMPARISON_SRC}
    ${PBRLIB_TESTS_IMAGE_COMPARISON_H}
    pbrlib_tests_run.cpp
)

#####################################################
#   Source grouping
#####################################################
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} 
    PREFIX ""
    FILES
    ${PBRLIB_TESTS_MATH_SRC}
    ${PBRLIB_TESTS_MATH_H}
    ${PBRLIB_TESTS_RENDERER_SRC}
    ${PBRLIB_TESTS_RENDERER_H}
    ${PBRLIB_TESTS_SCENE_SRC}
    ${PBRLIB_TESTS_IMAGE_COMPARISON_SRC}
    ${PBRLIB_TESTS_IMAGE_COMPARISON_H}
    pbrlib_tests_run.cpp
)

target_include_directories(pbrlib-tests PRIVATE ../backend/include)

#####################################################
#   link gtest
#####################################################
if (WIN32)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

add_subdirectory(${PROJECT_SOURCE_DIR}/third_party/googletest third_party/googletest)
enable_testing()
include_directories(../third_party/googletest/googletest/include ${gtest_SOURCE_DIR})

target_link_libraries(pbrlib-tests PRIVATE pbrlib gtest)
