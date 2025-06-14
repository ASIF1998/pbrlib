#pragma once

#include <backend/renderer/vulkan/buffer.hpp>

#include <pbrlib/math/vec4.hpp>
#include <pbrlib/math/vec2.hpp>
#include <pbrlib/math/matrix4x4.hpp>
#include <pbrlib/math/aabb.hpp>

#include <optional>

#include <vector>
#include <unordered_map>

#include <string_view>

namespace pbrlib
{
    class SceneItem;
}

namespace pbrlib::backend
{
    struct VertexAttribute final
    {
        pbrlib::math::vec4      pos;
        pbrlib::math::u16vec3   normal;
        pbrlib::math::u16vec3   tangent;
        pbrlib::math::u16vec2   uv;
    };

    struct MeshDraw final
    {
        math::mat4  model;
        math::mat4  normal;
    };

    class MeshManager final
    {
    public:
        struct Bindings
        {
            enum
            {
                eVertexBuffersReferences,
                eMeshesDrawInfos,

                eCount
            }; 
        };

        explicit MeshManager(vk::Device& device);
        
        MeshManager(MeshManager&& manager)      = delete;
        MeshManager(const MeshManager& manager) = delete;
        
        ~MeshManager();
        
        MeshManager& operator = (MeshManager&& manager)         = delete;
        MeshManager& operator = (const MeshManager& manager)    = delete;
        
        void update();

        void add (
            std::string_view                    name,
            std::span<const VertexAttribute>    attributes, 
            std::span<const uint32_t>           indices,
            SceneItem*                          ptr_item
        );

        void updateItemTransform(const SceneItem* ptr_item, const math::mat4& transform);

        [[nodiscard]] VkDescriptorSet descriptorSet() const;

        [[nodiscard]] const vk::Buffer& indexBuffer(uint32_t mesh_id)   const;
        [[nodiscard]] const vk::Buffer& vertexBuffer(uint32_t mesh_id)  const;

        [[nodiscard]] size_t meshCount() const;        

    private:
        vk::Device& _device;

        std::vector<vk::Buffer> _vbos;
        std::vector<vk::Buffer> _ibos;

        std::optional<vk::Buffer> _vbos_refs;

        std::vector<MeshDraw>       _meshes_draw_info;
        std::optional<vk::Buffer>   _meshes_draw_buffer;

        VkDescriptorSet         _descriptor_set_handle          = VK_NULL_HANDLE;
        VkDescriptorSetLayout   _descriptor_set_layout_handle   = VK_NULL_HANDLE;

        bool _descriptor_set_is_changed = true;

        std::unordered_map<const SceneItem*, size_t> _item_to_mesh_draw_info_index;
    };
}