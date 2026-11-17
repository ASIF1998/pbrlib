#include "../utils.hpp"

#include <backend/components.hpp>

#include <pbrlib/scene/scene.hpp>
#include <pbrlib/input/input_stay.hpp>
#include <pbrlib/transforms.hpp>

TEST(SceneItemTests, Ctor)
{
    pbrlib::Scene       scene("scene");
    pbrlib::math::mat4  transform (1.0f);

    const auto& item = scene.addItem("scene-item");

    pbrlib::testing::thisTrue(item.hasComponent<pbrlib::components::Tag>());
    pbrlib::testing::thisTrue(item.hasComponent<pbrlib::components::Transform>());

    const auto& tag_component = item.getComponent<pbrlib::components::Tag>();
    pbrlib::testing::equality<std::string_view>(tag_component.name, "scene-item");

    const auto& transform_component = item.getComponent<pbrlib::components::Transform>();
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

    item.update([](
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

TEST(SceneItemTests, GetItem)
{
    pbrlib::Scene scene("scene");

    constexpr std::string_view item_name = "new-item";

    const auto& new_item = scene.addItem(item_name);

    const auto ptr_item = scene.item(item_name);

    const auto& tag_1 = new_item.getComponent<pbrlib::components::Tag>();
    const auto& tag_2 = ptr_item->getComponent<pbrlib::components::Tag>();

    pbrlib::testing::thisTrue(tag_1.name == tag_2.name);
}

TEST(SceneItemTests, CreateInstance)
{
    pbrlib::Scene scene("scene");

    constexpr std::string_view item_name            = "new-item";
    constexpr std::string_view instance_item_name   = "instance-item";

    const auto transform = pbrlib::transforms::translate(pbrlib::math::vec3(-3, -5, 10));

    const auto& new_item        = scene.addItem(item_name);
    const auto& instance_item   = scene.createInstance(item_name, instance_item_name, transform);

    const auto& instance_tag = instance_item.getComponent<pbrlib::components::Tag>();

    pbrlib::testing::thisTrue(instance_tag.name == instance_item_name);
}
