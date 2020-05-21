//
//  PointLightNodeTests.cpp
//  PBRLibTests
//
//  Created by Асиф Мамедов on 19/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <ctime>

#include <gtest/gtest.h>

#include "../../src/SceneGraph/PointLightNode.hpp"

using namespace testing;
using namespace pbrlib;
using namespace std;

struct TestPointLightNodeModifier :
    public IPointLightNodeModifier
{
    TestPointLightNodeModifier() :
        IPointLightNodeModifier (),
        i                       (0)
    {}

    virtual void update(PointLightNode* ptr_node, float delta_time) override
    {}

    virtual type_index  getType() const override
    {
        return INodeModifier::getTypeIndex<TestPointLightNodeModifier>();
    }

    int i;
};

TEST(SceneGraphPointLightNode, Constructor)
{
    constexpr string_view   name1   = "Point Light Node";
    const string            name2   = "Node 2";
    constexpr char          name3[] = "Name 3";

    constexpr Vec3<float>   light_color (1.0f, 0.0f, 0.0f);
    constexpr float         intensity   (100.0f);
    constexpr Vec3<float>   light_pos   (25.0f, -45.0f, -30.0f);

    AABB raabb;

    PointLight::Builder light_builder;

    light_builder.setColor(light_color);
    light_builder.setIntensity(intensity);
    light_builder.setPosition(light_pos);

    PtrPointLight ptr_point_light = light_builder.buildPtr();

    PointLightNode      node1;
    PointLightNode      node2(name2, nullptr, light_builder);
    PtrPointLightNode   node3 = PointLightNode::make(name3, &node2, ptr_point_light);

    constexpr Matrix4x4<float> rm;

    EXPECT_FALSE(node1.worldAABBIsCurrent())    << "При инициализации мировой ограничивающий объём должен быть не актуальным." << endl;
    EXPECT_FALSE(node2.worldAABBIsCurrent())    << "При инициализации мировой ограничивающий объём должен быть не актуальным." << endl;
    EXPECT_FALSE(node3->worldAABBIsCurrent())   << "При инициализации мировой ограничивающий объём должен быть не актуальным." << endl;

    EXPECT_FALSE(node1.worldTransformIsCurrent())   << "При инициализации мировое преобразование должно быть не актуальным." << endl;
    EXPECT_FALSE(node2.worldTransformIsCurrent())   << "При инициализации мировое преобразование должно быть не актуальным." << endl;
    EXPECT_FALSE(node3->worldTransformIsCurrent())  << "При инициализации мировое преобразование должно быть не актуальным." << endl;

    EXPECT_EQ(raabb, node1.getWorldAABB())  << "Не правильная инициализация мирового ограничивающего объёма." << endl; 
    EXPECT_EQ(raabb, node2.getWorldAABB())  << "Не правильная инициализация мирового ограничивающего объёма." << endl; 
    EXPECT_EQ(raabb, node3->getWorldAABB()) << "Не правильная инициализация мирового ограничивающего объёма." << endl; 

    EXPECT_EQ(nullptr, node1.getParent())   << "При инициализации у объекта появился указатель на родителя (его не должно быть)." << endl;
    EXPECT_EQ(nullptr, node2.getParent())   << "При инициализации у объекта появился указатель на родителя (его не должно быть)." << endl;
    EXPECT_EQ(&node2, node3->getParent())   << "При инициализации появился не корректный указатель на родителя." << endl;

    EXPECT_TRUE(node1.getChildren().empty())    << "При инициализирование появились дочерние узлы." << endl;
    EXPECT_TRUE(node2.getChildren().empty())    << "При инициализирование появились дочерние узлы." << endl;
    EXPECT_TRUE(node3->getChildren().empty())   << "При инициализирование появились дочерние узлы." << endl;

    EXPECT_EQ(rm, node1.getWorldTransform().getMatrix())    << "Не правильное инициализирование мирового преобразования." << endl;
    EXPECT_EQ(rm, node2.getWorldTransform().getMatrix())    << "Не правильное инициализирование мирового преобразования." << endl;
    EXPECT_EQ(rm, node3->getWorldTransform().getMatrix())   << "Не правильное инициализирование мирового преобразования." << endl;

    EXPECT_EQ(rm, node1.getLocalTransform().getMatrix()) << "Не правильное инициализирование локального преобразования." << endl;
    EXPECT_EQ(rm, node2.getLocalTransform().getMatrix()) << "Не правильное инициализирование локального преобразования." << endl;
    EXPECT_EQ(rm, node3->getLocalTransform().getMatrix()) << "Не правильное инициализирование локального преобразования." << endl;

    EXPECT_EQ(name1, node1.getName())   << "Не правильное инициализирование имени." << endl;
    EXPECT_EQ(name2, node2.getName())   << "Не правильное инициализирование имени." << endl;
    EXPECT_EQ(name3, node3->getName())  << "Не правильное инициализирование имени." << endl;
    
    EXPECT_EQ(nullptr, node1.getLight())            << "Не правильное инициализирование указателя на источник света." << endl;
    EXPECT_NE(nullptr, node2.getLight())            << "Не правильное инициализирование указателя на источник света." << endl;
    EXPECT_EQ(ptr_point_light, node3->getLight())   << "Не правильное инициализирование указателя на источник света." << endl;

    EXPECT_EQ(light_pos, node2.getLight()->getPosition())   << "Не правильное инициализирование источника света." << endl;
    EXPECT_EQ(intensity, node2.getLight()->getIntensity())  << "Не правильное инициализирование источника света." << endl;
    EXPECT_EQ(light_color, node2.getLight()->getColor())    << "Не правильное инициализирование источника света." << endl;

    EXPECT_EQ(light_pos, node3->getLight()->getPosition())  << "Не правильное инициализирование источника света." << endl;
    EXPECT_EQ(intensity, node3->getLight()->getIntensity()) << "Не правильное инициализирование источника света." << endl;
    EXPECT_EQ(light_color, node3->getLight()->getColor())   << "Не правильное инициализирование источника света." << endl;
}

