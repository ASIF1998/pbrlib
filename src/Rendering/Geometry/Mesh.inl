//
//  Mesh.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 29/04/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include "Mesh.hpp"

#include "../../math/vec3.hpp"
#include "../../math/vec2.hpp"

namespace pbrlib
{
    inline Mesh::Mesh(const string_view name) :
        Component(name)
    {}

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
            size_t offset_to_uv = offsetof(VertexAttrib, uv);

            return *reinterpret_cast<Vec2<float>*>(
                ptr_vertex_attrib_buffer->getData() + vertex_attrib_buffer_offset + offset_to_uv + stride * i
            );
        } else if constexpr (Attrib == MeshAttribute::Normal) {
            size_t offset_to_normal = offsetof(VertexAttrib, normal);

            return *reinterpret_cast<Vec3<float>*>(
                ptr_vertex_attrib_buffer->getData() + vertex_attrib_buffer_offset + offset_to_normal + stride * i
            );
        } else if constexpr (Attrib == MeshAttribute::Tangent) {
            size_t offset_to_tangent = offsetof(VertexAttrib, tangent);

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
            size_t offset_to_uv = offsetof(VertexAttrib, uv);

            return *reinterpret_cast<Vec2<float>*>(
                ptr_vertex_attrib_buffer->getData() + vertex_attrib_buffer_offset + offset_to_uv + stride * i
            );
        } else if constexpr (Attrib == MeshAttribute::Normal) {
            size_t offset_to_normal = offsetof(VertexAttrib, normal);

            return *reinterpret_cast<Vec3<float>*>(
                ptr_vertex_attrib_buffer->getData() + vertex_attrib_buffer_offset + offset_to_normal + stride * i
            );
        } else if constexpr (Attrib == MeshAttribute::Tangent) {
            size_t offset_to_tangent = offsetof(VertexAttrib, tangent);

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

    inline type_index Mesh::getType() const
    {
        return ComponentUtil::getTypeIndex<Mesh>();
    }

    inline PtrMesh Mesh::make(const string_view name)
    {
        return make_shared<Mesh>(name);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inline void Mesh::Builder::setAABB(const AABB& bbox)
    {
        _bbox = bbox;
    }

    inline void Mesh::Builder::setName(const string_view name)
    {
        _name = name;
    }
   
    inline Buffer::Builder<Mesh::VertexAttrib>& Mesh::Builder::getVertexBufferBuilder() noexcept
    {
        return _vertex_buffer_builder;
    }

    inline const Buffer::Builder<Mesh::VertexAttrib>& Mesh::Builder::getVertexBufferBuilder() const noexcept
    {
        return _vertex_buffer_builder;
    }

    Buffer::Builder<uint32_t>& Mesh::Builder::getIndexBufferBuilder() noexcept
    {
        return _index_buffer_builder;
    }

    inline const Buffer::Builder<uint32_t>& Mesh::Builder::getIndexBufferBuilder() const noexcept
    {
        return _index_buffer_builder;
    }

    inline Mesh Mesh::Builder::build() const
    {
        Mesh mesh (_name);

        mesh.aabb                       = _bbox;
        mesh.index_buffer_offset        = 0;
        mesh.ptr_index_buffer           = _index_buffer_builder.buildPtr();
        mesh.ptr_material               = nullptr;
        mesh.ptr_vertex_attrib_buffer   = _vertex_buffer_builder.buildPtr();
        mesh.num_vertices               = static_cast<uint32_t>(mesh.ptr_vertex_attrib_buffer->getSize() / sizeof(VertexAttrib));
        mesh.num_indices                = static_cast<uint32_t>(mesh.ptr_index_buffer->getSize() / sizeof(uint32_t));

        return mesh;
    }

    inline PtrMesh Mesh::Builder::buildPtr()  const
    {
        PtrMesh ptr_mesh = Mesh::make(_name);

        ptr_mesh->aabb                      = _bbox;
        ptr_mesh->index_buffer_offset       = 0;
        ptr_mesh->ptr_index_buffer          = _index_buffer_builder.buildPtr();
        ptr_mesh->ptr_material              = nullptr;
        ptr_mesh->ptr_vertex_attrib_buffer  = _vertex_buffer_builder.buildPtr();
        ptr_mesh->num_vertices              = static_cast<uint32_t>(ptr_mesh->ptr_vertex_attrib_buffer->getSize() / sizeof(VertexAttrib));
        ptr_mesh->num_indices               = static_cast<uint32_t>(ptr_mesh->ptr_index_buffer->getSize() / sizeof(uint32_t));

        return ptr_mesh;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template<typename VertexAttribAllocatorType, typename IndexAllocatorType>
    inline void Mesh::BuilderWithData<VertexAttribAllocatorType, IndexAllocatorType>::setAABB(const AABB& bbox)
    {
        _bbox = bbox;
    }

    template<typename VertexAttribAllocatorType, typename IndexAllocatorType>
    inline void Mesh::BuilderWithData<VertexAttribAllocatorType, IndexAllocatorType>::setName(const string_view name)
    {
        _name = name;
    }

    template<typename VertexAttribAllocatorType, typename IndexAllocatorType>
    inline Buffer::BuilderWithData<Mesh::VertexAttrib, VertexAttribAllocatorType>& 
        Mesh::BuilderWithData<VertexAttribAllocatorType, IndexAllocatorType>::getVertexBufferBuilder() noexcept
    {
        return _vertex_buffer_builder;
    }

    template<typename VertexAttribAllocatorType, typename IndexAllocatorType>
    inline const Buffer::BuilderWithData<Mesh::VertexAttrib, VertexAttribAllocatorType>& 
        Mesh::BuilderWithData<VertexAttribAllocatorType, IndexAllocatorType>::getVertexBufferBuilder() const noexcept 
    {
        return _vertex_buffer_builder;
    }

    template<typename VertexAttribAllocatorType, typename IndexAllocatorType>
    inline Buffer::BuilderWithData<uint32_t, IndexAllocatorType>& 
        Mesh::BuilderWithData<VertexAttribAllocatorType, IndexAllocatorType>::getIndexBufferBuilder() noexcept 
    {
        return _index_buffer_builder;
    }

    template<typename VertexAttribAllocatorType, typename IndexAllocatorType>
    inline const Buffer::BuilderWithData<uint32_t, IndexAllocatorType>& 
        Mesh::BuilderWithData<VertexAttribAllocatorType, IndexAllocatorType>::getIndexBufferBuilder() const noexcept 
    {
        return _index_buffer_builder;
    }

    template<typename VertexAttribAllocatorType, typename IndexAllocatorType>
    inline Mesh Mesh::BuilderWithData<VertexAttribAllocatorType, IndexAllocatorType>::build() const
    {
        Mesh mesh (_name);

        mesh.aabb                       = _bbox;
        mesh.index_buffer_offset        = 0;
        mesh.ptr_index_buffer           = _index_buffer_builder.buildPtr();
        mesh.ptr_material               = nullptr;
        mesh.ptr_vertex_attrib_buffer   = _vertex_buffer_builder.buildPtr();
        mesh.num_vertices               = mesh.ptr_vertex_attrib_buffer->getSize() / sizeof(VertexAttrib);
        mesh.num_indices                = mesh.ptr_index_buffer->getSize() / sizeof(uint32_t);

        return mesh;
    }

    template<typename VertexAttribAllocatorType, typename IndexAllocatorType>
    inline PtrMesh Mesh::BuilderWithData<VertexAttribAllocatorType, IndexAllocatorType>::buildPtr() const
    {
        PtrMesh ptr_mesh = Mesh::make(_name);

        ptr_mesh->aabb                      = _bbox;
        ptr_mesh->index_buffer_offset       = 0;
        ptr_mesh->ptr_index_buffer          = _index_buffer_builder.buildPtr();
        ptr_mesh->ptr_material              = nullptr;
        ptr_mesh->ptr_vertex_attrib_buffer  = _vertex_buffer_builder.buildPtr();
        ptr_mesh->num_vertices              = ptr_mesh->ptr_vertex_attrib_buffer->getSize() / sizeof(VertexAttrib);
        ptr_mesh->num_indices               = ptr_mesh->ptr_index_buffer->getSize() / sizeof(uint32_t);
        
        return ptr_mesh;
    }
}
