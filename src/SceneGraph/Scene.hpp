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
    class INodeModifier;

    class Scene
    {
    public:
        class Node;

        using PtrNode = shared_ptr<Node>;

        class Node
        {
        public:
            inline Node(
                const string_view   name    = "No name",
                Node*               parent  = nullptr
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
            inline void setNodeModifier(INodeModifier* ptr_node_modifier);
            inline void setName(const string_view name);

            inline bool worldTransformIsCurrent()               const noexcept;
            inline void worldTransformIsCurrent(bool current)   noexcept;
            inline bool worldAABBIsCurrent()                    const noexcept;
            inline void worldAABBIsCurrent(bool current)        noexcept;

            /**
             * @brief Метод необходимый для добавления дочернего узла.
             * @details 
             *      При добавлении дочернего узла указатель на родителя 
             *      устанавливается автоматически.
             * 
             * @param child ссылка на дочерний узел.
            */
            inline void addChild(PtrNode&& child);

            /**
             * @brief Метод необходимый для добавления дочернего узла.
             * @details 
             *      При добавлении дочернего узла указатель на родителя 
             *      устанавливается автоматически.
             * 
             * @param child ссылка на дочерний узел.
            */
            inline void addChild(const PtrNode& child);

            /**
             * @brief Метод необходимый для добавления дочернего узла.
             * @details 
             *      При добавлении дочернего узла указатель на родителя 
             *      устанавливается автоматически.
             * 
             * @param node_name название дочернего узла.
             * @return Указатель на дочерний узел.
            */
            inline PtrNode& addChild(const string_view node_name);

            inline virtual void update(float delta_time, const Transform& world_transform);

            inline static PtrNode make(
                const string_view   name    = "No name",
                Node*               parent  = nullptr
            );

        private:
            Node*                                                   _ptr_parent; 
            vector<PtrNode>                                         _ptr_children;
            Transform                                               _local_transform;
            Transform                                               _world_transform;
            bool                                                    _world_transform_is_current;
            bool                                                    _world_aabb_is_current;
            AABB                                                    _world_bbox;
            unordered_map<type_index, unique_ptr<INodeModifier>>    _node_modifiers;
            string                                                  _name;
        };

    public:
    private:
    };

    class INodeModifier
    {
    public:
        inline INodeModifier(const string_view name = "Node Modifier");
        inline virtual ~INodeModifier();

        virtual void update(Scene::Node* ptr_node, float delta_time) = 0;

        inline void setName(const string_view name);

        inline string&          getName() noexcept;
        inline const string&    getName() const noexcept;
        virtual type_index      getType() const = 0;

        template<typename NodeModifierType>
        inline static type_index getTypeIndex();

    private:
        string _name;
    };
}

#include "Scene.inl"

#endif /* Scene_hpp */