TEST(SceneGraphPointLightNode, GettersAndSetters)
{
    srand(static_cast<unsigned>(time(nullptr)));

    constexpr Vec3<float>   light_color (1.0f, 0.0f, 0.0f);
    constexpr float         intensity   (100.0f);
    constexpr Vec3<float>   light_pos   (25.0f, -45.0f, -30.0f);

    const int init_value = rand();
    
    PointLight::Builder light_builder;

    light_builder.setColor(light_color);
    light_builder.setIntensity(intensity);
    light_builder.setPosition(light_pos);

    PtrPointLight ptr_point_light = light_builder.buildPtr();

    PointLightNode      node1;
    PtrPointLightNode   node2 = PointLightNode::make();

    node1.setLight(light_builder);
    node2->setLight(ptr_point_light);

    EXPECT_EQ(light_pos, node1.getLight()->getPosition())   << "Ошибка в методе setLight(...)." << endl;
    EXPECT_EQ(intensity, node1.getLight()->getIntensity())  << "Ошибка в методе setLight(...)." << endl;
    EXPECT_EQ(light_color, node1.getLight()->getColor())    << "Ошибка в методе setLight(...)." << endl;

    EXPECT_EQ(light_pos, node2->getLight()->getPosition())  << "Ошибка в методе setLight(...)." << endl;
    EXPECT_EQ(intensity, node2->getLight()->getIntensity()) << "Ошибка в методе setLight(...)." << endl;
    EXPECT_EQ(light_color, node2->getLight()->getColor())   << "Ошибка в методе setLight(...)." << endl;

    EXPECT_FALSE(node1.hasPointLightNodeModifier<TestPointLightNodeModifier>()) << "Не правильное инициализирование модификаторов." << endl;

    {
        TestPointLightNodeModifier* test_point_light_node_modifier = new TestPointLightNodeModifier;

        test_point_light_node_modifier->i = init_value;
        node1.addPointLightNodeModifier(test_point_light_node_modifier);
    }

    EXPECT_TRUE(node1.hasPointLightNodeModifier<TestPointLightNodeModifier>())              << "Ошибка в методе addPointLightNodeModifier(...)." << endl;
    EXPECT_EQ(init_value, node1.getPointLightNodeModifier<TestPointLightNodeModifier>().i)  << "Ошибка в методе addPointLightNodeModifier(...)." << endl;
}

TEST(SceneGraphPointLightNode, UpdateTest)
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
    
    constexpr Vec3<float>   light_color (1.0f, 0.0f, 0.0f);
    constexpr float         intensity   (100.0f);
    constexpr Vec3<float>   light_pos   (25.0f, -45.0f, -30.0f);
    
    PointLight::Builder light_builder;

    light_builder.setColor(light_color);
    light_builder.setIntensity(intensity);
    light_builder.setPosition(light_pos);
    
    Scene scene;

    PtrPointLightNode node1 = scene.makePointLight(light_builder, "Node 1");
    PtrPointLightNode node2 = scene.makePointLight(light_builder, "Node 2");
    PtrPointLightNode node3 = scene.makePointLight(light_builder, "Node 3");
    PtrPointLightNode node4 = scene.makePointLight(light_builder, "Node 4");
    PtrPointLightNode node5 = scene.makePointLight(light_builder, "Node 5");

    node1->addChild(node2);
    node1->addChild(node3);
    node2->addChild(node4);
    node3->addChild(node5);

    node2->setParent(node1.get());
    node3->setParent(node1.get());
    node4->setParent(node2.get());
    node5->setParent(node3.get());

    node1->setLocalTransform(t1);
    node2->setLocalTransform(t2);
    node3->setLocalTransform(t3);

    node4->setWorldAABB(bbox1);
    node5->setWorldAABB(bbox2);

    node1->update(0.2f, Transform());

    EXPECT_EQ(t1.getMatrix(), node1->getLocalTransform().getMatrix())           << "Не правильно работает метод update(...)." << endl;
    EXPECT_EQ(t1.getMatrix(), node2->getWorldTransform().getMatrix())           << "Не правильно работает метод update(...)." << endl;
    EXPECT_EQ(t1.getMatrix(), node3->getWorldTransform().getMatrix())           << "Не правильно работает метод update(...)." << endl;
    EXPECT_EQ((t1 * t2).getMatrix(), node4->getWorldTransform().getMatrix())    << "Не правильно работает метод update(...)." << endl;
    EXPECT_EQ((t1 * t3).getMatrix(), node5->getWorldTransform().getMatrix())    << "Не правильно работает метод update(...)." << endl;

    EXPECT_EQ(bbox3, node1->getWorldAABB()) << "Не правильно работает метод update(...)." << endl;
    EXPECT_EQ(bbox1, node2->getWorldAABB()) << "Не правильно работает метод update(...)." << endl;
    EXPECT_EQ(bbox2, node3->getWorldAABB()) << "Не правильно работает метод update(...)." << endl;
    EXPECT_EQ(bbox1, node4->getWorldAABB()) << "Не правильно работает метод update(...)." << endl;
    EXPECT_EQ(bbox2, node5->getWorldAABB()) << "Не правильно работает метод update(...)." << endl;
}
