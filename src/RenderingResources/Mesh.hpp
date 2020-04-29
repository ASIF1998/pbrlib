//
//  Mesh.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 29/04/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef Mesh_hpp
#define Mesh_hpp

#include "../Rendering/VulkanWrapper/Buffer.hpp"

#include "../math/vec3.hpp"

#include "AABB.hpp"

namespace pbrlib
{
    enum class MeshAttribute
    {
        Position,
        UV,
        Normal,
        Tangent,
    };

    struct Mesh
    {
    public:
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

    public:
        shared_ptr<Buffer>      ptr_index_buffer;
        shared_ptr<Buffer>      ptr_vertex_attrib_buffer;
        uint32_t                num_vertices;
        uint32_t                vertex_attrib_buffer_offset;    /// В sizeof(uint8_t).
        uint32_t                num_indices;
        uint32_t                index_buffer_offset;            /// В sizeof(uint8_t).
        VkIndexType             index_type                      = VK_INDEX_TYPE_UINT32;
        uint32_t                stride                          = sizeof(float) * 11;
        VkPrimitiveTopology     topology                        = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        AABB                    aabb;

        /**
         * TODO: 
         *      shared_ptr<ImageView> views [Material::Textures::NUM_TEXTURES];
        */
    };
}

#include "Mesh.inl"

#endif /* Mesh_hpp */
