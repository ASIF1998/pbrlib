#include "../utils.hpp"

#include <pbrlib/scene/scene.hpp>
#include <pbrlib/input/input_stay.hpp>

TEST(SceneItemTests, Ctor)
{
    pbrlib::Scene       scene("scene");
    pbrlib::math::mat4  transform (1.0f);

    const auto& item = scene.addItem("scene-item");

    pbrlib::testing::thisTrue(item.hasComponent<pbrlib::component::Tag>());
    pbrlib::testing::thisTrue(item.hasComponent<pbrlib::component::Transform>());

    const auto& tag_component = item.getComponent<pbrlib::component::Tag>();
    pbrlib::testing::equality<std::string_view>(tag_component.name, "scene-item");

    const auto& transform_component = item.getComponent<pbrlib::component::Transform>();
    pbrlib::testing::equality(transform_component.transform, transform);
}

TEST(SceneItemTests, UpdateCallback)
{
    struct TestComponent
    {
        int num = 0;
    };

    pbrlib::InputStay input_stay;

    constexpr int test_component_num_value = 213;

    pbrlib::Scene scene("scene");
    
    auto& item = scene.addItem("scene-item");

    item.updateCallback([](
        pbrlib::SceneItem&          item, 
        const pbrlib::InputStay&    input_stay, 
        float                       delta_time, 
        const pbrlib::math::mat4&   world_transform
    )
    {
        pbrlib::testing::thisTrue(item.hasComponent<TestComponent>());
        auto& test_component = item.getComponent<TestComponent>();
        test_component.num = test_component_num_value;
    });

    item.addComponent<TestComponent>();
    scene.update(input_stay, 0.0f);

    pbrlib::testing::thisTrue(item.hasComponent<TestComponent>());

    const auto& test_component = item.getComponent<TestComponent>();

    pbrlib::testing::equality(test_component.num, test_component_num_value);
}