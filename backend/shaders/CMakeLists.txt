cmake_minimum_required(VERSION 3.27)

set(PBRLIB_SHADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/fxaa.comp.glsl
    ${CMAKE_CURRENT_SOURCE_DIR}/gpu_cpu_constants.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bilateral_blur.comp.glsl
    ${CMAKE_CURRENT_SOURCE_DIR}/filter.glsl
    ${CMAKE_CURRENT_SOURCE_DIR}/generation.glsl
    ${CMAKE_CURRENT_SOURCE_DIR}/math.glsl

    ${CMAKE_CURRENT_SOURCE_DIR}/gbuffer_generator/exports.glsl
    ${CMAKE_CURRENT_SOURCE_DIR}/gbuffer_generator/gbuffer_generator.vert.glsl
    ${CMAKE_CURRENT_SOURCE_DIR}/gbuffer_generator/gbuffer_generator.frag.glsl
    ${CMAKE_CURRENT_SOURCE_DIR}/gbuffer_generator/packing.glsl

    ${CMAKE_CURRENT_SOURCE_DIR}/material_manager/exports.glsl
    ${CMAKE_CURRENT_SOURCE_DIR}/mesh_manager/exports.glsl

    ${CMAKE_CURRENT_SOURCE_DIR}/ssao/exports.glsl
    ${CMAKE_CURRENT_SOURCE_DIR}/ssao/ssao.comp.glsl

    CACHE INTERNAL ""
)