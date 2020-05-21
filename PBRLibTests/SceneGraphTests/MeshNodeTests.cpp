//
//  MeshNodeTests.cpp
//  PBRLibTests
//
//  Created by Асиф Мамедов on 19/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <ctime>

#include <gtest/gtest.h>

#include "../../src/SceneGraph/MeshNode.hpp"

using namespace testing;
using namespace pbrlib;
using namespace std;

struct TestMeshNodeModifier :
    public IMeshNodeModifier
{
    TestMeshNodeModifier() :
        IMeshNodeModifier   (),
        i                   (0)
    {}

    virtual void update(MeshNode* ptr_node, float delta_time) override
    {}

    virtual type_index  getType() const override
    {
        return INodeModifier::getTypeIndex<TestMeshNodeModifier>();
    }

    int i;
};

TEST(SceneGraphMeshNode, Constructor)
{
    constexpr string_view   name1   = "Mesh Node";
    const string            name2   = "Node 2";
    constexpr char          name3[] = "Name 3";

    AABB raabb;

    PtrMesh ptr_mesh = Mesh::make();

    MeshNode    node1;
    MeshNode    node2(name2, nullptr, ptr_mesh);
    PtrMeshNode node3 = MeshNode::make(name3, &node2, ptr_mesh);

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

    EXPECT_EQ(rm, node1.getLocalTransform().getMatrix())    << "Не правильное инициализирование локального преобразования." << endl;
    EXPECT_EQ(rm, node2.getLocalTransform().getMatrix())    << "Не правильное инициализирование локального преобразования." << endl;
    EXPECT_EQ(rm, node3->getLocalTransform().getMatrix())   << "Не правильное инициализирование локального преобразования." << endl;

    EXPECT_EQ(name1, node1.getName())   << "Не правильное инициализирование имени." << endl;
    EXPECT_EQ(name2, node2.getName())   << "Не правильное инициализирование имени." << endl;
    EXPECT_EQ(name3, node3->getName())  << "Не правильное инициализирование имени." << endl;
    
    EXPECT_EQ(nullptr, node1.getMesh())     << "Не правильное инициализирование указателя на меш." << endl;
    EXPECT_NE(nullptr, node2.getMesh())     << "Не правильное инициализирование указателя на меш." << endl;
    EXPECT_EQ(ptr_mesh, node3->getMesh())   << "Не правильное инициализирование указателя на меш." << endl;
}

TEST(SceneGraphMeshNode, GettersAndSetters)
{
    srand(static_cast<unsigned>(time(nullptr)));

    const int init_value = rand();

    PtrMesh     ptr_mesh = Mesh::make();
    MeshNode    node;

    node.setMesh(ptr_mesh);

    EXPECT_EQ(ptr_mesh, node.getMesh());

    EXPECT_FALSE(node.hasMeshNodeModifier<TestMeshNodeModifier>()) << "Не правильное инициализирование модификаторов." << endl;

    {
        TestMeshNodeModifier* test_mesh_node_modifier = new TestMeshNodeModifier;

        test_mesh_node_modifier->i = init_value;
        node.addMeshNodeModifier(test_mesh_node_modifier);
    }

    EXPECT_TRUE(node.hasMeshNodeModifier<TestMeshNodeModifier>())               << "Ошибка в методе addMeshNodeModifier(...)." << endl;
    EXPECT_EQ(init_value, node.getMeshNodeModifier<TestMeshNodeModifier>().i)   << "Ошибка в методе addMeshNodeModifier(...)." << endl;
}

TEST(SceneGraphMeshNode, UpdateTest)
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
    
    MeshNode     node1;
    PtrMeshNode  node2 = MeshNode::make("Node 2", &node1);
    PtrMeshNode  node3 = MeshNode::make("Node 3", &node1);
    PtrMeshNode  node4 = MeshNode::make("Node 4", node2.get());
    PtrMeshNode  node5 = MeshNode::make("Node 5", node3.get());
    
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
