//
//  Mesh.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 29/04/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <stdexcept>
#include <algorithm>

#include "Mesh.hpp"

#include <pbrlib/math/vec3.hpp>
#include <pbrlib/math/vec2.hpp>

namespace pbrlib
{
    inline Mesh::Mesh(const string_view name) :
        Component(name)
    {}

    inline void Mesh::mapVertexAttribBuffer() const
    {
        _ptr_vertex_attrib_buffer->getDeviceMemory()->map();
    }

    inline void Mesh::unmapVertexAttribBuffer() const
    {
        _ptr_vertex_attrib_buffer->getDeviceMemory()->unmap();
    }

    inline void Mesh::mapIndexBuffer() const
    {
        _ptr_index_buffer->getDeviceMemory()->map();
    }

    inline void Mesh::unmapIndexBuffer() const
    {
        _ptr_index_buffer->getDeviceMemory()->unmap();
    }

    template<MeshAttribute Attrib>
    inline auto& Mesh::getAttribute(size_t i)
    {
        assert(_ptr_vertex_attrib_buffer->getDeviceMemory()->getMapStatus() == MapStatus::MAPPED);

        if constexpr (Attrib == MeshAttribute::Position) {
            return *reinterpret_cast<Vec3<float>*>(
                _ptr_vertex_attrib_buffer->getDeviceMemory()->getData() + _vertex_attrib_buffer_offset + _stride * i
            );
        } else if constexpr (Attrib == MeshAttribute::UV) {
            size_t offset_to_uv = offsetof(VertexAttrib, uv);

            return *reinterpret_cast<Vec2<float>*>(
                _ptr_vertex_attrib_buffer->getDeviceMemory()->getData() + _vertex_attrib_buffer_offset + offset_to_uv + _stride * i
            );
        } else if constexpr (Attrib == MeshAttribute::Normal) {
            size_t offset_to_normal = offsetof(VertexAttrib, normal);

            return *reinterpret_cast<Vec3<float>*>(
                _ptr_vertex_attrib_buffer->getDeviceMemory()->getData() + _vertex_attrib_buffer_offset + offset_to_normal + _stride * i
            );
        } else if constexpr (Attrib == MeshAttribute::Tangent) {
            size_t offset_to_tangent = offsetof(VertexAttrib, tangent);

            return *reinterpret_cast<Vec3<float>*>(
                _ptr_vertex_attrib_buffer->getDeviceMemory()->getData() + _vertex_attrib_buffer_offset + offset_to_tangent + _stride * i
            );
        }
    }

    template<MeshAttribute Attrib>
    inline const auto& Mesh::getAttribute(size_t i) const
    {
        assert(_ptr_vertex_attrib_buffer->getDeviceMemory()->getMapStatus() == MapStatus::MAPPED);

        if constexpr (Attrib == MeshAttribute::Position) {
            return *reinterpret_cast<Vec3<float>*>(
                _ptr_vertex_attrib_buffer->getDeviceMemory()->getData() + _vertex_attrib_buffer_offset + _stride * i
            );
        } else if constexpr (Attrib == MeshAttribute::UV) {
            size_t offset_to_uv = offsetof(VertexAttrib, uv);

            return *reinterpret_cast<Vec2<float>*>(
                _ptr_vertex_attrib_buffer->getDeviceMemory()->getData() + _vertex_attrib_buffer_offset + offset_to_uv + _stride * i
            );
        } else if constexpr (Attrib == MeshAttribute::Normal) {
            size_t offset_to_normal = offsetof(VertexAttrib, normal);

            return *reinterpret_cast<Vec3<float>*>(
                _ptr_vertex_attrib_buffer->getDeviceMemory()->getData() + _vertex_attrib_buffer_offset + offset_to_normal + _stride * i
            );
        } else if constexpr (Attrib == MeshAttribute::Tangent) {
            size_t offset_to_tangent = offsetof(VertexAttrib, tangent);

            return *reinterpret_cast<Vec3<float>*>(
                _ptr_vertex_attrib_buffer->getDeviceMemory()->getData() + _vertex_attrib_buffer_offset + offset_to_tangent + _stride * i
            );
        }
    }

    inline uint32_t Mesh::getIndex(size_t i) const
    {
        assert(_ptr_index_buffer->getDeviceMemory()->getMapStatus() == MapStatus::MAPPED);
        assert(i < _num_indices);
        return *(reinterpret_cast<uint32_t*>(_ptr_index_buffer->getDeviceMemory()->getData() + _index_buffer_offset) + i);
    }

    inline PtrBuffer& Mesh::getIndexBuffer() noexcept
    {
        return _ptr_index_buffer;
    }

    inline const PtrBuffer& Mesh::getIndexBuffer() const noexcept
    {
        return _ptr_index_buffer;
    }

    inline PtrBuffer& Mesh::getVertexBuffer() noexcept
    {
        return _ptr_vertex_attrib_buffer;
    }

