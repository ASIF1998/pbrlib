#pragma once

#include <backend/renderer/vulkan/buffer.hpp>

#if 0
#include <pbrlib/math/vec3.hpp>
#include <pbrlib/math/vec2.hpp>
#else
#include <pbrlib/math/vec4.hpp>
#endif
namespace pbrlib
{
    struct VertexAttribute
    {
#if 0
        pbrlib::math::vec3 pos;
        pbrlib::math::vec3 normal;
        pbrlib::math::vec3 tangent;
        pbrlib::math::vec2 uv;
#else 
        pbrlib::math::vec4 pos;
        pbrlib::math::vec4 normal;
        pbrlib::math::vec4 tangent;
        pbrlib::math::vec4 uv;
#endif
    };

    struct MeshComponent
    {
        vk::Buffer  vertex_buffer;
        uint32_t    vertex_count = 0;

        vk::Buffer  index_buffer;
        uint32_t    index_count = 0;
    };
}
