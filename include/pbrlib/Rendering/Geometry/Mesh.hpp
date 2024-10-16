//
//  Mesh.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 29/04/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef Mesh_hpp
#define Mesh_hpp

#include <pbrlib/Rendering/VulkanWrapper/Buffer.hpp>
#include <pbrlib/Rendering/Geometry/AABB.hpp>
#include <pbrlib/Rendering/Material/Material.hpp>

#include <pbrlib/scene/Component.hpp>

#include <pbrlib/math/vec3.hpp>

#include <span>

namespace pbrlib
{
    enum class MeshAttribute
    {
        Position,
        UV,
        Normal,
        Tangent,

        Count
    };

    struct Mesh :
        public Component<Mesh>
    {
        friend class MeshAssimp;

    public:
        /**
         * @struct Vertex.
         * @brief Данный класс описывает атрибуты одной вершины.
        */
        struct VertexAttrib
        {
            math::Vec3<float> position;
            math::Vec2<float> uv;
            math::Vec3<float> normal;
            math::Vec3<float> tangent;
        };

        class Builder
        {
        public:
            inline void setAABB(const AABB& bbox);
            inline void setName(const std::string_view name);

            inline Buffer::Builder<VertexAttrib>&           getVertexBufferBuilder()    noexcept;
            inline const Buffer::Builder<VertexAttrib>&     getVertexBufferBuilder()    const noexcept;
            inline Buffer::Builder<uint32_t>&               getIndexBufferBuilder()     noexcept;
            inline const Buffer::Builder<uint32_t>&         getIndexBufferBuilder()     const noexcept;

            inline Mesh                     build()     const;
            inline std::unique_ptr<Mesh>    buildPtr()  const;

        private:
            Buffer::Builder<VertexAttrib>   _vertex_buffer_builder;
            Buffer::Builder<uint32_t>       _index_buffer_builder;
            AABB                            _bbox;
            std::string                     _name;
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
            inline void setName(const std::string_view name);

            inline TVertexBufferBuilder&        getVertexBufferBuilder()    noexcept;
            inline const TVertexBufferBuilder&  getVertexBufferBuilder()    const noexcept;
            inline TIndexBufferBuilder&         getIndexBufferBuilder()     noexcept;
            inline const TIndexBufferBuilder&   getIndexBufferBuilder()     const noexcept;

            inline Mesh                     build()     const;
            inline std::unique_ptr<Mesh>    buildPtr()  const;

        private:
            TVertexBufferBuilder    _vertex_buffer_builder;
            TIndexBufferBuilder     _index_buffer_builder;
            AABB                    _bbox;
            std::string             _name;
        };

    public:
        inline Mesh(const std::string_view name = "Mesh");

        inline void mapVertexAttribBuffer()     const;
        inline void unmapVertexAttribBuffer()   const;

        inline void mapIndexBuffer()    const;
        inline void unmapIndexBuffer()  const;

        template<MeshAttribute Attrib>
        inline auto& getAttribute(size_t i);

        template<MeshAttribute Attrib>
        inline const auto& getAttribute(size_t i) const;

        inline uint32_t                         getIndex(size_t i)          const;
        inline std::shared_ptr<const Buffer>    getIndexBuffer()            const noexcept;
        inline std::shared_ptr<const Buffer>    getVertexBuffer()           const noexcept;
        inline uint32_t                         getNumIndices()             const noexcept;
        inline uint32_t                         getNumVertices()            const noexcept;
        inline uint32_t                         getIndexBufferOffset()      const noexcept; //!< Возвращает смещение в байтах.
        inline uint32_t                         getVertexBufferOffset()     const noexcept; //!< Возвращает смещение в байтах.
        inline VkIndexType                      getIndexType()              const noexcept;
        inline AABB&                            getAABB()                   noexcept;
        inline const AABB&                      getAABB()                   const noexcept; 
        inline std::shared_ptr<const Material>  getMaterial()               const noexcept;

        inline void setIndex(size_t i, uint32_t val);
        inline void setIndexBuffer(std::shared_ptr<Buffer> ptr_buffer);
        inline void setVertexBuffer(std::shared_ptr<Buffer> ptr_buffer);
        inline void setAABB(const AABB& bbox);
        inline void setNumVertices(size_t num);
        inline void setNumIndices(size_t num);
        inline void setVertexBufferOffset(size_t offset);
        inline void setIndexBufferOffset(size_t offset);
        inline void setMaterial(std::shared_ptr<const Material> ptr_material);

        /**
         * При использовании этих методов ограничивающий объём остаётся прежним.
         * Для установки огрничивающего объёма воспользуйтесь методом setAABB.
        */
        inline void setData(std::span<const uint32_t> ib);
        inline void setData(std::span<const VertexAttrib> vb);

        /**
         * @brief Метод копирует содержимое индексного буффера в ib.
         * 
         * @param[out] ib буффер, в который будет осуществлятся копирование.
        */
        inline void copyIndexBuffer(std::vector<uint32_t>& ib);

        /**
         * @brief Метод копирует содержимое вершинного буффера в vb.
         * 
         * @param[out] vb буффер, в который будет осуществлятся копирование.
        */
        inline void copyVertexBuffer(std::vector<VertexAttrib>& vb);

        inline static std::unique_ptr<Mesh> make(const std::string_view name = "Mesh");

    private:
        std::shared_ptr<Buffer>         _ptr_index_buffer;
        std::shared_ptr<Buffer>         _ptr_vertex_attrib_buffer;
        uint32_t                        _num_vertices;
        uint32_t                        _vertex_attrib_buffer_offset;    //!< В sizeof(uint8_t).
        uint32_t                        _num_indices;
        uint32_t                        _index_buffer_offset;  	         //!< В sizeof(uint8_t).
        VkIndexType                     _index_type                      = VK_INDEX_TYPE_UINT32;
        uint32_t                        _stride                          = sizeof(VertexAttrib);
        AABB                            _aabb;
        std::shared_ptr<const Material> _ptr_material;
    };
}

#include "Mesh.inl"

#endif /* Mesh_hpp */
