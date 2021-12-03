set(PBRLIB_MATH_TESTS_SOURCE
    ${PROJECT_SOURCE_DIR}/PBRLibTests/MathTests/Matrix2x2Tests.cpp
    ${PROJECT_SOURCE_DIR}/PBRLibTests/MathTests/Matrix3x3Tests.cpp
    ${PROJECT_SOURCE_DIR}/PBRLibTests/MathTests/Matrix4x4Tests.cpp
    ${PROJECT_SOURCE_DIR}/PBRLibTests/MathTests/QuaternionTests.cpp
    ${PROJECT_SOURCE_DIR}/PBRLibTests/MathTests/Vec2Tests.cpp
    ${PROJECT_SOURCE_DIR}/PBRLibTests/MathTests/Vec3Tests.cpp
    ${PROJECT_SOURCE_DIR}/PBRLibTests/MathTests/Vec4Tests.cpp
)

set(PBRLIB_MEMORY_TESTS_SOURCE
    ${PROJECT_SOURCE_DIR}/PBRLibTests/MemoryTests/MemoryArenaTests.cpp
    ${PROJECT_SOURCE_DIR}/PBRLibTests/MemoryTests/MemoryBlockTests.cpp
    ${PROJECT_SOURCE_DIR}/PBRLibTests/MemoryTests/STLAlignedAllocatorTests.cpp
    ${PROJECT_SOURCE_DIR}/PBRLibTests/MemoryTests/STLArenaLinearAllocatorTests.cpp
)

set(PBRLIB_MOVING_TESTS_SOURCE
    ${PROJECT_SOURCE_DIR}/PBRLibTests/MovingTests/TransformTests.cpp
)

set(PBRLIB_CAMERAS_TESTS_SOURCE
    ${PROJECT_SOURCE_DIR}/PBRLibTests/RenderingTests/CamerasTests/PerspectiveCameraTests.cpp
)

set(PBRLIB_GEOMETRY_TESTS_SOURCE
    ${PROJECT_SOURCE_DIR}/PBRLibTests/RenderingTests/GeometryTests/AABBTests.cpp
    ${PROJECT_SOURCE_DIR}/PBRLibTests/RenderingTests/GeometryTests/MeshTests.cpp
)

set(PBRLIB_LIGHTS_TESTS_SOURCE
    ${PROJECT_SOURCE_DIR}/PBRLibTests/RenderingTests/LightsTests/DirectionLightTests.cpp
    ${PROJECT_SOURCE_DIR}/PBRLibTests/RenderingTests/LightsTests/PointLightTests.cpp
    ${PROJECT_SOURCE_DIR}/PBRLibTests/RenderingTests/LightsTests/SpotLightTests.cpp
)

set(PBRLIB_SCENE_GRAPH_TESTS_SOURCE
    ${PROJECT_SOURCE_DIR}/PBRLibTests/SceneGraphTests/MeshNodeTests.cpp
    ${PROJECT_SOURCE_DIR}/PBRLibTests/SceneGraphTests/SceneNodeTests.cpp
    ${PROJECT_SOURCE_DIR}/PBRLibTests/SceneGraphTests/SceneTests.cpp
)

if (WIN32)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

add_subdirectory(${PROJECT_SOURCE_DIR}/third_party/googletest third_party/googletest)
enable_testing()
include_directories(../third_party/googletest/googletest/include ${gtest_SOURCE_DIR})

add_executable(PBRLibTests
    "../src/PBRLibResources.hpp"
    "../src/PBRLibResources.inl"
    "../src/PBRLibResources.cpp"
    ${PBRLIB_MATH_HEADERS}
    ${PBRLIB_MEMORY_SOURCE}
    ${PBRLIB_MEMORY_HEADERS}
    ${PBRLIB_MOVING_SOURCE}
    ${PBRLIB_MOVING_HEADERS}
    ${PBRLIB_WINDOW_SOURCE}
    ${PBRLIB_WINDOW_HEADERS}
    ${PBRLIB_CAMERAS_SOURCE}
    ${PBRLIB_CAMERAS_HEADERS}
    ${PBRLIB_GEOMETRY_SOURCE}
    ${PBRLIB_GEOMETRY_HEADERS}
    ${PBRLIB_LIGHTS_SOURCE}
    ${PBRLIB_LIGHTS_HEADERS}
    ${PBRLIB_MATERIAL_SOURCE}
    ${PBRLIB_MATERIAL_HEADERS}
    ${PBRLIB_SUB_PASSES_SOURCE}
    ${PBRLIB_SUB_PASSES_HEADERS}
    ${PBR_RENDERERS_SOURCE}
    ${PBR_RENDERERS_HEADERS}
    ${PBRLIB_VULKAN_WRAPPER_SOURCE}
    ${PBRLIB_VULKAN_WRAPPER_HEADERS}
    ${PBRLIB_SCENE_GRAPH_SOURCE}
    ${PBRLIB_SCENE_GRAPH_HEADERS}

    ${PBRLIB_MATH_TESTS_SOURCE}   
    ${PBRLIB_MEMORY_TESTS_SOURCE}
    ${PBRLIB_MOVING_TESTS_SOURCE}
    ${PBRLIB_CAMERAS_TESTS_SOURCE}
    ${PBRLIB_GEOMETRY_TESTS_SOURCE}
    ${PBRLIB_LIGHTS_TESTS_SOURCE}
    ${PBRLIB_SCENE_GRAPH_TESTS_SOURCE}

    utils.hpp
    pbrlib_tests_run.cpp
)

target_link_libraries(PBRLibTests PUBLIC 
    ${Vulkan_LIBRARIES}
    assimp
    SDL2
    SDL2main
    gtest
)

target_include_directories(PBRLibTests PUBLIC 
    ${Vulkan_INCLUDE_DIRS}
)

source_group("" FILES pbrlib_tests_run.cpp utils.hpp)
source_group(MathTests FILES ${PBRLIB_MATH_TESTS_SOURCE})
source_group(MemoryTests FILES ${PBRLIB_MEMORY_TESTS_SOURCE})
source_group(MovingTests FILES ${PBRLIB_MOVING_TESTS_SOURCE})
source_group(RenderingTests\\CamerasTests FILES ${PBRLIB_CAMERAS_TESTS_SOURCE})
source_group(RenderingTests\\GeometryTests FILES ${PBRLIB_GEOMETRY_TESTS_SOURCE})
source_group(RenderingTests\\LightsTests FILES ${PBRLIB_LIGHTS_TESTS_SOURCE})
source_group(SceneGraphTests FILES ${PBRLIB_SCENE_GRAPH_TESTS_SOURCE})