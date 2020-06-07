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

        inline uint32_t    getIndex(size_t i) const;
        inline void        setIndex(size_t i, uint32_t val);

        inline virtual type_index getType() const override;

        inline static PtrMesh make(const string_view name = "Mesh");

    public:
        PtrBuffer           ptr_index_buffer;
        PtrBuffer           ptr_vertex_attrib_buffer;
        uint32_t            num_vertices;
        uint32_t            vertex_attrib_buffer_offset;    //!< В sizeof(uint8_t).
        uint32_t            num_indices;
        uint32_t            index_buffer_offset;  	          
        VkIndexType         index_type                      = VK_INDEX_TYPE_UINT32;
        uint32_t            stride                          = sizeof(VertexAttrib);
        VkPrimitiveTopology topology                        = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        AABB                aabb;
        PtrMaterial         ptr_material;
    };
}

#include "Mesh.inl"

#endif /* Mesh_hpp */
