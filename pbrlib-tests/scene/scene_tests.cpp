#include "../utils.hpp"

#include <pbrlib/scene/Scene.hpp>

#include <optional>

TEST(SceneTests, Ctor)
{
    pbrlib::Scene scene ("scene-tests");
    ASSERT_EQ(scene.name(), "scene-tests");
}