#include "../utils/assertions.hpp"
#include "../utils/graphviz_generator.hpp"

#include <pbrlib/engine.hpp>
#include <pbrlib/config.hpp>

#include <pbrlib/scene/scene.hpp>

#include <backend/utils/paths.hpp>

#include <optional>

#include <fstream>

TEST(SceneTests, Ctor)
{
    pbrlib::Scene scene ("scene-tests");
    ASSERT_EQ(scene.name(), "scene-tests");
}

TEST(SceneTests, JunkShopImport)
{
    const auto filename = pbrlib::utils::projectRoot() / "pbrlib-tests/content/Blender 2.glb";

    pbrlib::Config config;
    config.width        = 10;    
    config.height       = 10;    
    config.resible      = false;
    config.title        = "junk-shop-import";
    config.drawInWindow = false;

    pbrlib::Engine engine (config);

    pbrlib::Scene scene ("junk-shop");
    scene.import(filename, &engine);

    auto ptr_graphviz_generator = std::make_unique<pbrlib::testing::GraphvizGenerator>();

    scene.visit(ptr_graphviz_generator);

    auto graph = ptr_graphviz_generator->source();

    std::ofstream file (pbrlib::utils::projectRoot() / "pbrlib-tests/content/graph.dot");
    if (file)
        file << ptr_graphviz_generator->source();

    file.close();
}