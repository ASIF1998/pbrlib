//
//  SceneTests.cpp
//  PBRLibTests
//
//  Created by Асиф Мамедов on 19/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#if 0

#include "../utils.hpp"

#include <pbrlib/PBRLibResources.hpp>
#include <pbrlib/SceneGraph/Scene.hpp>
#include <pbrlib/SceneGraph/SceneView.hpp>
#include <pbrlib/Rendering/Window.hpp>

using namespace pbrlib;

TEST(SceneGraphScene, Constructor)
{
    string_view name1 = "Scene";
    string_view name2 = "Scene 2";
    
    Window::Builder window_builder;
    
    window_builder.setTitle("Testing");
    window_builder.setExtend(800, 600);
    window_builder.setPosition(Window::WINDOW_POSITION_CENTERED, Window::WINDOW_POSITION_CENTERED);

    std::shared_ptr ptr_pbrlib_resources    = PBRLibResources::init();
    std::shared_ptr ptr_window              = window_builder.buildPtr();
    
    SceneView scene_view1 (name1, ptr_pbrlib_resources, ptr_window);
    SceneView scene_view2 (name2, ptr_pbrlib_resources, ptr_window);

    Scene& scene1 = scene_view1.getScene();
    Scene& scene2 = scene_view2.getScene();

    EXPECT_TRUE(scene1.getRoot() == nullptr) << "При инициализации у объекта появился указатель на корневой узел (его не должно быть).";
    EXPECT_TRUE(scene2.getRoot() == nullptr) << "При инициализации у объекта появился указатель на корневой узел (его не должно быть).";

    pbrlib::testing::utils::equality(string(name1), scene1.getName(), "Не правильное инициализирование имени.");
    pbrlib::testing::utils::equality(string(name2), scene2.getName(), "Не правильное инициализирование имени.");
}

