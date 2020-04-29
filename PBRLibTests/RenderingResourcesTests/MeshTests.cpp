//
//  MeshTests.cpp
//  PBRLibTests
//
//  Created by Асиф Мамедов on 29/04/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <vector>

#include <gtest/gtest.h>

#include "../../src/RenderingResources/Mesh.hpp"

#include "../../src/math/vec3.hpp"
#include "../../src/math/vec2.hpp"

#include "../../src/Rendering/VulkanWrapper/Instance.hpp"
#include "../../src/Rendering/VulkanWrapper/PhysicalDevice.hpp"
#include "../../src/Rendering/VulkanWrapper/Device.hpp"

#include "../../src/Rendering/VulkanWrapper/Buffer.hpp"

using namespace testing;
using namespace pbrlib;
using namespace pbrlib::math;
using namespace std;

struct Vertex
{
    Vec3<float> pos;
    Vec2<float> uv;
    Vec3<float> normal;
    Vec3<float> tangent;
};

TEST(RenderingResourceMesh, GettersAndSetters)
{
    PtrInstance ptr_instance = Instance::make(
        "RenderingResourceMesh::GettersAndSetters", 
        VK_MAKE_VERSION(0, 0, 1)
    );

    PhysicalDevice gpu = ptr_instance->getPhysicalDevice(
        VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU    |
        VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU  |
        VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU
    );

    float queue_priority = 1.0f;

    vector<VkDeviceQueueCreateInfo> device_queue_create_info;

    device_queue_create_info.push_back(
        VkDeviceQueueCreateInfo {
            .sType              = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .queueFamilyIndex   = 0,
            .queueCount         = 1,
            .pQueuePriorities   = &queue_priority
        }
    );

    PtrDevice ptr_device = gpu.makeDevice(device_queue_create_info);

    constexpr size_t num_vertex = 200;
    constexpr size_t num_indices = 152;

    constexpr Vec3<float>   pnt_r   (0.00334f, 0.5454f, 0.4567f);
    constexpr Vec2<float>   uv_r    (0.6534f, 0.67003f);
    constexpr uint32_t      index_r (1332);

    vector<Vertex>      vertices;
    vector<uint32_t>    indices;

    vertices.reserve(num_vertex);
    indices.reserve(num_indices);

    for(size_t i{0}; i < num_vertex; i++) {
        vertices.push_back(Vertex{
            .pos = Vec3<float> (
                static_cast<float>(i), 
                static_cast<float>(i), 
                static_cast<float>(i)
            ),

            .uv = Vec2<float> (
                static_cast<float>(i), 
                static_cast<float>(i)
            ),

            .normal = Vec3<float> (
                static_cast<float>(i), 
                static_cast<float>(i), 
                static_cast<float>(i)
            ),

            .tangent = Vec3<float> (
                static_cast<float>(i), 
                static_cast<float>(i), 
                static_cast<float>(i)
            ),
        });
    }

    for (uint32_t i{0}; i < num_indices; i++) {
        indices.push_back(i * 2);
    }

    shared_ptr<Buffer> ptr_vertex_buffer = make_shared<Buffer>(
        ptr_device,
        num_vertex * sizeof(Vertex),
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        1,
        0
    );

    shared_ptr<Buffer> ptr_index_buffer = make_shared<Buffer>(
        ptr_device,
        num_indices * sizeof(uint32_t),
        VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
        1,
        0
    );

    ptr_vertex_buffer->map();
    ptr_vertex_buffer->setData(vertices);
    ptr_vertex_buffer->unmap();

    ptr_index_buffer->map();
    ptr_index_buffer->setData(indices);
    ptr_index_buffer->unmap();

    Mesh mesh1;
    Mesh mesh2;

    mesh1.ptr_vertex_attrib_buffer       = ptr_vertex_buffer;
    mesh1.ptr_index_buffer               = ptr_index_buffer;
    mesh1.num_vertices                   = static_cast<uint32_t>(num_vertex) / 2;
    mesh1.num_indices                    = static_cast<uint32_t>(num_indices) /2;
    mesh1.index_buffer_offset            = 0;
    mesh1.vertex_attrib_buffer_offset    = 0;

    mesh2.ptr_vertex_attrib_buffer       = ptr_vertex_buffer;
    mesh2.ptr_index_buffer               = ptr_index_buffer;
    mesh2.num_vertices                   = static_cast<uint32_t>(num_vertex) / 2;
    mesh2.num_indices                    = static_cast<uint32_t>(num_indices) /2;
    mesh2.index_buffer_offset            = num_indices / 2 * sizeof(uint32_t);
    mesh2.vertex_attrib_buffer_offset    = num_vertex / 2 * sizeof(Vertex);
    
    mesh1.mapVertexAttribBuffer();

    for (size_t i{0}; i < num_vertex / 2; i++) {
        EXPECT_EQ(vertices[i].pos, mesh1.getAttribute<MeshAttribute::Position>(i));
        EXPECT_EQ(vertices[i].uv, mesh1.getAttribute<MeshAttribute::UV>(i));
        EXPECT_EQ(vertices[i].normal, mesh1.getAttribute<MeshAttribute::Normal>(i));
        EXPECT_EQ(vertices[i].tangent, mesh1.getAttribute<MeshAttribute::Tangent>(i));
    }

    mesh1.unmapVertexAttribBuffer();

    mesh1.mapIndexBuffer();

    for (size_t i{0}; i < num_indices / 2; i++) {
        EXPECT_EQ(indices[i], mesh1.getIndex(i));
    }

    mesh1.unmapIndexBuffer();

    mesh2.mapVertexAttribBuffer();

    for (size_t i{num_vertex / 2}; i < num_vertex; i++) {
        EXPECT_EQ(vertices[i].pos, mesh2.getAttribute<MeshAttribute::Position>(i - (num_vertex / 2)));
        EXPECT_EQ(vertices[i].uv, mesh2.getAttribute<MeshAttribute::UV>(i - (num_vertex / 2)));
        EXPECT_EQ(vertices[i].normal, mesh2.getAttribute<MeshAttribute::Normal>(i - (num_vertex / 2)));
        EXPECT_EQ(vertices[i].tangent, mesh2.getAttribute<MeshAttribute::Tangent>(i - (num_vertex / 2)));
    }

    mesh2.unmapVertexAttribBuffer();

    mesh2.mapIndexBuffer();

    for (size_t i{num_indices / 2}; i < num_indices; i++) {
        EXPECT_EQ(indices[i], mesh2.getIndex(i - (num_indices / 2)));
    }

    mesh2.unmapIndexBuffer();

    mesh1.mapVertexAttribBuffer();
    mesh2.mapVertexAttribBuffer();

    mesh1.getAttribute<MeshAttribute::Position>(13) = pnt_r;
    mesh1.getAttribute<MeshAttribute::UV>(13)       = uv_r;
    mesh1.getAttribute<MeshAttribute::Normal>(13)   = pnt_r;
    mesh1.getAttribute<MeshAttribute::Tangent>(13)  = pnt_r;

    mesh2.getAttribute<MeshAttribute::Position>(13) = pnt_r;
    mesh2.getAttribute<MeshAttribute::UV>(13)       = uv_r;
    mesh2.getAttribute<MeshAttribute::Normal>(13)   = pnt_r;
    mesh2.getAttribute<MeshAttribute::Tangent>(13)  = pnt_r;

    EXPECT_EQ(mesh1.getAttribute<MeshAttribute::Position>(13), pnt_r);
    EXPECT_EQ(mesh1.getAttribute<MeshAttribute::UV>(13), uv_r);
    EXPECT_EQ(mesh1.getAttribute<MeshAttribute::Normal>(13), pnt_r);
    EXPECT_EQ(mesh1.getAttribute<MeshAttribute::Tangent>(13), pnt_r);

    EXPECT_EQ(mesh2.getAttribute<MeshAttribute::Position>(13), pnt_r);
    EXPECT_EQ(mesh2.getAttribute<MeshAttribute::UV>(13), uv_r);
    EXPECT_EQ(mesh2.getAttribute<MeshAttribute::Normal>(13), pnt_r);
    EXPECT_EQ(mesh2.getAttribute<MeshAttribute::Tangent>(13), pnt_r);

    mesh2.unmapVertexAttribBuffer();
    mesh1.unmapVertexAttribBuffer();

    mesh1.mapIndexBuffer();
    mesh2.mapIndexBuffer();

    mesh1.setIndex(23, index_r);
    mesh2.setIndex(23, index_r);

    EXPECT_EQ(index_r, mesh1.getIndex(23));
    EXPECT_EQ(index_r, mesh2.getIndex(23));

    mesh2.unmapIndexBuffer();
    mesh1.unmapIndexBuffer();
}
