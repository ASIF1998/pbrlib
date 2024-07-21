//
//  MeshTests.cpp
//  PBRLibTests
//
//  Created by Асиф Мамедов on 29/04/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#if 0

#include <vector>

#include "../../utils.hpp"

#include <pbrlib/PBRLibResources.hpp>

#include <pbrlib/Rendering/Geometry/Mesh.hpp>
#include <pbrlib/Rendering/Geometry/MeshManager.hpp>

#include <pbrlib/math/vec2.hpp>
#include <pbrlib/math/vec3.hpp>

#include <pbrlib/Rendering/VulkanWrapper/Buffer.hpp>

using namespace pbrlib;
using namespace pbrlib::math;
using namespace std;

TEST(RenderingGeometryMesh, GettersAndSetters)
{
    srand(static_cast<unsigned>(time(nullptr)));
    
    auto ptr_instance = Instance::make(
        "RenderingResourceMesh::GettersAndSetters", 
        VK_MAKE_VERSION(0, 0, 1)
    );

    auto gpu = ptr_instance->getPhysicalDevice(
        VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU    |
        VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU  |
        VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU
    );

    float queue_priority = 1.0f;

    vector<VkDeviceQueueCreateInfo> device_queue_create_info;

    VkDeviceQueueCreateInfo device_queue_ci = { };
    device_queue_ci.sType               = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    device_queue_ci.queueFamilyIndex    = 0;
    device_queue_ci.queueCount          = 1;
    device_queue_ci.pQueuePriorities    = &queue_priority;

    device_queue_create_info.push_back(device_queue_ci);

    auto ptr_device = gpu.makeDevice(ptr_instance.get(), device_queue_create_info);

    constexpr size_t num_vertex = 200;
    constexpr size_t num_indices = 152;

    constexpr Vec3<float>   pnt_r   (0.00334f, 0.54540f, 0.4567f);
    constexpr Vec2<float>   uv_r    (0.65340f, 0.67003f);
    constexpr uint32_t      index_r (1332);

    vector<Mesh::VertexAttrib>  vertices;
    vector<uint32_t>            indices;

    vertices.reserve(num_vertex);
    indices.reserve(num_indices);

    for(size_t i{0}; i < num_vertex; i++) {
        Mesh::VertexAttrib vert_attrib = { };

        vert_attrib.position = Vec3<float> (
            static_cast<float>(rand()),
            static_cast<float>(rand()),
            static_cast<float>(rand())
        );

        vert_attrib.uv = Vec2<float> (
            static_cast<float>(rand()),
            static_cast<float>(rand())
        );

        vert_attrib.normal = Vec3<float> (
            static_cast<float>(rand()),
            static_cast<float>(rand()),
            static_cast<float>(rand())
        );

        vert_attrib.tangent = Vec3<float> (
            static_cast<float>(rand()),
            static_cast<float>(rand()),
            static_cast<float>(rand())
        );
    }

    for (uint32_t i{0}; i < num_indices; i++) {
        indices.push_back(i * 2);
    }

    Buffer::BuilderWithData<Mesh::VertexAttrib> build_vertex_buffer;
    Buffer::BuilderWithData<uint32_t>           build_index_buffer;

    build_vertex_buffer.setUsage(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
    build_vertex_buffer.setMemoryTypeIndex(gpu.memory.getMemoryType(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));
    build_vertex_buffer.setDevice(ptr_device.get());
    build_vertex_buffer.setData(vertices);
    build_vertex_buffer.addQueueFamily(0);
    build_vertex_buffer.setSize(num_vertex);

    build_index_buffer.setUsage(VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
    build_index_buffer.setMemoryTypeIndex(gpu.memory.getMemoryType(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));
    build_index_buffer.setDevice(ptr_device.get());
    build_index_buffer.setData(indices);
    build_index_buffer.addQueueFamily(0);
    build_index_buffer.setSize(num_indices);

    std::shared_ptr<Buffer> ptr_vertex_buffer = build_vertex_buffer.buildPtr();
    std::shared_ptr<Buffer> ptr_index_buffer  = build_index_buffer.buildPtr();

    Mesh mesh1;
    Mesh mesh2;
    
    mesh1.setVertexBuffer(ptr_vertex_buffer);
    mesh1.setIndexBuffer(ptr_index_buffer);
    mesh1.setNumVertices(num_vertex / 2);
    mesh1.setNumIndices(num_indices / 2);
    mesh1.setIndexBufferOffset(0);
    mesh1.setVertexBufferOffset(0);
    
    mesh2.setVertexBuffer(ptr_vertex_buffer);
    mesh2.setIndexBuffer(ptr_index_buffer);
    mesh2.setNumVertices(num_vertex / 2);
    mesh2.setNumIndices(num_indices / 2);
    mesh2.setIndexBufferOffset(num_indices / 2);
    mesh2.setVertexBufferOffset(num_vertex / 2);
    
    mesh1.mapVertexAttribBuffer();

    for (size_t i{0}; i < num_vertex / 2; i++) {
        pbrlib::testing::utils::equality(vertices[i].position, mesh1.getAttribute<MeshAttribute::Position>(i));
        pbrlib::testing::utils::equality(vertices[i].uv, mesh1.getAttribute<MeshAttribute::UV>(i));
        pbrlib::testing::utils::equality(vertices[i].normal, mesh1.getAttribute<MeshAttribute::Normal>(i));
        pbrlib::testing::utils::equality(vertices[i].tangent, mesh1.getAttribute<MeshAttribute::Tangent>(i));
    }

    mesh1.unmapVertexAttribBuffer();

    mesh1.mapIndexBuffer();

    for (size_t i{0}; i < num_indices / 2; i++) {
        pbrlib::testing::utils::equality(indices[i], mesh1.getIndex(i));
    }

    mesh1.unmapIndexBuffer();

    mesh2.mapVertexAttribBuffer();

    for (size_t i{mesh1.getVertexBufferOffset()}; i < mesh1.getNumVertices(); i++) {
        pbrlib::testing::utils::equality(vertices[i].position, mesh2.getAttribute<MeshAttribute::Position>(i - (num_vertex / 2)));
        pbrlib::testing::utils::equality(vertices[i].uv, mesh2.getAttribute<MeshAttribute::UV>(i - (num_vertex / 2)));
        pbrlib::testing::utils::equality(vertices[i].normal, mesh2.getAttribute<MeshAttribute::Normal>(i - (num_vertex / 2)));
        pbrlib::testing::utils::equality(vertices[i].tangent, mesh2.getAttribute<MeshAttribute::Tangent>(i - (num_vertex / 2)));
    }

    mesh2.unmapVertexAttribBuffer();

    mesh2.mapIndexBuffer();

    for (size_t i{num_indices / 2}; i < num_indices; i++) {
        pbrlib::testing::utils::equality(indices[i], mesh2.getIndex(i - (num_indices / 2)));
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

    pbrlib::testing::utils::equality(mesh1.getAttribute<MeshAttribute::Position>(13), pnt_r);
    pbrlib::testing::utils::equality(mesh1.getAttribute<MeshAttribute::UV>(13), uv_r);
    pbrlib::testing::utils::equality(mesh1.getAttribute<MeshAttribute::Normal>(13), pnt_r);
    pbrlib::testing::utils::equality(mesh1.getAttribute<MeshAttribute::Tangent>(13), pnt_r);

    pbrlib::testing::utils::equality(mesh2.getAttribute<MeshAttribute::Position>(13), pnt_r);
    pbrlib::testing::utils::equality(mesh2.getAttribute<MeshAttribute::UV>(13), uv_r);
    pbrlib::testing::utils::equality(mesh2.getAttribute<MeshAttribute::Normal>(13), pnt_r);
    pbrlib::testing::utils::equality(mesh2.getAttribute<MeshAttribute::Tangent>(13), pnt_r);

    mesh2.unmapVertexAttribBuffer();
    mesh1.unmapVertexAttribBuffer();

    mesh1.mapIndexBuffer();
    mesh2.mapIndexBuffer();

    mesh1.setIndex(23, index_r);
    mesh2.setIndex(23, index_r);

    pbrlib::testing::utils::equality(index_r, mesh1.getIndex(23));
    pbrlib::testing::utils::equality(index_r, mesh2.getIndex(23));

    mesh2.unmapIndexBuffer();
    mesh1.unmapIndexBuffer();
}
#endif
