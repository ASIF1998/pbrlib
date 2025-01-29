#include "../utils.hpp"

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
    ContentTests()
    {
        pbrlib::Config config 
        {
            .title          = "scene-importer-tests",
            .drawInWindow   = false
        };

        _engine.emplace(config);
    }

    bool check(const std::filesystem::path& filename)
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
            _engine->setupCallback([&filename, &result](pbrlib::Engine* ptr_engine, pbrlib::Scene* ptr_scene)
            {
                result = ptr_scene->import(pbrlib::utils::projectRoot() / "pbrlib-tests/content" / filename, ptr_engine);
            });

            _engine->run();

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
    pbrlib::testing::thisTrue(check("Blender 2.glb"));
}