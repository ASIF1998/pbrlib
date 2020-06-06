//
//  SceneTests.cpp
//  PBRLibTests
//
//  Created by Асиф Мамедов on 19/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <gtest/gtest.h>

#include "../../src/SceneGraph/Scene.hpp" 
#include "../../src/SceneGraph/SceneView.hpp"

#include "../../src/Rendering/Window.hpp"

using namespace testing;
using namespace pbrlib;

TEST(SceneGraphScene, Constructor)
{


    string_view name1 = "Scene";
    string_view name2 = "Scene 2";

    Window window ("Testing", 800, 600, Window::WINDOW_POSITION_CENTERED, Window::WINDOW_POSITION_CENTERED);

    SceneView scene_view1 (name1, window);
    SceneView scene_view2 (name2, window);

    Scene& scene1 = scene_view1.getScene();
    Scene& scene2 = scene_view2.getScene();

    EXPECT_EQ(nullptr, scene1.getRootNode()) << "При инициализации у объекта появился указатель на корневой узел (его не должно быть)." << endl;
    EXPECT_EQ(nullptr, scene2.getRootNode()) << "При инициализации у объекта появился указатель на корневой узел (его не должно быть)." << endl;

    EXPECT_EQ(name1, scene1.getName()) << "Не правильное инициализирование имени." << endl;
    EXPECT_EQ(name2, scene2.getName()) << "Не правильное инициализирование имени." << endl;
}

TEST(SceneGraphScene, GettersAndSetters)
{
    string_view name = "Scene 1";

    Window window ("Testing", 800, 600, Window::WINDOW_POSITION_CENTERED, Window::WINDOW_POSITION_CENTERED);
    
    SceneView scene_view (name, window);

    Scene& scene = scene_view.getScene();

    Scene::PtrNode ptr_node = Scene::Node::make();

    scene.setName(name);
    scene.setRootNode(ptr_node);
    
    EXPECT_EQ(name, scene.getName())            << "Ошибка в методе setName(...)." << endl;
    EXPECT_EQ(ptr_node, scene.getRootNode())    << "Ошибка в методе setRootNode(...)." << endl;
}

TEST(SceneGraphScene, UpdateAndCompanentTest)
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
    
    constexpr float width   = 800.0f;
    constexpr float height  = 600.0f;

    constexpr float near    = 0.01f;
    constexpr float far     = 100.0f;
    constexpr float fovy    = 45.0f;

    constexpr Vec3<float> eye   (0.0f, 0.0f, -1.0f);
    constexpr Vec3<float> pos   (0.0f, 0.0f, 0.0f);
    constexpr Vec3<float> up    (0.0f, 1.0f, 0.0f);
    
    constexpr Viewport viewport {
        .x          = width / 2.0f,
        .y          = height / 2.0f,
        .width      = width,
        .height     = height,
        .minDepth   = 0.0f,
        .maxDepth   = 1.0f
    };

    PerspectiveCamera::Builder  camera_builder;
    SpotLight::Builder          slight_builder;
    PointLight::Builder         plight_builder;

    camera_builder.setAspect(width, height);
    camera_builder.setEye(eye);
    camera_builder.setNearClipp(near);
    camera_builder.setFarClipp(far);
    camera_builder.setPosition(pos);
    camera_builder.setUp(up);
    camera_builder.setFovy(fovy);
    camera_builder.setViewport(viewport);
    camera_builder.setName("Camera Buidler");
    slight_builder.setColor(light_color);
    slight_builder.setIntensity(intensity);
    slight_builder.setPosition(light_pos);
    slight_builder.setOuterRadius(outer_radius);
    slight_builder.setInnerRadius(inner_radius);
    slight_builder.setDirection(dir_light);
    slight_builder.setName("Spot Light Builder");
    
    plight_builder.setColor(light_color);
    plight_builder.setPosition(light_pos);
    plight_builder.setIntensity(intensity);
    plight_builder.setName("Point Light Builder");

    Window window ("Testing", 800, 600, Window::WINDOW_POSITION_CENTERED, Window::WINDOW_POSITION_CENTERED);
    
    SceneView scene_view ("Scene", window);

    Scene& scene = scene_view.getScene();

    Scene::PtrNode node1 = scene.makeSpotLight(slight_builder, "Node 1");
    Scene::PtrNode node2 = scene.makePointLight(plight_builder, "Node 2");
    Scene::PtrNode node3 = scene.makeSpotLight(slight_builder, "Node 3");
    Scene::PtrNode node4 = scene.makeSpotLight(slight_builder, "Node 4");
    Scene::PtrNode node5 = scene.makePointLight(plight_builder, "Node 5");
    Scene::PtrNode node6 = scene.makeCamera(camera_builder);
    
    node1->addChild(node2);
    node1->addChild(node3);
    node2->addChild(node4);
    node3->addChild(node5);

    node1->setLocalTransform(t1);
    node2->setLocalTransform(t2);
    node3->setLocalTransform(t3);

    node4->setWorldAABB(bbox1);
    node5->setWorldAABB(bbox2);

    scene.setRootNode(node1);
    scene.update(0.2);
    
    EXPECT_TRUE(node1->hasComponent<SpotLight>())   << "Не правильно работает метод makeSpotLight(...)." << endl;
    EXPECT_TRUE(node2->hasComponent<PointLight>())  << "Не правильно работает метод makePointLight(...)." << endl;
    EXPECT_TRUE(node3->hasComponent<SpotLight>())   << "Не правильно работает метод makeSpotLight(...)." << endl;
    EXPECT_TRUE(node4->hasComponent<SpotLight>())   << "Не правильно работает метод makeSpotLight(...)." << endl;
    EXPECT_TRUE(node5->hasComponent<PointLight>())  << "Не правильно работает метод makePointLight(...)." << endl;
    EXPECT_TRUE(node6->hasComponent<CameraBase>())  << "Не правильно работает метод makeCamera(...)." << endl;
    
    EXPECT_EQ("Spot Light Builder", node1->getComponent<SpotLight>().getName());
    EXPECT_EQ("Point Light Builder", node2->getComponent<PointLight>().getName());
    EXPECT_EQ("Camera Buidler", node6->getComponent<CameraBase>().getName());

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
