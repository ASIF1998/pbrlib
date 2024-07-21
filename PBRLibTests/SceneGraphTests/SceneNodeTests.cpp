//
//  SceneNodeTests.cpp
//  PBRLibTests
//
//  Created by Асиф Мамедов on 15/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include "../utils.hpp"

#include <pbrlib/SceneGraph/Scene.hpp>
#include <pbrlib/SceneGraph/Component.hpp>

#include <string>
#include <string_view>

using namespace pbrlib;
using namespace std;

struct TestNodeComponent :
    public Component<TestNodeComponent>
{
    TestNodeComponent() :
        Component("Test Node Component")
    {}
    
    int i;
};

TEST(SceneGraphNode, Constructor)
{
    constexpr string_view   name1   = "No name";
    const string            name2   = "Node 2";

    AABB raabb;

    SceneItem node1;
    SceneItem node2(name2);

    constexpr Matrix4x4<float> rm;

    pbrlib::testing::utils::thisFalse(node1.worldAABBIsCurrent(), "При инициализации мировой ограничивающий объём должен быть не актуальным.");
    pbrlib::testing::utils::thisFalse(node2.worldAABBIsCurrent(), "При инициализации мировой ограничивающий объём должен быть не актуальным.");

    pbrlib::testing::utils::thisFalse(node1.worldTransformIsCurrent(), "При инициализации мировое преобразование должно быть не актуальным.");
    pbrlib::testing::utils::thisFalse(node2.worldTransformIsCurrent(), "При инициализации мировое преобразование должно быть не актуальным.");

    pbrlib::testing::utils::equality(raabb[0], node1.getWorldAABB()[0], "Не правильная инициализация мирового ограничивающего объёма."); 
    pbrlib::testing::utils::equality(raabb[1], node1.getWorldAABB()[1], "Не правильная инициализация мирового ограничивающего объёма.");

    EXPECT_EQ(raabb, node2.getWorldAABB()) << "Не правильная инициализация мирового ограничивающего объёма." << endl;
    pbrlib::testing::utils::equality(raabb[0], node2.getWorldAABB()[0], "Не правильная инициализация мирового ограничивающего объёма."); 
    pbrlib::testing::utils::equality(raabb[1], node2.getWorldAABB()[1], "Не правильная инициализация мирового ограничивающего объёма.");

    pbrlib::testing::utils::equality(static_cast<size_t>(0), reinterpret_cast<size_t>(node1.getParent()), "При инициализации у объекта появился указатель на родителя (его не должно быть).");
    pbrlib::testing::utils::equality(static_cast<size_t>(0), reinterpret_cast<size_t>(node2.getParent()), "При инициализации у объекта появился указатель на родителя (его не должно быть).");
    
    pbrlib::testing::utils::thisTrue(node1.getChildrenCount() == 0, "При инициализирование появились дочерние узлы.");
    pbrlib::testing::utils::thisTrue(node2.getChildrenCount() == 0, "При инициализирование появились дочерние узлы.");

    pbrlib::testing::utils::equality(rm, node1.getWorldTransform().getMatrix(), "Не правильное инициализирование мирового преобразования.");
    pbrlib::testing::utils::equality(rm, node2.getWorldTransform().getMatrix(), "Не правильное инициализирование мирового преобразования.");

    pbrlib::testing::utils::equality(rm, node1.getLocalTransform().getMatrix(), "Не правильное инициализирование локального преобразования.");
    pbrlib::testing::utils::equality(rm, node2.getLocalTransform().getMatrix(), "Не правильное инициализирование локального преобразования.");

    pbrlib::testing::utils::equality(string(name1), node1.getName(), "Не правильное инициализирование имени.");
    pbrlib::testing::utils::equality(name2, node2.getName(), "Не правильное инициализирование имени.");
}

