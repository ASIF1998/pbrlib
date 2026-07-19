cmake_minimum_required(VERSION 3.27)

set(PBRLIB_BACKEND_VULKAN_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/compute_pipeline.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pipeline_layout.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/surface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vma_usage.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/buffer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/graphics_pipeline.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/render_pass.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/unique_handler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/device.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/command_buffer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/image.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shader_compiler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/framebuffer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sync.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pixel_format.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/descriptor_binder.cpp
    CACHE INTERNAL ""
)

set(PBRLIB_BACKEND_VULKAN_H
    ${CMAKE_CURRENT_SOURCE_DIR}/framebuffer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/compute_pipeline.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gpu_marker_colors.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pipeline_layout.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/surface.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/buffer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/config.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/check.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/graphics_pipeline.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/render_pass.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/unique_handler.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/device.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/unique_handler.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/command_buffer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/image.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shader_compiler.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/utils.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sync.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pixel_format.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/descriptor_binder.hpp
    CACHE INTERNAL ""
)
