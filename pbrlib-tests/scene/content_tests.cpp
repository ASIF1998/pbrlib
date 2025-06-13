#include "../utils.hpp"

#include "../renderer/framegraph_resources_getter.hpp"

#include <backend/scene/material_manager.hpp>
#include <backend/scene/mesh_manager.hpp>

#include <pbrlib/config.hpp>
#include <pbrlib/engine.hpp>

#include <pbrlib/scene/scene.hpp>

#include <backend/utils/paths.hpp>

#include <optional>

#include <filesystem>

class ContentTests :
    public ::testing::Test
{
public:
    void SetUp() override 
    {
        if constexpr (!pbrlib::testing::vk::isSupport())
            GTEST_SKIP();

        pbrlib::Config config 
        {
            .title          = "scene-importer-tests",
            .draw_in_window = false
        };

        _engine.emplace(config);
    }

    void TearDown() override
    {
        _engine = std::nullopt;
    }

    bool check(const std::filesystem::path& filename, size_t imageCount, size_t materialCount, size_t meshCount)
    {
        /**
         * @todo
         * В будущем нужно будет тестировать следующим образом:
         *     1. добавить ещё один аргумент transform в метод check(), 
         *      что бы можно было нормальным образом спозиционировать модельку 
         *      в середине сцены
         *     2. рендерить сцену с 6 сторон и сравнивать результаты с эталоном
        */
        try 
        {
            bool result = false;
            _engine->setupCallback([&filename, &result](pbrlib::Engine& engine, pbrlib::Scene& scene)
            {
                result = scene.import(pbrlib::backend::utils::projectRoot() / "pbrlib-tests/content" / filename, engine);
            });

            pbrlib::testing::FrameGraphResourcesGetter getter (_engine.value());
            
            _engine->run();

            const auto ptr_mesh_manager = getter.meshManager();
            if (!ptr_mesh_manager)
                return false;

            result &= ptr_mesh_manager->meshCount() == meshCount;
            
            const auto ptr_material_manager = getter.materialManager();
            if (!ptr_material_manager)
                return false;

            result &= 
                    ptr_material_manager->imageCount() == imageCount 
                &&  ptr_material_manager->materialCount() == materialCount;

            return result;
        }
        catch (...)
        {
            return false;
        }

        return false;
    }

private:
    std::optional<pbrlib::Engine> _engine;
};

TEST_F(ContentTests, Blender_2)
{
    pbrlib::testing::thisTrue(check("Blender 2.glb", 140, 35, 35));
}