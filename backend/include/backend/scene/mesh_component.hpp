#pragma once

#include <backend/renderer/vulkan/buffer.hpp>

#include <pbrlib/math/vec3.hpp>
#include <pbrlib/math/vec2.hpp>

namespace pbrlib
{
    struct VertexAttribute
    {
        pbrlib::math::vec3 pos;
        pbrlib::math::vec3 normal;
        pbrlib::math::vec3 tangent;
        pbrlib::math::vec2 uv;
    };

    struct MeshComponent
    {
        vk::Buffer  vertex_buffer;
        uint32_t    vertex_count = 0;

        vk::Buffer  index_buffer;
        uint32_t    index_count = 0;
    };
}
