#pragma once

#include <backend/renderer/vulkan/buffer.hpp>
#include <backend/renderer/vulkan/image.hpp>

#include <pbrlib/math/vec2.hpp>
#include <pbrlib/math/vec4.hpp>
#include <pbrlib/math/aabb.hpp>

#include <vector>

namespace pbrlib
{
    class SceneItem;
}

namespace pbrlib::backend::component
{
    struct Renderable final
    {
        Renderable() = default;

        const SceneItem* ptr_item = nullptr;

        uint32_t material_id = std::numeric_limits<uint32_t>::max();

        uint32_t    mesh_id         = std::numeric_limits<uint32_t>::max();
        size_t      vertex_count    = 0;
        size_t      index_count     = 0;

        pbrlib::math::AABB bbox;
    };
}