TEST(SceneGraphNode, GettersAndSetters)
{
    srand((uint32_t)time(nullptr));
    
    Transform world_transform = Transform::translate(Vec3<float>(-5.0f, 25.0f, -13.5f));
    Transform local_transform = Transform::rotateX(60.0f);

    AABB world_aabb(
        Vec3<float> (-3.0f, 4.000f, -1.0f),
        Vec3<float> (34.0f, -32.0f, 1.00f)
    );
    
    int rval = rand() % 10000;

    constexpr string_view node1_name    = "Node 1";
    constexpr string_view node2_name    = "Node 2";
    constexpr string_view node3_name    = "Node 3";
    constexpr string_view node4_name    = "Node 4";

    SceneItem node1   (node1_name);
    SceneItem node2   ("No name");

    std::shared_ptr node3 = SceneItem::make(node3_name);
    std::shared_ptr node4 = node2.addChild(node4_name);

    node2.setParent(&node1);
    node2.addChild(node3);
    node2.setWorldTransform(world_transform);
    node2.setLocalTransform(local_transform);
    node2.setWorldAABB(world_aabb);
    node2.setName(node2_name);

    pbrlib::testing::utils::equality(reinterpret_cast<size_t>(&node1), reinterpret_cast<size_t>(node2.getParent()), "Не правильно работает метод setParent(...) в классе Scene::Node.");

    pbrlib::testing::utils::equality(
        std::static_pointer_cast<const SceneItem>(node4), 
        node2.getChild(0), 
        "Не правильно работает метод addChild(const string_view).\n"
        "Дочерний узел node4 должен быть первым." 
    );

    pbrlib::testing::utils::equality(
        node4->getParent(), 
        &node2, 
        "Не правильно работает метод addChild(const string_view).\n"
        "Узел node2 должен быть родителем узла node4."
    );

    pbrlib::testing::utils::equality(
        std::static_pointer_cast<const SceneItem>(node3), 
        node2.getChild(1),
        "Не правильно работает метод addChild(const PtrNode&).\n"
        "Узел node3 должен быть вторым."
    );

    pbrlib::testing::utils::equality(
        node3->getParent(), 
        &node2,  
        "Не правильно работает метод addChild(const PtrNode&).\n"
        "Узел node2 должен быть родителем узла node3."
    );

    pbrlib::testing::utils::equality(node2.getWorldTransform().getMatrix(), world_transform.getMatrix(), "Не правильно работает метод setWorldTransform(...).");
    pbrlib::testing::utils::thisTrue(node2.worldTransformIsCurrent(), "Не правильно работает метод setWorldTransform(...).");
    pbrlib::testing::utils::equality(node2.getLocalTransform().getMatrix(), local_transform.getMatrix(), "Не правильно работает метод setLocalTransform(...).");

    pbrlib::testing::utils::equality(world_aabb, node2.getWorldAABB(), "Не правильно работает метод setWorldAABB(...).");
    pbrlib::testing::utils::thisTrue(node2.worldAABBIsCurrent(), "Не правильно работает метод setWorldAABB(...).");

    pbrlib::testing::utils::equality(string(node2_name), node2.getName(), "Не правильно работает метод setName(...).");
    
    node2.addComponent(make_shared<TestNodeComponent>());
    
    pbrlib::testing::utils::thisTrue(node2.hasComponent<TestNodeComponent>(), "Ошибка в addComponent(...) или в hasComponent()");
    
    node2.getComponent<TestNodeComponent>().i = rval;
    
    pbrlib::testing::utils::equality(rval, node2.getComponent<TestNodeComponent>().i, "Ошибка в getComponent(...). Не правильно инициализирована переменная.");
}

TEST(SceneGraphNode, UpdateDetachAndMakesTest)
{
    Transform t1 = Transform::translate(Vec3<float>(-5.0f, 0.0f, 0.0f));
    Transform t2 = Transform::rotateX(60.0f);
    Transform t3 = Transform::rotateY(90.0f);

    AABB bbox1(
        Vec3<float>(0.5f),
        Vec3<float>(1.0f)
    );

    AABB bbox2(
        Vec3<float>(0.5f),
        Vec3<float>(-1.0f)
    );

    AABB bbox3(
        Vec3<float>(1.0f),
        Vec3<float>(-1.0f)
    );

    SceneItem node1;

    auto node2 = node1.addChild("Node 2");
    auto node3 = node1.addChild("Node 3");
    auto node4 = node2->addChild("Node 4");
    auto node5 = node3->addChild("Node 5");

    node1.setLocalTransform(t1);
    node2->setLocalTransform(t2);
    node3->setLocalTransform(t3);

    node4->setWorldAABB(bbox1);
    node5->setWorldAABB(bbox2);

    node1.update(nullptr, 0.2f, Transform());

    pbrlib::testing::utils::equality(t1.getMatrix(), node1.getLocalTransform().getMatrix(), "Не правильно работает метод update(...).");
    pbrlib::testing::utils::equality(t1.getMatrix(), node2->getWorldTransform().getMatrix(), "Не правильно работает метод update(...).");
    pbrlib::testing::utils::equality(t1.getMatrix(), node3->getWorldTransform().getMatrix(), "Не правильно работает метод update(...).");
    pbrlib::testing::utils::equality((t1 * t2).getMatrix(), node4->getWorldTransform().getMatrix(), "Не правильно работает метод update(...).");
    pbrlib::testing::utils::equality((t1 * t3).getMatrix(), node5->getWorldTransform().getMatrix(), "Не правильно работает метод update(...).");

    pbrlib::testing::utils::equality(bbox3, node1.getWorldAABB(), "Не правильно работает метод update(...).");
    pbrlib::testing::utils::equality(bbox1, node2->getWorldAABB(), "Не правильно работает метод update(...).");
    pbrlib::testing::utils::equality(bbox2, node3->getWorldAABB(), "Не правильно работает метод update(...).");
    pbrlib::testing::utils::equality(bbox1, node4->getWorldAABB(), "Не правильно работает метод update(...).");
    pbrlib::testing::utils::equality(bbox2, node5->getWorldAABB(), "Не правильно работает метод update(...).");

    pbrlib::testing::utils::notEquality(static_cast<size_t>(0), reinterpret_cast<size_t>(node1.getChild(0).get()));
    pbrlib::testing::utils::notEquality(static_cast<size_t>(0), reinterpret_cast<size_t>(node1.getChild(1).get()));

    node1.detachChild("Node 2");

    pbrlib::testing::utils::equality(static_cast<size_t>(0), reinterpret_cast<size_t>(node1.getChild(0).get()));
    pbrlib::testing::utils::notEquality(static_cast<size_t>(0), reinterpret_cast<size_t>(node1.getChild(1).get()));

    node1.detachChild(node3);

    pbrlib::testing::utils::equality(static_cast<size_t>(0), reinterpret_cast<size_t>(node1.getChild(0).get()));
    pbrlib::testing::utils::equality(static_cast<size_t>(0), reinterpret_cast<size_t>(node1.getChild(1).get()));
}