    inline const PtrBuffer& Mesh::getVertexBuffer() const noexcept
    {
        return _ptr_vertex_attrib_buffer;
    }

    inline uint32_t Mesh::getNumIndices() const noexcept
    {
        return _num_indices;
    }

    inline uint32_t Mesh::getNumVertices() const noexcept
    {
        return _num_vertices;
    }

    inline uint32_t Mesh::getIndexBufferOffset() const noexcept
    {
        return _index_buffer_offset;
    }

    inline uint32_t Mesh::getVertexBufferOffset() const noexcept
    {
        return _vertex_attrib_buffer_offset;
    }

    inline VkIndexType Mesh::getIndexType() const noexcept
    {
        return _index_type;
    }

    inline AABB& Mesh::getAABB() noexcept
    {
        return _aabb;
    }

    inline const AABB& Mesh::getAABB() const noexcept
    {
        return _aabb;
    }

    inline PtrMaterial& Mesh::getMaterial() noexcept
    {
        return _ptr_material;
    }

    inline const PtrMaterial& Mesh::getMaterial() const noexcept
    {
        return _ptr_material;
    }

    inline void Mesh::setIndex(size_t i, uint32_t val)
    {
        assert(_ptr_index_buffer->getDeviceMemory()->getMapStatus() == MapStatus::MAPPED);
        assert(i < _num_indices);

        *(reinterpret_cast<uint32_t*>(_ptr_index_buffer->getDeviceMemory()->getData() + _index_buffer_offset) + i) = val;
    }

    inline void Mesh::setIndexBuffer(const PtrBuffer& ptr_buffer)
    {
        if (ptr_buffer->getSize() % sizeof(uint32_t)) {
            throw invalid_argument("Размер буфера не кратен sizeof(uint32_t)");
        }

        _ptr_index_buffer = ptr_buffer;
    }

    inline void Mesh::setVertexBuffer(const PtrBuffer& ptr_buffer)
    {
        if (ptr_buffer->getSize() % sizeof(VertexAttrib)) {
            throw invalid_argument("Размер буфера не кратен sizeof(Mesh::VertexAttrib)");
        }

        _ptr_vertex_attrib_buffer = ptr_buffer;
    }

    inline void Mesh::setAABB(const AABB& bbox)
    {
        _aabb = bbox;
    }

    inline void Mesh::setData(const vector<uint32_t>& ib)
    {
        if ((_index_buffer_offset + ib.size()) * sizeof(uint32_t) >= _ptr_index_buffer->getSize()) {
            throw invalid_argument("Выход за границу памяти.");
        }

        bool is_mapped = _ptr_index_buffer->getDeviceMemory()->getMapStatus() == MapStatus::MAPPED;

        if (!is_mapped) {
            _ptr_index_buffer->getDeviceMemory()->map();
        }

        uint8_t*    ptr     = _ptr_index_buffer->getDeviceMemory()->getData();
        size_t      size    = std::min(static_cast<size_t>(_ptr_index_buffer->getSize()), sizeof(uint32_t) * ib.size());

        memcpy(ptr, ib.data(), size);

        if (!is_mapped) {
            _ptr_index_buffer->getDeviceMemory()->unmap();
        }
    }

    inline void Mesh::setData(const vector<Mesh::VertexAttrib>& vb)
    {
        if ((_vertex_attrib_buffer_offset + vb.size()) * sizeof(VertexAttrib) >= _ptr_vertex_attrib_buffer->getSize()) {
            throw invalid_argument("Выход за границу памяти.");
        }

        bool is_mapped = _ptr_vertex_attrib_buffer->getDeviceMemory()->getMapStatus() == MapStatus::MAPPED;

        if (!is_mapped) {
            _ptr_vertex_attrib_buffer->getDeviceMemory()->map();
        }

        uint8_t*    ptr     = _ptr_vertex_attrib_buffer->getDeviceMemory()->getData();
        size_t      size    = std::min(static_cast<size_t>(_ptr_vertex_attrib_buffer->getSize()), sizeof(uint32_t) * vb.size());

        memcpy(ptr, vb.data(), size);

        if (!is_mapped) {
            _ptr_vertex_attrib_buffer->getDeviceMemory()->unmap();
        }
    }

    inline void Mesh::setNumVertices(size_t num)
    {
        _num_vertices = static_cast<uint32_t>(num);
    }

    inline void Mesh::setNumIndices(size_t num)
    {
        _num_indices = static_cast<uint32_t>(num);
    }

    inline void Mesh::setVertexBufferOffset(size_t offset)
    {
        _vertex_attrib_buffer_offset = static_cast<uint32_t>(offset * sizeof(VertexAttrib));
    }

    inline void Mesh::setIndexBufferOffset(size_t offset)
    {
        _index_buffer_offset = static_cast<uint32_t>(offset * sizeof(uint32_t));
    }

