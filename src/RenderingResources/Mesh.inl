//
//  Mesh.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 29/04/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include "Mesh.hpp"

#include "../math/vec3.hpp"
#include "../math/vec2.hpp"

namespace pbrlib
{
    inline void Mesh::mapVertexAttribBuffer() const
    {
        ptr_vertex_attrib_buffer->map();
    }

    inline void Mesh::unmapVertexAttribBuffer() const
    {
        ptr_vertex_attrib_buffer->unmap();
    }

    inline void Mesh::mapIndexBuffer() const
    {
        ptr_index_buffer->map();
    }

    inline void Mesh::unmapIndexBuffer() const
    {
        ptr_index_buffer->unmap();
    }

    template<MeshAttribute Attrib>
    inline auto& Mesh::getAttribute(size_t i)
    {
        assert(ptr_vertex_attrib_buffer->getMapStatus() == MapStatus::MAPPED);

        if constexpr (Attrib == MeshAttribute::Position) {
            return *reinterpret_cast<Vec3<float>*>(
                ptr_vertex_attrib_buffer->getData() + vertex_attrib_buffer_offset + stride * i
            );
        } else if constexpr (Attrib == MeshAttribute::UV) {
            size_t offset_to_uv = sizeof(float) * 3;

            return *reinterpret_cast<Vec2<float>*>(
                ptr_vertex_attrib_buffer->getData() + vertex_attrib_buffer_offset + offset_to_uv + stride * i
            );
        } else if constexpr (Attrib == MeshAttribute::Normal) {
            size_t offset_to_normal = sizeof(float) * 5;

            return *reinterpret_cast<Vec3<float>*>(
                ptr_vertex_attrib_buffer->getData() + vertex_attrib_buffer_offset + offset_to_normal + stride * i
            );
        } else if constexpr (Attrib == MeshAttribute::Tangent) {
            size_t offset_to_tangent = sizeof(float) * 8;

            return *reinterpret_cast<Vec3<float>*>(
                ptr_vertex_attrib_buffer->getData() + vertex_attrib_buffer_offset + offset_to_tangent + stride * i
            );
        }
    }

    template<MeshAttribute Attrib>
    inline const auto& Mesh::getAttribute(size_t i) const
    {
        assert(ptr_vertex_attrib_buffer->getMapStatus() == MapStatus::MAPPED);

        if constexpr (Attrib == MeshAttribute::Position) {
            return *reinterpret_cast<Vec3<float>*>(
                ptr_vertex_attrib_buffer->getData() + vertex_attrib_buffer_offset + stride * i
            );
        } else if constexpr (Attrib == MeshAttribute::UV) {
            size_t offset_to_uv = sizeof(float) * 3;

            return *reinterpret_cast<Vec2<float>*>(
                ptr_vertex_attrib_buffer->getData() + vertex_attrib_buffer_offset + offset_to_uv + stride * i
            );
        } else if constexpr (Attrib == MeshAttribute::Normal) {
            size_t offset_to_normal = sizeof(float) * 5;

            return *reinterpret_cast<Vec3<float>*>(
                ptr_vertex_attrib_buffer->getData() + vertex_attrib_buffer_offset + offset_to_normal + stride * i
            );
        } else if constexpr (Attrib == MeshAttribute::Tangent) {
            size_t offset_to_tangent = sizeof(float) * 8;

            return *reinterpret_cast<Vec3<float>*>(
                ptr_vertex_attrib_buffer->getData() + vertex_attrib_buffer_offset + offset_to_tangent + stride * i
            );
        }
    }

    inline uint32_t Mesh::getIndex(size_t i) const
    {
        assert(ptr_index_buffer->getMapStatus() == MapStatus::MAPPED);
        assert(i < num_indices);
        return *(reinterpret_cast<uint32_t*>(ptr_index_buffer->getData() + index_buffer_offset) + i);
    }

    inline void Mesh::setIndex(size_t i, uint32_t val)
    {
        assert(ptr_index_buffer->getMapStatus() == MapStatus::MAPPED);
        assert(i < num_indices);

        *(reinterpret_cast<uint32_t*>(ptr_index_buffer->getData() + index_buffer_offset) + i) = val;
    }
}
