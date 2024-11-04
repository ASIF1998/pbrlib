#include "../utils.hpp"

#include <pbrlib/scene/Scene.hpp>

TEST(SceneItemTests, Ctor)
{
    pbrlib::Scene       scene("scene");
    pbrlib::Transform   transform;

    const auto& item = scene.addItem("scene-item");

    ASSERT_TRUE(item.hasComponent<pbrlib::TagComponent>());
    ASSERT_TRUE(item.hasComponent<pbrlib::TransformComponent>());

    const auto& tag_component = item.getComponent<pbrlib::TagComponent>();
    EXPECT_EQ(tag_component.name, "scene-item");

    const auto& transform_component = item.getComponent<pbrlib::TransformComponent>();
    EXPECT_EQ(transform_component.transform.getMatrix(), transform.getMatrix());
}

TEST(SceneItemTests, UpdateCallback)
{
    struct TestComponent
    {
        int num = 0;
    };

    constexpr int test_component_num_value = 213;

    pbrlib::Scene scene("scene");
    
    auto& item = scene.addItem("scene-item");

    item.updateCallback([](
        pbrlib::SceneItem*          ptr_item, 
        const pbrlib::InputStay*    ptr_input_stay, 
        float                       delta_time, 
        const pbrlib::Transform&    world_transform
    )
    {
        ASSERT_TRUE(ptr_item->hasComponent<TestComponent>());
        auto& test_component = ptr_item->getComponent<TestComponent>();
        test_component.num = test_component_num_value;
    });

    item.addComponent<TestComponent>();
    scene.update(nullptr, 0.0f);

    ASSERT_TRUE(item.hasComponent<TestComponent>());

    const auto& test_component = item.getComponent<TestComponent>();

    ASSERT_EQ(test_component.num, test_component_num_value);
}