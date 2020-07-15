//
//  Mesh.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 29/04/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef Mesh_hpp
#define Mesh_hpp

#include "../VulkanWrapper/Buffer.hpp"

#include "../../math/vec3.hpp"

#include "AABB.hpp"

#include "../../SceneGraph/Component.hpp"

#include "../Material/Material.hpp"

namespace pbrlib
{
    class Mesh;

    using PtrMesh = shared_ptr<Mesh>;

    enum class MeshAttribute
    {
        Position,
        UV,
        Normal,
        Tangent,

        Count
    };
    
    struct Mesh :
        public Component
    {
        friend class MeshAssimp;
        
    public:
        /**
         * @struct Vertex.
         * @brief Данный класс описывает атрибуты одной вершины.
        */
        struct VertexAttrib
        {
            Vec3<float> position;
            Vec2<float> uv;
            Vec3<float> normal;
            Vec3<float> tangent;
        };

        class Builder
        {
        public:
            inline void setAABB(const AABB& bbox);
            inline void setName(const string_view name);

            inline Buffer::Builder<VertexAttrib>&           getVertexBufferBuilder()    noexcept;
            inline const Buffer::Builder<VertexAttrib>&     getVertexBufferBuilder()    const noexcept;
            inline Buffer::Builder<uint32_t>&               getIndexBufferBuilder()     noexcept;
            inline const Buffer::Builder<uint32_t>&         getIndexBufferBuilder()     const noexcept;

            inline Mesh     build()     const;
            inline PtrMesh  buildPtr()  const;

        private:
            Buffer::Builder<VertexAttrib>   _vertex_buffer_builder;
            Buffer::Builder<uint32_t>       _index_buffer_builder;
            AABB                            _bbox;
            string                          _name;
        };

        template<
            typename VertexAttribAllocatorType  = STLAlignedAllocator<VertexAttrib>,
            typename IndexAllocatorType         = STLAlignedAllocator<uint32_t>
        >
        class BuilderWithData
        {
        public:
            using TVertexBufferBuilder  = Buffer::BuilderWithData<VertexAttrib, VertexAttribAllocatorType>;
            using TIndexBufferBuilder   = Buffer::BuilderWithData<uint32_t, IndexAllocatorType>;

        public:
            inline void setAABB(const AABB& bbox);
            inline void setName(const string_view name);

            inline TVertexBufferBuilder&        getVertexBufferBuilder()    noexcept;
            inline const TVertexBufferBuilder&  getVertexBufferBuilder()    const noexcept;
            inline TIndexBufferBuilder&         getIndexBufferBuilder()     noexcept;
            inline const TIndexBufferBuilder&   getIndexBufferBuilder()     const noexcept;

            inline Mesh     build()     const;
            inline PtrMesh  buildPtr()  const;
            
        private:
            TVertexBufferBuilder    _vertex_buffer_builder;
            TIndexBufferBuilder     _index_buffer_builder;
            AABB                    _bbox;
            string                  _name;
        };

    public:
        inline Mesh(const string_view name = "Mesh");

        inline void mapVertexAttribBuffer()    const;
        inline void unmapVertexAttribBuffer()  const;

        inline void mapIndexBuffer()   const;
        inline void unmapIndexBuffer() const;

        template<MeshAttribute Attrib>
        inline auto& getAttribute(size_t i);

        template<MeshAttribute Attrib>
        inline const auto& getAttribute(size_t i) const;

        inline uint32_t             getIndex(size_t i)          const;
        inline PtrBuffer&           getIndexBuffer()            noexcept;
        inline const PtrBuffer&     getIndexBuffer()            const noexcept;
        inline PtrBuffer&           getVertexBuffer()           noexcept;
        inline const PtrBuffer&     getVertexBuffer()           const noexcept;
        inline uint32_t             getNumIndices()             const noexcept;
        inline uint32_t             getNumVertices()            const noexcept;
        inline uint32_t             getIndexBufferOffset()      const noexcept; //!< Возвращает смещение в байтах.
        inline uint32_t             getVertexBufferOffset()     const noexcept; //!< Возвращает смещение в байтах.
        inline VkIndexType          getIndexType()              const noexcept;
        inline AABB&                getAABB()                   noexcept;
        inline const AABB&          getAABB()                   const noexcept; 
        inline PtrMaterial&         getMaterial()               noexcept;
        inline const PtrMaterial&   getMaterial()               const noexcept;
        inline virtual type_index   getType()                   const override;

        inline void setIndex(size_t i, uint32_t val);
        inline void setIndexBuffer(const PtrBuffer& ptr_buffer);
        inline void setVertexBuffer(const PtrBuffer& ptr_buffer);
        inline void setAABB(const AABB& bbox);
        inline void setNumVertices(size_t num);
        inline void setNumIndices(size_t num);
        inline void setVertexBufferOffset(size_t offset);
        inline void setIndexBufferOffset(size_t offset);
        inline void setMaterial(const PtrMaterial& ptr_material);

        /**
         * При использовании этих методов ограничивающий объём остаётся прежним.
         * Для установки огрничивающего объёма воспользуйтесь методом setAABB.
        */
        inline void setData(const vector<uint32_t>& ib);
        inline void setData(const vector<VertexAttrib>& vb);

        /**
         * @brief Метод копирует содержимое индексного буффера в ib.
         * 
         * @param[out] ib буффер, в который будет осуществлятся копирование.
        */
        inline void copyIndexBuffer(vector<uint32_t>& ib);

        /**
         * @brief Метод копирует содержимое вершинного буффера в vb.
         * 
         * @param[out] vb буффер, в который будет осуществлятся копирование.
        */
        inline void copyVertexBuffer(vector<VertexAttrib>& vb);

        inline static PtrMesh make(const string_view name = "Mesh");

    private:
        PtrBuffer           _ptr_index_buffer;
        PtrBuffer           _ptr_vertex_attrib_buffer;
        uint32_t            _num_vertices;
        uint32_t            _vertex_attrib_buffer_offset;    //!< В sizeof(uint8_t).
        uint32_t            _num_indices;
        uint32_t            _index_buffer_offset;  	         //!< В sizeof(uint8_t).
        VkIndexType         _index_type                      = VK_INDEX_TYPE_UINT32;
        uint32_t            _stride                          = sizeof(VertexAttrib);
        VkPrimitiveTopology _topology                        = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        AABB                _aabb;
        PtrMaterial         _ptr_material;
    };
}

#include "Mesh.inl"

#endif /* Mesh_hpp */
