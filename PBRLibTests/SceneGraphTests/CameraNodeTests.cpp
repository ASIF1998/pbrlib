//
//  CameraNodeTests.cpp
//  PBRLibTests
//
//  Created by Асиф Мамедов on 19/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <ctime>

#include <gtest/gtest.h>

#include "../../src/SceneGraph/CameraNode.hpp"

using namespace testing;
using namespace pbrlib;
using namespace std;

struct TestCameraNodeModifier :
    public ICameraNodeModifier
{
    TestCameraNodeModifier() :
        ICameraNodeModifier (),
        i                   (0)
    {}

    virtual void update(CameraNode* ptr_node, float delta_time) override
    {}

    virtual type_index  getType() const override
    {
        return INodeModifier::getTypeIndex<TestCameraNodeModifier>();
    }

    int i;
};

TEST(SceneGraphCameraNode, Constructor)
{
    constexpr string_view   name1   = "Camera Node";
    const string            name2   = "Node 2";

    constexpr float width   = 800.0f;
    constexpr float height  = 600.0f;

    constexpr float near    = 0.01f;
    constexpr float far     = 100.0f;
    constexpr float fovy    = 45.0f;

    constexpr Vec3<float> eye   (0.0f, 0.0f, -1.0f);
    constexpr Vec3<float> pos   (0.0f, 0.0f, 0.0f);
    constexpr Vec3<float> up    (0.0f, 1.0f, 0.0f);

    Transform r1 = Transform::lookAt(pos, eye, up);
    Transform r2 = Transform::perspective(fovy, width / height, near, far);
    
    constexpr Viewport viewport {
        .x          = width / 2.0f,
        .y          = height / 2.0f,
        .width      = width,
        .height     = height,
        .minDepth   = 0.0f,
        .maxDepth   = 1.0f
    };

    PerspectiveCamera::Builder camera_builder;

    camera_builder.setAspect(width, height);
    camera_builder.setEye(eye);
    camera_builder.setNearClipp(near);
    camera_builder.setFarClipp(far);
    camera_builder.setPosition(pos);
    camera_builder.setUp(up);
    camera_builder.setFovy(fovy);
    camera_builder.setViewport(viewport);

    PtrICamera ptr_camera = camera_builder.ptrBuild();

    AABB raabb;

    CameraNode      node1;
    CameraNode      node2(name2, camera_builder);

    constexpr Matrix4x4<float> rm;

    EXPECT_FALSE(node1.worldAABBIsCurrent())    << "При инициализации мировой ограничивающий объём должен быть не актуальным." << endl;
    EXPECT_FALSE(node2.worldAABBIsCurrent())    << "При инициализации мировой ограничивающий объём должен быть не актуальным." << endl;

    EXPECT_FALSE(node1.worldTransformIsCurrent())   << "При инициализации мировое преобразование должно быть не актуальным." << endl;
    EXPECT_FALSE(node2.worldTransformIsCurrent())   << "При инициализации мировое преобразование должно быть не актуальным." << endl;

    EXPECT_EQ(raabb, node1.getWorldAABB())  << "Не правильная инициализация мирового ограничивающего объёма." << endl;
    EXPECT_EQ(raabb, node2.getWorldAABB())  << "Не правильная инициализация мирового ограничивающего объёма." << endl;

    EXPECT_EQ(nullptr, node1.getParent())   << "При инициализации у объекта появился указатель на родителя (его не должно быть)." << endl;
    EXPECT_EQ(nullptr, node2.getParent())   << "При инициализации у объекта появился указатель на родителя (его не должно быть)." << endl;

    EXPECT_TRUE(node1.getChildren().empty())    << "При инициализирование появились дочерние узлы." << endl;
    EXPECT_TRUE(node2.getChildren().empty())    << "При инициализирование появились дочерние узлы." << endl;

    EXPECT_EQ(rm, node1.getWorldTransform().getMatrix())    << "Не правильное инициализирование мирового преобразования." << endl;
    EXPECT_EQ(rm, node2.getWorldTransform().getMatrix())    << "Не правильное инициализирование мирового преобразования." << endl;

    EXPECT_EQ(rm, node1.getLocalTransform().getMatrix())    << "Не правильное инициализирование локального преобразования." << endl;
    EXPECT_EQ(rm, node2.getLocalTransform().getMatrix())    << "Не правильное инициализирование локального преобразования." << endl;

    EXPECT_EQ(name1, node1.getName())   << "Не правильное инициализирование имени." << endl;
    EXPECT_EQ(name2, node2.getName())   << "Не правильное инициализирование имени." << endl;
    
    EXPECT_EQ(nullptr, node1.getCamera())       << "Не правильное инициализирование указателя на источник света." << endl;
    EXPECT_NE(nullptr, node2.getCamera())       << "Не правильное инициализирование указателя на источник света." << endl;

    EXPECT_EQ(near, node2.getCamera()->getNearClipp())          << "Не правильное инициализирование камеры." << endl;
    EXPECT_EQ(far, node2.getCamera()->getFarClipp())            << "Не правильное инициализирование камеры." << endl;
    EXPECT_EQ(eye, node2.getCamera()->getDirection())           << "Не правильное инициализирование камеры." << endl;
    EXPECT_EQ(pos, node2.getCamera()->getPosition())            << "Не правильное инициализирование камеры." << endl;
    EXPECT_EQ(r1, node2.getCamera()->getView())                 << "Не правильное инициализирование камеры." << endl;
    EXPECT_EQ(r2, node2.getCamera()->getProjection())           << "Не правильное инициализирование камеры." << endl;
    EXPECT_EQ(r2 * r1, node2.getCamera()->getViewProjection())  << "Не правильное инициализирование камеры." << endl;
    
    EXPECT_EQ(viewport.x, node2.getCamera()->getViewport().x)               << "Не правильное инициализирование камеры." << endl;
    EXPECT_EQ(viewport.y, node2.getCamera()->getViewport().y)               << "Не правильное инициализирование камеры." << endl;
    EXPECT_EQ(viewport.width, node2.getCamera()->getViewport().width)       << "Не правильное инициализирование камеры." << endl;
    EXPECT_EQ(viewport.height, node2.getCamera()->getViewport().height)     << "Не правильное инициализирование камеры." << endl;
    EXPECT_EQ(viewport.minDepth, node2.getCamera()->getViewport().minDepth) << "Не правильное инициализирование камеры." << endl;
    EXPECT_EQ(viewport.maxDepth, node2.getCamera()->getViewport().maxDepth) << "Не правильное инициализирование камеры." << endl;
}

