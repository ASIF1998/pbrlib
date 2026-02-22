cmake_minimum_required(VERSION 3.27)

set(PBRLIB_SHADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/fxaa.glsl.comp
    ${CMAKE_CURRENT_SOURCE_DIR}/gpu_cpu_constants.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bilateral_blur.glsl.comp
    ${CMAKE_CURRENT_SOURCE_DIR}/filter.glsl
    ${CMAKE_CURRENT_SOURCE_DIR}/generation.glsl
    ${CMAKE_CURRENT_SOURCE_DIR}/math.glsl

    ${CMAKE_CURRENT_SOURCE_DIR}/gbuffer_generator/exports.glsl
    ${CMAKE_CURRENT_SOURCE_DIR}/gbuffer_generator/gbuffer_generator.glsl.vert
    ${CMAKE_CURRENT_SOURCE_DIR}/gbuffer_generator/gbuffer_generator.glsl.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/gbuffer_generator/packing.glsl

    ${CMAKE_CURRENT_SOURCE_DIR}/material_manager/exports.glsl
    ${CMAKE_CURRENT_SOURCE_DIR}/mesh_manager/exports.glsl

    ${CMAKE_CURRENT_SOURCE_DIR}/ssao/exports.glsl
    ${CMAKE_CURRENT_SOURCE_DIR}/ssao/ssao.glsl.comp

    CACHE INTERNAL ""
)