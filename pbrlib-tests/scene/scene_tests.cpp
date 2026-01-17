#include "../utils.hpp"

#include <pbrlib/scene/Scene.hpp>

#include <optional>

TEST(SceneTests, Ctor)
{
    pbrlib::Scene scene ("scene-tests");
    pbrlib::testing::equality<std::string_view>(scene.name(), "scene-tests");
}