    inline void Mesh::copyIndexBuffer(vector<uint32_t>& ib)
    {
        bool is_mapped = _ptr_index_buffer->getDeviceMemory()->getMapStatus() == MapStatus::MAPPED;

        if (!is_mapped) {
            _ptr_index_buffer->getDeviceMemory()->map();
        }

        uint8_t*    ptr_data    = _ptr_index_buffer->getDeviceMemory()->getData();
        size_t      size        = _ptr_index_buffer->getSize();

        if (size_t num_ind = size / sizeof(uint32_t); num_ind != ib.size()) {
            ib.resize(num_ind);
        }

        memcpy(ib.data(), ptr_data, size);

        if (!is_mapped) {
            _ptr_index_buffer->getDeviceMemory()->unmap();
        }
    }

    inline void Mesh::setMaterial(const PtrMaterial& ptr_material)
    {
        _ptr_material = ptr_material;
    }

    inline void Mesh::copyVertexBuffer(vector<Mesh::VertexAttrib>& vb)
    {
        bool is_mapped = _ptr_vertex_attrib_buffer->getDeviceMemory()->getMapStatus() == MapStatus::MAPPED;

        if (!is_mapped) {
            _ptr_vertex_attrib_buffer->getDeviceMemory()->map();
        }

        uint8_t*    ptr_data    = _ptr_vertex_attrib_buffer->getDeviceMemory()->getData();
        size_t      size        = _ptr_vertex_attrib_buffer->getSize();

        if (size_t num_vert = size / sizeof(VertexAttrib); num_vert != vb.size()) {
            vb.resize(num_vert);
        }

        memcpy(vb.data(), ptr_data, size);

        if (!is_mapped) {
            _ptr_vertex_attrib_buffer->getDeviceMemory()->unmap();
        }
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

        mesh._aabb                      = _bbox;
        mesh._index_buffer_offset       = 0;
        mesh._ptr_index_buffer          = _index_buffer_builder.buildPtr();
        mesh._ptr_material              = nullptr;
        mesh._ptr_vertex_attrib_buffer  = _vertex_buffer_builder.buildPtr();
        mesh._num_vertices              = static_cast<uint32_t>(mesh._ptr_vertex_attrib_buffer->getSize() / sizeof(VertexAttrib));
        mesh._num_indices               = static_cast<uint32_t>(mesh._ptr_index_buffer->getSize() / sizeof(uint32_t));

        return mesh;
    }

    inline PtrMesh Mesh::Builder::buildPtr()  const
    {
        PtrMesh ptr_mesh = Mesh::make(_name);

        ptr_mesh->_aabb                     = _bbox;
        ptr_mesh->_index_buffer_offset      = 0;
        ptr_mesh->_ptr_index_buffer         = _index_buffer_builder.buildPtr();
        ptr_mesh->_ptr_material             = nullptr;
        ptr_mesh->_ptr_vertex_attrib_buffer = _vertex_buffer_builder.buildPtr();
        ptr_mesh->_num_vertices             = static_cast<uint32_t>(ptr_mesh->_ptr_vertex_attrib_buffer->getSize() / sizeof(VertexAttrib));
        ptr_mesh->_num_indices              = static_cast<uint32_t>(ptr_mesh->_ptr_index_buffer->getSize() / sizeof(uint32_t));

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

        mesh._aabb                      = _bbox;
        mesh._index_buffer_offset       = 0;
        mesh._ptr_index_buffer          = _index_buffer_builder.buildPtr();
        mesh._ptr_material              = nullptr;
        mesh._ptr_vertex_attrib_buffer  = _vertex_buffer_builder.buildPtr();
        mesh._num_vertices              = mesh._ptr_vertex_attrib_buffer->getSize() / sizeof(VertexAttrib);
        mesh._num_indices               = mesh._ptr_index_buffer->getSize() / sizeof(uint32_t);

        return mesh;
    }

    template<typename VertexAttribAllocatorType, typename IndexAllocatorType>
    inline PtrMesh Mesh::BuilderWithData<VertexAttribAllocatorType, IndexAllocatorType>::buildPtr() const
    {
        PtrMesh ptr_mesh = Mesh::make(_name);

        ptr_mesh->_aabb                     = _bbox;
        ptr_mesh->_index_buffer_offset      = 0;
        ptr_mesh->_ptr_index_buffer         = _index_buffer_builder.buildPtr();
        ptr_mesh->_ptr_material             = nullptr;
        ptr_mesh->_ptr_vertex_attrib_buffer = _vertex_buffer_builder.buildPtr();
        ptr_mesh->_num_vertices             = ptr_mesh->_ptr_vertex_attrib_buffer->getSize() / sizeof(VertexAttrib);
        ptr_mesh->_num_indices              = ptr_mesh->_ptr_index_buffer->getSize() / sizeof(uint32_t);

        return ptr_mesh;
    }
}