cmake_minimum_required(VERSION 3.27)

set(PBRLIB_MATH_H
    ${CMAKE_CURRENT_SOURCE_DIR}/casts.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/casts.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/lerp.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/matrix3x3.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/matrix3x3.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/matrix4x4.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/matrix4x4.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/vec3.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vec3.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/vec4.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vec4.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/aabb.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/aabb.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/matrix2x2.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/matrix2x2.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/quat.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/quat.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/vec2.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vec2.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/concepts.hpp
    CACHE INTERNAL ""
)

source_group("include\\pbrlib\\math" FILES
    "${PBRLIB_MATH_H}"
)
