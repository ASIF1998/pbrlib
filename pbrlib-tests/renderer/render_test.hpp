#pragma once

#include "../utils.hpp"
#include "../image_comparison/image_comparison.hpp"

#include "framegraph_resources_getter.hpp"

#include <backend/utils/paths.hpp>
#include <backend/renderer/vulkan/image.hpp>

#include <pbrlib/config.hpp>
#include <pbrlib/scene/scene.hpp>
#include <pbrlib/engine.hpp>

#include <optional>

#include <filesystem>

#include <string_view>

namespace pbrlib::testing
{
    struct Settings
    {
        math::vec3 up   = math::vec3(0, 1, 0);
        math::vec3 pos  = math::vec3(0, 0, 0);
        math::vec3 eye  = math::vec3(0, 0, 1);

        float fov = 60.0f;
    };

    class RenderTest :
        public ::testing::Test
    {
        void SetUp() override;
        void TearDown() override;

    public:
        explicit RenderTest(std::string_view name, uint32_t width, uint32_t height);

        void setup(const std::filesystem::path& content, const Settings& settings);
        void check(const std::filesystem::path& reference_image, std::string_view attachment_name);

        pbrlib::Config& config() noexcept;
        pbrlib::Engine& engine() noexcept;

    private:
        std::optional<pbrlib::Engine>                   _engine;
        std::optional<pbrlib::testing::ImageComparison> _comparator;

        pbrlib::Config _config;

        std::optional<pbrlib::testing::FrameGraphResourcesGetter> _frame_graph_getter;
    };
}
