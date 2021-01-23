set(PBRLIB_MATH_TESTS_SOURCE
    MathTests/Matrix2x2Tests.cpp
    MathTests/Matrix3x3Tests.cpp
    MathTests/Matrix4x4Tests.cpp
    MathTests/QuaternionTests.cpp
    MathTests/Vec2Tests.cpp
    MathTests/Vec3Tests.cpp
    MathTests/Vec4Tests.cpp
)

set(PBRLIB_MEMORY_TESTS_SOURCE
    MemoryTests/MemoryArenaTests.cpp
    MemoryTests/MemoryBlockTests.cpp
    MemoryTests/STLAlignedAllocatorTests.cpp
    MemoryTests/STLArenaLinearAllocatorTests.cpp
)

set(PBRLIB_MOVING_TESTS_SOURCE
    MovingTests/TransformTests.cpp
)

set(PBRLIB_CAMERAS_TESTS_SOURCE
    RenderingTests/CamerasTests/PerspectiveCameraTests.cpp
)

set(PBRLIB_GEOMETRY_TESTS_SOURCE
    RenderingTests/GeometryTests/AABBTests.cpp
    RenderingTests/GeometryTests/MeshTests.cpp
)

set(PBRLIB_LIGHTS_TESTS_SOURCE
    RenderingTests/LightsTests/DirectionLightTests.cpp
    RenderingTests/LightsTests/PointLightTests.cpp
    RenderingTests/LightsTests/SpotLightTests.cpp
)

set(PBRLIB_SCENE_GRAPH_TESTS_SOURCE
    SceneGraphTests/MeshNodeTests.cpp
    SceneGraphTests/SceneNodeTests.cpp
    SceneGraphTests/SceneTests.cpp
)

add_executable(PBRLibTests
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
    pbrlib_tests_run.cpp
)

find_package(GTest REQUIRED)

target_link_libraries(PBRLibTests PUBLIC 
    ${Vulkan_LIBRARIES}
    ${SDL2_LIBRARIES} 
    ${assimp_LIBRARIES}
    ${GTEST_LIBRARIES}
)

target_include_directories(PBRLibTests PUBLIC 
    ${Vulkan_INCLUDE_DIRS}
    ${SDL2_INCLUDE_DIRS} 
    ${assimp_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/${STB_INCLUDE_DIR}
    ${GTEST_INCLUDE_DIRS}
)

source_group(MathTests FILES ${PBRLIB_MATH_TESTS_SOURCE})
source_group(MemoryTests FILES ${PBRLIB_MEMORY_TESTS_SOURCE})
source_group(MovingTests FILES ${PBRLIB_MOVING_TESTS_SOURCE})
source_group(RenderingTests\\CamerasTests FILES ${PBRLIB_CAMERAS_TESTS_SOURCE})
source_group(RenderingTests\\GeometryTests FILES ${PBRLIB_GEOMETRY_TESTS_SOURCE})
source_group(RenderingTests\\LightsTests FILES ${PBRLIB_LIGHTS_TESTS_SOURCE})
source_group(SceneGraphTests FILES ${PBRLIB_SCENE_GRAPH_TESTS_SOURCE})
source_group("" FILES pbrlib_tests_run.cpp)