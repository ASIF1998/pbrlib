#pragma once

#include <string_view>

namespace pbrlib::backend
{
    class FrameGraph;
    class MaterialManager;
    class MeshManager;
}

namespace pbrlib::backend::vk
{
    class Device;
    class Image;
}

namespace pbrlib
{
    class Engine;
}

namespace pbrlib::testing
{
    class FrameGraphResourcesGetter final
    {
    public:
        explicit FrameGraphResourcesGetter(Engine& engine);

        FrameGraphResourcesGetter(FrameGraphResourcesGetter&& resource_getter)      = delete;
        FrameGraphResourcesGetter(const FrameGraphResourcesGetter& resource_getter) = delete;

        FrameGraphResourcesGetter& operator = (FrameGraphResourcesGetter&& resource_getter)         = delete;
        FrameGraphResourcesGetter& operator = (const FrameGraphResourcesGetter& resource_getter)    = delete;

        [[nodiscard]] backend::vk::Device&  device();
        [[nodiscard]] backend::vk::Image*   image(std::string_view name);

        [[nodiscard]] const backend::MaterialManager* materialManager() const;
        [[nodiscard]] const backend::MeshManager*     meshManager()   const;

    private:
        backend::FrameGraph* _ptr_frame_graph;
    };
}