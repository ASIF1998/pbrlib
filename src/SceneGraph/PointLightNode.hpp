//
//  PointLightNode.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 17/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef PointLightNode_hpp
#define PointLightNode_hpp

#include "../Rendering/Lights/PointLight.hpp"
#include "Scene.hpp"

namespace pbrlib
{
    class PointLightNode;
    class IPointLightNodeModifier;

    using PtrPointLightNode             = shared_ptr<PointLightNode>;
    using PtrIPointLightNodeModifier    = unique_ptr<IPointLightNodeModifier>;

    class PointLightNode :
        public Scene::Node
    {
    public:
        PointLightNode(
            const string_view   name    = "Point Light Node",
            Scene::Node*        parent  = nullptr
        );

        PointLightNode(
            const string_view           name,
            Scene::Node*                parent,
            const PointLight::Builder&  light_builder
        );

        PointLightNode(
            const string_view       name,
            Scene::Node*            parent,
            const PtrPointLight&    ptr_light
        );

        PointLightNode(const PointLight::Builder& light_builder);
        PointLightNode(const PtrPointLight& ptr_light);

        virtual ~PointLightNode();

        void setLight(const PointLight::Builder& light_builder);
        void setLight(const PtrPointLight& ptr_light);
        
        void addPointLightNodeModifier(IPointLightNodeModifier* ptr_light_node_modifire);

        PtrPointLight&          getLight() noexcept;
        const PtrPointLight&    getLight() const noexcept;

        template<typename PointLightNodeModifier>
        inline PointLightNodeModifier& getPointLightNodeModifier();

        template<typename PointLightNodeModifier>
        inline const PointLightNodeModifier& getPointLightNodeModifier() const;

        template<typename PointLightNodeModifier>
        inline bool hasPointLightNodeModifier() const;

        virtual void update(float delta_time, const Transform& world_transform) override;

    private:
        PtrPointLight                                           _ptr_light;
        unordered_map<type_index, PtrIPointLightNodeModifier>   _point_light_node_modifier;
    };

    class IPointLightNodeModifier
    {
    public:
        IPointLightNodeModifier(const string_view name = "Spot Light Node Modifier");
        virtual ~IPointLightNodeModifier();

        virtual void update(PointLightNode* ptr_node, float delta_time) = 0;

        void setName(const string_view name);

        string&         getName() noexcept;
        const string&   getName() const noexcept;

        virtual type_index  getType() const = 0; 

    private:
        string _name;
    };
}

#include "PointLightNode.inl"

#endif /* PointLightNode_hpp */
