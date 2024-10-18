//
//  MeshNodeTests.cpp
//  PBRLibTests
//
//  Created by Асиф Мамедов on 19/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <ctime>

#include "../utils.hpp"

#include <pbrlib/scene/mesh_node.hpp>

using namespace pbrlib;
using namespace std;

TEST(SceneGraphMeshNode, Constructor)
{
    constexpr string_view   name1   = "Mesh Node";
    const string            name2   = "Node 2";

    AABB raabb;

    std::shared_ptr ptr_mesh = Mesh::make();

    MeshNode node1;
    MeshNode node2(name2, ptr_mesh);

    constexpr Matrix4x4<float> rm;

    pbrlib::testing::utils::thisFalse(node1.worldAABBIsCurrent(), "При инициализации мировой ограничивающий объём должен быть не актуальным.");
    pbrlib::testing::utils::thisFalse(node2.worldAABBIsCurrent(), "При инициализации мировой ограничивающий объём должен быть не актуальным.");

    pbrlib::testing::utils::thisFalse(node1.worldTransformIsCurrent(), "При инициализации мировое преобразование должно быть не актуальным.");
    pbrlib::testing::utils::thisFalse(node2.worldTransformIsCurrent(), "При инициализации мировое преобразование должно быть не актуальным.");

    pbrlib::testing::utils::equality(raabb, node1.getWorldAABB(), "Не правильная инициализация мирового ограничивающего объёма.");
    pbrlib::testing::utils::equality(raabb, node2.getWorldAABB(), "Не правильная инициализация мирового ограничивающего объёма.");

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
    
    pbrlib::testing::utils::equality(static_cast<size_t>(0), reinterpret_cast<size_t>(node1.getMesh().get()), "Не правильное инициализирование указателя на меш.");
    pbrlib::testing::utils::notEquality(static_cast<size_t>(0), reinterpret_cast<size_t>(node2.getMesh().get()), "Не правильное инициализирование указателя на меш.");
    pbrlib::testing::utils::thisFalse(node1.hasComponent<Mesh>());
    pbrlib::testing::utils::thisTrue(node2.hasComponent<Mesh>());
}

TEST(SceneGraphMeshNode, GettersAndSetters)
{
    srand(static_cast<unsigned>(time(nullptr)));

    std::shared_ptr<Mesh> ptr_mesh = Mesh::make();
    MeshNode    node;

    node.setMesh(ptr_mesh);

    pbrlib::testing::utils::equality(std::static_pointer_cast<const Mesh>(ptr_mesh), node.getMesh());
    pbrlib::testing::utils::thisTrue(node.hasComponent<Mesh>());
    pbrlib::testing::utils::equality(reinterpret_cast<size_t>(ptr_mesh.get()), reinterpret_cast<size_t>(&node.getComponent<Mesh>()));
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
    std::shared_ptr node2 = MeshNode::make("Node 2");
    std::shared_ptr node3 = MeshNode::make("Node 3");
    std::shared_ptr node4 = MeshNode::make("Node 4");
    std::shared_ptr node5 = MeshNode::make("Node 5");
    
    node1.addChild(node2);
    node1.addChild(node3);
    node2->addChild(node4);
    node3->addChild(node5);

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

    pbrlib::testing::utils::equality(bbox3[0], node1.getWorldAABB()[0], "Не правильно работает метод update(...).");
    pbrlib::testing::utils::equality(bbox3[1], node1.getWorldAABB()[1], "Не правильно работает метод update(...).");

    pbrlib::testing::utils::equality(bbox1[0], node2->getWorldAABB()[0], "Не правильно работает метод update(...).");
    pbrlib::testing::utils::equality(bbox1[1], node2->getWorldAABB()[1], "Не правильно работает метод update(...).");

    pbrlib::testing::utils::equality(bbox2[0], node3->getWorldAABB()[0], "Не правильно работает метод update(...).");
    pbrlib::testing::utils::equality(bbox2[1], node3->getWorldAABB()[1], "Не правильно работает метод update(...).");

    pbrlib::testing::utils::equality(bbox1[0], node4->getWorldAABB()[0], "Не правильно работает метод update(...).");
    pbrlib::testing::utils::equality(bbox1[1], node4->getWorldAABB()[1], "Не правильно работает метод update(...).");

    pbrlib::testing::utils::equality(bbox2[0], node5->getWorldAABB()[0], "Не правильно работает метод update(...).");
    pbrlib::testing::utils::equality(bbox2[1], node5->getWorldAABB()[1], "Не правильно работает метод update(...).");
}
