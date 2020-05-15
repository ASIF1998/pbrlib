//
//  Scene.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 15/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef Scene_hpp
#define Scene_hpp

#include <memory>
#include <vector>
#include <unordered_map>

#include <string>
#include <string_view>

#include <typeindex>

#include "../Moving/Transform.hpp"
#include "../Rendering/Geometry/AABB.hpp"

using namespace std;

namespace pbrlib
{
    class NodeModifier;

    class Scene
    {
    public:
        class Node;

        using PtrNode = shared_ptr<Node>;

        class Node
        {
        public:
            inline Node() = default;

            inline Node(
                const string_view   name,
                const Transform&    local_transform,
                Node*               parent = nullptr
            );

            inline Node(
                const string_view   name,
                const Transform&    local_transform,
                const Transform&    world_transform,
                Node*               parent = nullptr
            );

            inline virtual ~Node();

            inline AABB&                   getWorldAABB()       noexcept;
            inline const AABB&             getWorldAABB()       const noexcept;
            inline Node*                   getParent()          noexcept;
            inline const Node*             getParent()          const noexcept;
            inline PtrNode&                getChild(size_t i);
            inline const PtrNode&          getChild(size_t i)   const;
            inline vector<PtrNode>&        getChildren()        noexcept;
            inline const vector<PtrNode>&  getChildren()        const noexcept;
            inline Transform&              getLocalTransform()  noexcept;
            inline const Transform&        getLocalTransform()  const noexcept;
            inline Transform&              getWorldTransform()  noexcept;
            inline const Transform&        getWorldTransform()  const noexcept;
            inline string&                 getName()            noexcept;
            inline const string&           getName()            const noexcept;

            template<typename NodeModifierType>
            inline NodeModifierType& getNodeModifier();

            template<typename NodeModifierType>
            inline const NodeModifierType& getNodeModifier() const;

            inline void setParent(Node* ptr_parent) noexcept;
            inline void setChildren(vector<PtrNode>&& children);
            inline void setChildren(const vector<PtrNode>& children);
            inline void setLocalTransform(const Transform& transform);
            inline void setWorldTransform(const Transform& transform);
            inline void setWorldAABB(const AABB& bbox);
            inline void setNodeModifier(NodeModifier* ptr_node_modifier);
            inline void setName(const string_view name);

            inline bool isWorldTransformCurrent()                   const noexcept;
            inline void isWorldTransformCurrent(bool is_current)    noexcept;

            inline void addChild(PtrNode&& child);
            inline void addChild(const PtrNode& child);

            inline virtual void update(float delta_time, const Transform& world_transform);

        private:
            Node*                                               _ptr_parent; 
            vector<PtrNode>                                     _ptr_children;
            Transform                                           _local_transform;
            Transform                                           _world_transform;
            bool                                                _world_transform_is_current;
            AABB                                                _world_bbox;
            unordered_map<type_index, unique_ptr<NodeModifier>> _node_modifiers;
            string                                              _name;
        };

    public:
    private:
    };

    class NodeModifier
    {
    public:
        inline NodeModifier(const string_view name = "Node Modifier");
        inline virtual ~NodeModifier();

        virtual void update(Scene::Node* ptr_node, float delta_time) = 0;

        inline void setName(const string_view name);

        inline string&          getName() noexcept;
        inline const string&    getName() const noexcept;
        virtual type_index      getType() const = 0;

    private:
        string _name;
    };
}

#include "Scene.inl"

#endif /* Scene_hpp */