TEST(SceneGraphCameraNode, GettersAndSetters)
{
    srand(static_cast<unsigned>(time(nullptr)));

    const int init_value = rand();

    constexpr float width   = 800.0f;
    constexpr float height  = 600.0f;

    constexpr float near    = 0.01f;
    constexpr float far     = 100.0f;
    constexpr float fovy    = 45.0f;

    constexpr Vec3<float> eye   (0.0f, 0.0f, -1.0f);
    constexpr Vec3<float> pos   (0.0f, 0.0f, 0.0f);
    constexpr Vec3<float> up    (0.0f, 1.0f, 0.0f);

    Transform r1 = Transform::lookAt(pos, eye, up);
    Transform r2 = Transform::perspective(fovy, width / height, near, far);
    
    constexpr Viewport viewport {
        .x          = width / 2.0f,
        .y          = height / 2.0f,
        .width      = width,
        .height     = height,
        .minDepth   = 0.0f,
        .maxDepth   = 1.0f
    };

    PerspectiveCamera::Builder camera_builder;

    camera_builder.setAspect(width, height);
    camera_builder.setEye(eye);
    camera_builder.setNearClipp(near);
    camera_builder.setFarClipp(far);
    camera_builder.setPosition(pos);
    camera_builder.setUp(up);
    camera_builder.setFovy(fovy);
    camera_builder.setViewport(viewport);

    PtrICamera ptr_camera = camera_builder.ptrBuild();

    CameraNode      node1;
    PtrCameraNode   node2 = CameraNode::make();

    node1.setCamera(camera_builder);
    node2->setCamera(ptr_camera);

    EXPECT_EQ(near, node1.getCamera()->getNearClipp())          << "Не правильное инициализирование камеры." << endl;
    EXPECT_EQ(far, node1.getCamera()->getFarClipp())            << "Не правильное инициализирование камеры." << endl;
    EXPECT_EQ(eye, node1.getCamera()->getDirection())           << "Не правильное инициализирование камеры." << endl;
    EXPECT_EQ(pos, node1.getCamera()->getPosition())            << "Не правильное инициализирование камеры." << endl;
    EXPECT_EQ(r1, node1.getCamera()->getView())                 << "Не правильное инициализирование камеры." << endl;
    EXPECT_EQ(r2, node1.getCamera()->getProjection())           << "Не правильное инициализирование камеры." << endl;
    EXPECT_EQ(r2 * r1, node1.getCamera()->getViewProjection())  << "Не правильное инициализирование камеры." << endl;
    
    EXPECT_EQ(viewport.x, node1.getCamera()->getViewport().x)               << "Не правильное инициализирование камеры." << endl;
    EXPECT_EQ(viewport.y, node1.getCamera()->getViewport().y)               << "Не правильное инициализирование камеры." << endl;
    EXPECT_EQ(viewport.width, node1.getCamera()->getViewport().width)       << "Не правильное инициализирование камеры." << endl;
    EXPECT_EQ(viewport.height, node1.getCamera()->getViewport().height)     << "Не правильное инициализирование камеры." << endl;
    EXPECT_EQ(viewport.minDepth, node1.getCamera()->getViewport().minDepth) << "Не правильное инициализирование камеры." << endl;
    EXPECT_EQ(viewport.maxDepth, node1.getCamera()->getViewport().maxDepth) << "Не правильное инициализирование камеры." << endl;

    EXPECT_EQ(near, node2->getCamera()->getNearClipp())          << "Не правильное инициализирование камеры." << endl;
    EXPECT_EQ(far, node2->getCamera()->getFarClipp())            << "Не правильное инициализирование камеры." << endl;
    EXPECT_EQ(eye, node2->getCamera()->getDirection())           << "Не правильное инициализирование камеры." << endl;
    EXPECT_EQ(pos, node2->getCamera()->getPosition())            << "Не правильное инициализирование камеры." << endl;
    EXPECT_EQ(r1, node2->getCamera()->getView())                 << "Не правильное инициализирование камеры." << endl;
    EXPECT_EQ(r2, node2->getCamera()->getProjection())           << "Не правильное инициализирование камеры." << endl;
    EXPECT_EQ(r2 * r1, node2->getCamera()->getViewProjection())  << "Не правильное инициализирование камеры." << endl;
    
    EXPECT_EQ(viewport.x, node2->getCamera()->getViewport().x)               << "Не правильное инициализирование камеры." << endl;
    EXPECT_EQ(viewport.y, node2->getCamera()->getViewport().y)               << "Не правильное инициализирование камеры." << endl;
    EXPECT_EQ(viewport.width, node2->getCamera()->getViewport().width)       << "Не правильное инициализирование камеры." << endl;
    EXPECT_EQ(viewport.height, node2->getCamera()->getViewport().height)     << "Не правильное инициализирование камеры." << endl;
    EXPECT_EQ(viewport.minDepth, node2->getCamera()->getViewport().minDepth) << "Не правильное инициализирование камеры." << endl;
    EXPECT_EQ(viewport.maxDepth, node2->getCamera()->getViewport().maxDepth) << "Не правильное инициализирование камеры." << endl;

    EXPECT_FALSE(node1.hasCameraNodeModifier<TestCameraNodeModifier>())  << "Не правильное инициализирование модификаторов." << endl;

    {
        TestCameraNodeModifier* test_camera_node_modifier = new TestCameraNodeModifier;

        test_camera_node_modifier->i = init_value;
        node1.addCameraNodeModifier(test_camera_node_modifier);
    }

    EXPECT_TRUE(node1.hasCameraNodeModifier<TestCameraNodeModifier>())              << "Ошибка в методе addCameraNodeModifier(...)." << endl;
    EXPECT_EQ(init_value, node1.getCameraNodeModifier<TestCameraNodeModifier>().i)  << "Ошибка в методе addCameraNodeModifier(...)." << endl;
}

TEST(SceneGraphCameraNode, UpdateTest)
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

    CameraNode      node1;
    PtrCameraNode   node2 = CameraNode::make("Node 2");
    PtrCameraNode   node3 = CameraNode::make("Node 3");
    PtrCameraNode   node4 = CameraNode::make("Node 4");
    PtrCameraNode   node5 = CameraNode::make("Node 5");
    
    node1.addChild(node2);
    node1.addChild(node3);
    node2->addChild(node4);
    node3->addChild(node5);

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
}
