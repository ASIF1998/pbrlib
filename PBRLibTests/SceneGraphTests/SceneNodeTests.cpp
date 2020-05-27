//
//  SceneNodeTests.cpp
//  PBRLibTests
//
//  Created by Асиф Мамедов on 15/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <gtest/gtest.h>

#include "../../src/SceneGraph/Scene.hpp"
#include "../../src/SceneGraph/Component.hpp"
#include "../../src/SceneGraph/Script.hpp"

#include <string>
#include <string_view>

using namespace testing;
using namespace pbrlib;
using namespace std;

struct TestNodeComponent :
    public Component
{
    TestNodeComponent() :
        Component("Test Node Component")
    {}
    
    virtual type_index getType() const override
    {
        return ComponentUtil::getTypeIndex<TestNodeComponent>();
    }
    
    int i;
};

struct TestNodeScript :
    public Script
{
    TestNodeScript() :
        Script("Test Node Component")
    {}
    
    virtual type_index getType() const override
    {
        return ComponentUtil::getTypeIndex<TestNodeScript>();
    }
    
    virtual void update(Scene::Node* ptr_node, float delta_time) override
    {
        i = static_cast<int>(delta_time);
    }
    
    int i;
};

TEST(SceneGraphNode, Constructor)
{
    constexpr string_view   name1   = "No name";
    const string            name2   = "Node 2";

    AABB raabb;

    Scene::Node node1;
    Scene::Node node2(name2);

    constexpr Matrix4x4<float> rm;

    EXPECT_FALSE(node1.worldAABBIsCurrent()) << "При инициализации мировой ограничивающий объём должен быть не актуальным." << endl;
    EXPECT_FALSE(node2.worldAABBIsCurrent()) << "При инициализации мировой ограничивающий объём должен быть не актуальным." << endl;

    EXPECT_FALSE(node1.worldTransformIsCurrent()) << "При инициализации мировое преобразование должно быть не актуальным." << endl;
    EXPECT_FALSE(node2.worldTransformIsCurrent()) << "При инициализации мировое преобразование должно быть не актуальным." << endl;

    EXPECT_EQ(raabb, node1.getWorldAABB()) << "Не правильная инициализация мирового ограничивающего объёма." << endl; 
    EXPECT_EQ(raabb, node2.getWorldAABB()) << "Не правильная инициализация мирового ограничивающего объёма." << endl;

    EXPECT_EQ(nullptr, node1.getParent())   << "При инициализации у объекта появился указатель на родителя (его не должно быть)." << endl;
    EXPECT_EQ(nullptr, node2.getParent())   << "При инициализации у объекта появился указатель на родителя (его не должно быть)." << endl;
    
    EXPECT_TRUE(node1.getChildren().empty()) << "При инициализирование появились дочерние узлы." << endl;
    EXPECT_TRUE(node2.getChildren().empty()) << "При инициализирование появились дочерние узлы." << endl;

    EXPECT_EQ(rm, node1.getWorldTransform().getMatrix()) << "Не правильное инициализирование мирового преобразования." << endl;
    EXPECT_EQ(rm, node2.getWorldTransform().getMatrix()) << "Не правильное инициализирование мирового преобразования." << endl;

    EXPECT_EQ(rm, node1.getLocalTransform().getMatrix()) << "Не правильное инициализирование локального преобразования." << endl;
    EXPECT_EQ(rm, node2.getLocalTransform().getMatrix()) << "Не правильное инициализирование локального преобразования." << endl;

    EXPECT_EQ(name1, node1.getName()) << "Не правильное инициализирование имени." << endl;
    EXPECT_EQ(name2, node2.getName()) << "Не правильное инициализирование имени." << endl;
}

