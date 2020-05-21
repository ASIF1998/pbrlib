//
//  SceneTests.cpp
//  PBRLibTests
//
//  Created by Асиф Мамедов on 19/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <gtest/gtest.h>

#include "../../src/SceneGraph/Scene.hpp"
#include "../../src/SceneGraph/SpotLightNode.hpp"
#include "../../src/SceneGraph/PointLightNode.hpp"

using namespace testing;
using namespace pbrlib;

TEST(SceneGraphScene, Constructor)
{
    string_view name1 = "Scene";
    string_view name2 = "Scene 2";
    
    Scene scene1;
    Scene scene2(name2);

    EXPECT_EQ(nullptr, scene1.getRootNode()) << "При инициализации у объекта появился указатель на корневой узел (его не должно быть)." << endl;
    EXPECT_EQ(nullptr, scene2.getRootNode()) << "При инициализации у объекта появился указатель на корневой узел (его не должно быть)." << endl;

    EXPECT_EQ(name1, scene1.getName()) << "Не правильное инициализирование имени." << endl;
    EXPECT_EQ(name2, scene2.getName()) << "Не правильное инициализирование имени." << endl;
}

TEST(SceneGraphScene, GettersAndSetters)
{
    string_view name = "Scene 1";
    Scene scene;

    Scene::PtrNode ptr_node = Scene::Node::make();

    scene.setName(name);
    scene.setRootNode(ptr_node);
    
    EXPECT_EQ(name, scene.getName())            << "Ошибка в методе setName(...)." << endl;
    EXPECT_EQ(ptr_node, scene.getRootNode())    << "Ошибка в методе setRootNode(...)." << endl;
}

TEST(SceneGraphScene, UpdateTest)
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

    constexpr Vec3<float>   light_color     (1.0f, 0.0f, 0.0f);
    constexpr float         intensity       (100.0f);
    constexpr float         outer_radius    (50.0f);
    constexpr float         inner_radius    (40.0f);
    constexpr Vec3<float>   light_pos       (25.0f, -45.0f, -30.0f);
    constexpr Vec3<float>   dir_light       (1.0f, 0.0f, 0.0f);
    
    SpotLight::Builder slight_builder;
    PointLight::Builder plight_builder;

    slight_builder.setColor(light_color);
    slight_builder.setIntensity(intensity);
    slight_builder.setPosition(light_pos);
    slight_builder.setOuterRadius(outer_radius);
    slight_builder.setInnerRadius(inner_radius);
    slight_builder.setDirection(dir_light);
    
    plight_builder.setColor(light_color);
    plight_builder.setPosition(light_pos);
    plight_builder.setIntensity(intensity);

    Scene scene;

    PtrSpotLightNode    node1 = scene.makeSpotLight(slight_builder, "Node 1");
    PtrPointLightNode   node2 = scene.makePointLight(plight_builder, "Node 2");
    PtrSpotLightNode    node3 = scene.makeSpotLight(slight_builder, "Node 3");
    PtrSpotLightNode    node4 = scene.makeSpotLight(slight_builder, "Node 4");
    PtrPointLightNode   node5 = scene.makePointLight(plight_builder, "Node 5");
    
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

    scene.setRootNode(node1);
    scene.update(0.2);

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