TEST(SceneGraphScene, GettersAndSetters)
{
    string_view name = "Scene 1";

    Window::Builder window_builder;
    
    window_builder.setTitle("Testing");
    window_builder.setExtend(800, 600);
    window_builder.setPosition(Window::WINDOW_POSITION_CENTERED, Window::WINDOW_POSITION_CENTERED);

    std::shared_ptr ptr_pbrlib_resources    = PBRLibResources::init();
    std::shared_ptr ptr_window              = window_builder.buildPtr();
    
    SceneView scene_view (name, ptr_pbrlib_resources, ptr_window);

    Scene& scene = scene_view.getScene();

    std::shared_ptr ptr_node = SceneItem::make();

    scene.setName(name);
    scene.setRoot(ptr_node);
    
    pbrlib::testing::utils::equality(string(name), scene.getName(), "Ошибка в методе setName(...).");
    pbrlib::testing::utils::equality(ptr_node, scene.getRoot(), "Ошибка в методе setRoot(...).");
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

    constexpr Vec3<float> eye   (0.0f, 0.0f, -1.0f);
    constexpr Vec3<float> pos   (0.0f, 0.0f, 0.0f);
    constexpr Vec3<float> up    (0.0f, 1.0f, 0.0f);

    Viewport viewport = { };
    viewport.x          = width / 2.0f;
    viewport.y          = height / 2.0f;
    viewport.width      = width;
    viewport.height     = height;
    viewport.minDepth   = 0.0f;
    viewport.maxDepth   = 1.0f;

    PerspectiveCamera::Builder  camera_builder;
    SpotLight::Builder          slight_builder;
    PointLight::Builder         plight_builder;

    camera_builder.setAspect(width, height);
    camera_builder.setEye(eye);
    camera_builder.setNearClipp(0.01f);
    camera_builder.setFarClipp(100.0f);
    camera_builder.setPosition(pos);
    camera_builder.setUp(up);
    camera_builder.setFovy(45.0f);
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

    Window::Builder window_builder;
    
    window_builder.setTitle("Testing");
    window_builder.setExtend(800, 600);
    window_builder.setPosition(Window::WINDOW_POSITION_CENTERED, Window::WINDOW_POSITION_CENTERED);

    std::shared_ptr ptr_pbrlib_resources    = PBRLibResources::init();
    std::shared_ptr ptr_window              = window_builder.buildPtr();
    
    SceneView scene_view ("Scene", ptr_pbrlib_resources, ptr_window);

    Scene& scene = scene_view.getScene();

    std::shared_ptr node1 = scene.makeSpotLight(slight_builder, "Node 1");
    std::shared_ptr node2 = scene.makePointLight(plight_builder, "Node 2");
    std::shared_ptr node3 = scene.makeSpotLight(slight_builder, "Node 3");
    std::shared_ptr node4 = scene.makeSpotLight(slight_builder, "Node 4");
    std::shared_ptr node5 = scene.makePointLight(plight_builder, "Node 5");
    std::shared_ptr node6 = scene.makeCamera(camera_builder);
    
    node1->addChild(node2);
    node1->addChild(node3);
    node2->addChild(node4);
    node3->addChild(node5);

    node1->setLocalTransform(t1);
    node2->setLocalTransform(t2);
    node3->setLocalTransform(t3);

    node4->setWorldAABB(bbox1);
    node5->setWorldAABB(bbox2);

    scene.setRoot(node1);
    scene.update(nullptr, 0.2f);
    
    pbrlib::testing::utils::thisTrue(node1->hasComponent<SpotLight>(), "Не правильно работает метод makeSpotLight(...).");
    pbrlib::testing::utils::thisTrue(node2->hasComponent<PointLight>(), "Не правильно работает метод makePointLight(...).");
    pbrlib::testing::utils::thisTrue(node3->hasComponent<SpotLight>(), "Не правильно работает метод makeSpotLight(...).");
    pbrlib::testing::utils::thisTrue(node4->hasComponent<SpotLight>(), "Не правильно работает метод makeSpotLight(...).");
    pbrlib::testing::utils::thisTrue(node5->hasComponent<PointLight>(), "Не правильно работает метод makePointLight(...).");
    pbrlib::testing::utils::thisTrue(node6->hasComponent<CameraBase>(), "Не правильно работает метод makeCamera(...).");
    
    pbrlib::testing::utils::equality(string("Spot Light Builder"), node1->getComponent<SpotLight>().getName());
    pbrlib::testing::utils::equality(string("Point Light Builder"), node2->getComponent<PointLight>().getName());
    pbrlib::testing::utils::equality(string("Camera Buidler"), node6->getComponent<CameraBase>().getName());

    pbrlib::testing::utils::equality(t1.getMatrix(), node1->getLocalTransform().getMatrix(), "Не правильно работает метод update(...).");
    pbrlib::testing::utils::equality(t1.getMatrix(), node2->getWorldTransform().getMatrix(), "Не правильно работает метод update(...).");
    pbrlib::testing::utils::equality(t1.getMatrix(), node3->getWorldTransform().getMatrix(), "Не правильно работает метод update(...).");
    pbrlib::testing::utils::equality((t1 * t2).getMatrix(), node4->getWorldTransform().getMatrix(), "Не правильно работает метод update(...).");
    pbrlib::testing::utils::equality((t1 * t3).getMatrix(), node5->getWorldTransform().getMatrix(), "Не правильно работает метод update(...).");

    pbrlib::testing::utils::equality(bbox3[0], node1->getWorldAABB()[0], "Не правильно работает метод update(...).");
    pbrlib::testing::utils::equality(bbox3[1], node1->getWorldAABB()[1], "Не правильно работает метод update(...).");

    pbrlib::testing::utils::equality(bbox1[0], node2->getWorldAABB()[0], "Не правильно работает метод update(...).");
    pbrlib::testing::utils::equality(bbox1[1], node2->getWorldAABB()[1], "Не правильно работает метод update(...).");
    
    pbrlib::testing::utils::equality(bbox2[0], node3->getWorldAABB()[0], "Не правильно работает метод update(...).");
    pbrlib::testing::utils::equality(bbox2[1], node3->getWorldAABB()[1], "Не правильно работает метод update(...).");

    pbrlib::testing::utils::equality(bbox1[0], node4->getWorldAABB()[0], "Не правильно работает метод update(...).");
    pbrlib::testing::utils::equality(bbox1[1], node4->getWorldAABB()[1], "Не правильно работает метод update(...).");

    pbrlib::testing::utils::equality(bbox2[0], node5->getWorldAABB()[0], "Не правильно работает метод update(...).");
    pbrlib::testing::utils::equality(bbox2[1], node5->getWorldAABB()[1], "Не правильно работает метод update(...).");
}

#endif