TEST(SceneGraphNode, GettersAndSetters)
{
    srand((uint)time(nullptr));
    
    Transform world_transform = Transform::translate(Vec3<float>(-5.0f, 25.0f, -13.5f));
    Transform local_transform = Transform::rotateX(60.0f);

    AABB world_aabb(
        Vec3<float> (-3.0f, 4.0f, -1.0f),
        Vec3<float> (34.0f, -32.0f, 1.0f)
    );
    
    int rval = rand() % 10000;

    constexpr string_view node1_name    = "Node 1";
    constexpr string_view node2_name    = "Node 2";
    constexpr string_view node3_name    = "Node 3";
    constexpr string_view node4_name    = "Node 4";

    Scene::Node     node1   (node1_name);
    Scene::Node     node2   ("No name");
    Scene::PtrNode  node3   (Scene::Node::make(node3_name));
    Scene::PtrNode  node4   (node2.addChild(node4_name));

    node2.setParent(&node1);
    node2.addChild(node3);
    node2.setWorldTransform(world_transform);
    node2.setLocalTransform(local_transform);
    node2.setWorldAABB(world_aabb);
    node2.setName(node2_name);

    EXPECT_EQ(&node1, node2.getParent()) << "Не правильно работает метод setParent(...) в классе Scene::Node." << endl;

    EXPECT_EQ(node4, node2.getChildren().front())   <<  "Не правильно работает метод addChild(const string_view).\n"
                                                        "Дочерний узел node4 должен быть первым." 
                                                    << endl;
    EXPECT_EQ(node4->getParent(), &node2)   << "Не правильно работает метод addChild(const string_view).\n"
                                                "Узел node2 должен быть родителем узла node4."
                                            << endl;

    EXPECT_EQ(node3, node2.getChild(1))     <<  "Не правильно работает метод addChild(const PtrNode&).\n"
                                                "Узел node3 должен быть вторым."
                                            << endl;

    EXPECT_EQ(node3->getParent(), &node2)   <<  "Не правильно работает метод addChild(const PtrNode&).\n"
                                                "Узел node2 должен быть родителем узла node3."
                                            << endl;

    EXPECT_EQ(node2.getWorldTransform().getMatrix(), world_transform.getMatrix())   << "Не правильно работает метод setWorldTransform(...)." << endl;
    EXPECT_TRUE(node2.worldTransformIsCurrent())                                    << "Не правильно работает метод setWorldTransform(...)." << endl;
    EXPECT_EQ(node2.getLocalTransform().getMatrix(), local_transform.getMatrix())   << "Не правильно работает метод setLocalTransform(...)." << endl;

    EXPECT_EQ(world_aabb, node2.getWorldAABB()) << "Не правильно работает метод setWorldAABB(...)." << endl;
    EXPECT_TRUE(node2.worldAABBIsCurrent())     << "Не правильно работает метод setWorldAABB(...)." << endl;

    EXPECT_EQ(node2_name, node2.getName()) << "Не правильно работает метод setName(...)." << endl;
    
    node2.addComponent(make_shared<TestNodeComponent>());
    node2.addScript(make_shared<TestNodeScript>());
    
    EXPECT_TRUE(node2.hasComponent<TestNodeComponent>())    << "Ошибка в addComponent(...) или в hasComponent()" << endl;
    EXPECT_TRUE(node2.hasScript<TestNodeScript>())          << "Ошибка в addScript(...) или в hasScript()" << endl;
    
    node2.getComponent<TestNodeComponent>().i = rval;
    node2.getScript<TestNodeScript>().update(&node2, rval);
    
    EXPECT_EQ(rval, node2.getComponent<TestNodeComponent>().i)  << "Ошибка в getComponent(...). Не правильно инициализирована переменная." << endl;
    EXPECT_EQ(rval, node2.getScript<TestNodeScript>().i)        << "Ошибка в getScript(...). Не правильно инициализирована переменная." << endl;
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

    Scene::Node     node1;
    Scene::PtrNode  node2 (node1.addChild("Node 2"));
    Scene::PtrNode  node3 (node1.addChild("Node 3"));
    Scene::PtrNode  node4 (node2->addChild("Node 4"));
    Scene::PtrNode  node5 (node3->addChild("Node 5"));

    node1.setLocalTransform(t1);
    node2->setLocalTransform(t2);
    node3->setLocalTransform(t3);

    node4->setWorldAABB(bbox1);
    node5->setWorldAABB(bbox2);

    node1.update(0.2f, Transform());

    EXPECT_EQ(t1.getMatrix(), node1.getLocalTransform().getMatrix())            << "Не правильно работает метод update(...)." << endl;
    EXPECT_EQ(t1.getMatrix(), node2->getWorldTransform().getMatrix())           << "Не правильно работает метод update(...)." << endl;
    EXPECT_EQ(t1.getMatrix(), node3->getWorldTransform().getMatrix())           << "Не правильно работает метод update(...)." << endl;
    EXPECT_EQ((t1 * t2).getMatrix(), node4->getWorldTransform().getMatrix())    << "Не правильно работает метод update(...)." << endl;
    EXPECT_EQ((t1 * t3).getMatrix(), node5->getWorldTransform().getMatrix())    << "Не правильно работает метод update(...)." << endl;

    EXPECT_EQ(bbox3, node1.getWorldAABB())  << "Не правильно работает метод update(...)." << endl;
    EXPECT_EQ(bbox1, node2->getWorldAABB()) << "Не правильно работает метод update(...)." << endl;
    EXPECT_EQ(bbox2, node3->getWorldAABB()) << "Не правильно работает метод update(...)." << endl;
    EXPECT_EQ(bbox1, node4->getWorldAABB()) << "Не правильно работает метод update(...)." << endl;
    EXPECT_EQ(bbox2, node5->getWorldAABB()) << "Не правильно работает метод update(...)." << endl;

    EXPECT_NE(nullptr, node1.getChild(0));
    EXPECT_NE(nullptr, node1.getChild(1));

    node1.detachChild("Node 2");

    EXPECT_EQ(nullptr, node1.getChild(0));
    EXPECT_NE(nullptr, node1.getChild(1));

    node1.detachChild(node3);

    EXPECT_EQ(nullptr, node1.getChild(0));
    EXPECT_EQ(nullptr, node1.getChild(1));
}
