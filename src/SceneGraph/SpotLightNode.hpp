//
//  SpotLightNode.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 17/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef SpotLightNode_hpp
#define SpotLightNode_hpp

#include "../Rendering/Lights/SpotLight.hpp"
#include "Scene.hpp"

namespace pbrlib
{
    class SpotLightNode;
    class ISpotLightNodeModifier;

    using PtrSpotLightNode          = shared_ptr<SpotLightNode>;
    using PtrISpotLightNodeModifier = unique_ptr<ISpotLightNodeModifier>;

    class SpotLightNode :
        public Scene::Node
    {
    public:
        SpotLightNode(
            const string_view   name    = "Spot Light Node",
            Scene::Node*        parent  = nullptr
        );

        SpotLightNode(
            const string_view           name,
            Scene::Node*                parent,
            const SpotLight::Builder&   light_builder
        );

        SpotLightNode(
            const string_view   name,
            Scene::Node*        parent,
            const PtrSpotLight& ptr_light
        );

        SpotLightNode(const SpotLight::Builder& light_builder);
        SpotLightNode(const PtrSpotLight& ptr_light);

        virtual ~SpotLightNode();

        void setLight(const SpotLight::Builder& light_builder);
        void setLight(const PtrSpotLight& ptr_light);

        void addSpotLightNodeModifier(ISpotLightNodeModifier* ptr_light_node_modifire);

        PtrSpotLight&       getLight() noexcept;
        const PtrSpotLight& getLight() const noexcept;

        template<typename SpotLightNodeModifier>
        inline SpotLightNodeModifier& getSpotLightNodeModifier();

        template<typename SpotLightNodeModifierr>
        inline const SpotLightNodeModifierr& getSpotLightNodeModifier() const;

        template<typename SpotLightNodeModifierr>
        inline bool hasSpotLightNodeModifier() const;

        virtual void update(float delta_time, const Transform& world_transform) override;

    private:
        PtrSpotLight                                            _ptr_light;
        unordered_map<type_index, PtrISpotLightNodeModifier>    _spot_light_node_modifier;
    };

    class ISpotLightNodeModifier
    {
    public:
        ISpotLightNodeModifier(const string_view name = "Spot Light Node Modifier");
        virtual ~ISpotLightNodeModifier();

        virtual void update(SpotLightNode* ptr_node, float delta_time) = 0;

        void setName(const string_view name);

        string&         getName() noexcept;
        const string&   getName() const noexcept;

        virtual type_index  getType() const = 0; 

    private:
        string _name;
    };
}

#include "SpotLightNode.inl"

#endif /* SpotLightNode_hpp */
