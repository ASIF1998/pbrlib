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
    class DirectionLightNode;
    class SpotLightNode;
    class PointLightNode;

    using PtrDirectionLightNode     = shared_ptr<DirectionLightNode>;
    using PtrSpotLightNode          = shared_ptr<SpotLightNode>;
    using PtrPointLightNode         = shared_ptr<PointLightNode>;
    using PtrINodeModifier          = unique_ptr<INodeModifier>;

    class Scene
    {
    public:
        class Node;

        using PtrNode = shared_ptr<Node>;

        class Node
        {
        public:
            Node(
                const string_view   name    = "No name",
                Node*               parent  = nullptr
            );

            virtual ~Node();

            AABB&                   getWorldAABB()      noexcept;
            const AABB&             getWorldAABB()      const noexcept;
            Node*                   getParent()         noexcept;
            const Node*             getParent()         const noexcept;
            PtrNode&                getChild(size_t i);
            const PtrNode&          getChild(size_t i)  const;
            vector<PtrNode>&        getChildren()       noexcept;
            const vector<PtrNode>&  getChildren()       const noexcept;
            Transform&              getLocalTransform() noexcept;
            const Transform&        getLocalTransform() const noexcept;
            Transform&              getWorldTransform() noexcept;
            const Transform&        getWorldTransform() const noexcept;
            string&                 getName()           noexcept;
            const string&           getName()           const noexcept;

            template<typename NodeModifierType>
            inline NodeModifierType& getNodeModifier();

            template<typename NodeModifierType>
            inline const NodeModifierType& getNodeModifier() const;

            template<typename NodeModifierType>
            inline bool hasNodeModifier() const;

            void setParent(Node* ptr_parent) noexcept;
            void setChildren(vector<PtrNode>&& children);
            void setChildren(const vector<PtrNode>& children);
            void setLocalTransform(const Transform& transform);
            void setWorldTransform(const Transform& transform);
            void setWorldAABB(const AABB& bbox);
            void setName(const string_view name);

            bool worldTransformIsCurrent()              const noexcept;
            void worldTransformIsCurrent(bool current)  noexcept;
            bool worldAABBIsCurrent()                   const noexcept;
            void worldAABBIsCurrent(bool current)       noexcept;

            void addNodeModifier(INodeModifier* ptr_node_modifier);

            /**
             * @brief Метод необходимый для добавления дочернего узла.
             * @details 
             *      При добавлении дочернего узла указатель на родителя 
             *      устанавливается автоматически.
             * 
             * @param child ссылка на дочерний узел.
            */
            void addChild(PtrNode&& child);

            /**
             * @brief Метод необходимый для добавления дочернего узла.
             * @details 
             *      При добавлении дочернего узла указатель на родителя 
             *      устанавливается автоматически.
             * 
             * @param child ссылка на дочерний узел.
            */
            void addChild(const PtrNode& child);

            /**
             * @brief Метод необходимый для добавления дочернего узла.
             * @details 
             *      При добавлении дочернего узла указатель на родителя 
             *      устанавливается автоматически.
             * 
             * @param node_name название дочернего узла.
             * @return Указатель на дочерний узел.
            */
            PtrNode& addChild(const string_view node_name);

            virtual void update(float delta_time, const Transform& world_transform);

            static PtrNode make(
                const string_view   name    = "No name",
                Node*               parent  = nullptr
            );

        private:
            Node*                                           _ptr_parent; 
            vector<PtrNode>                                 _ptr_children;
            Transform                                       _local_transform;
            Transform                                       _world_transform;
            bool                                            _world_transform_is_current;
            bool                                            _world_aabb_is_current;
            AABB                                            _world_bbox;
            unordered_map<type_index, PtrINodeModifier>     _node_modifiers;
            string                                          _name;
        };

    public:
    private:
        vector<PtrDirectionLightNode>   _dir_light_nodes;
        vector<PtrSpotLightNode>        _spot_light_nodes;
        vector<PtrPointLightNode>       _point_light_nodes;
    };

    class INodeModifier
    {
    public:
        INodeModifier(const string_view name = "Node Modifier");
        virtual ~INodeModifier();

        virtual void update(Scene::Node* ptr_node, float delta_time) = 0;

        void setName(const string_view name);

        string&             getName() noexcept;
        const string&       getName() const noexcept;
        virtual type_index  getType() const = 0;

        template<typename NodeModifierType>
        inline static type_index getTypeIndex();

    private:
        string _name;
    };
}

#include "Scene.inl"

#endif /* Scene_hpp */
