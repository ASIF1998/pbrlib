//
//  DirectionLightNode.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 17/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef DirectionLightNode_hpp
#define DirectionLightNode_hpp

#include "../Rendering/Lights/DirectionLight.hpp"
#include "Scene.hpp"

namespace pbrlib
{
    class DirectionLightNode;
    class IDirectionLightNodeModifier;

    using PtrDirectionLightNode             = shared_ptr<DirectionLightNode>;
    using PtrIDirectionLightNodeModifier    = unique_ptr<IDirectionLightNodeModifier>;

    class DirectionLightNode :
        public Scene::Node
    {
    public:
        DirectionLightNode(
            const string_view   name    = "Direction Light Node",
            Scene::Node*        parent  = nullptr
        );

        DirectionLightNode(
            const string_view               name,
            Scene::Node*                    parent,
            const DirectionLight::Builder&  light_builder
        );

        DirectionLightNode(
            const string_view           name,
            Scene::Node*                parent,
            const PtrDirectionLight&    ptr_light
        );

        DirectionLightNode(const DirectionLight::Builder& light_builder);
        DirectionLightNode(const PtrDirectionLight& ptr_light);

        void setLight(const DirectionLight::Builder& light_builder);
        void setLight(const PtrDirectionLight& ptr_light);

        void addSpotLightNodeModifier(IDirectionLightNodeModifier* ptr_light_node_modifire);

        PtrDirectionLight&          getLight() noexcept;
        const PtrDirectionLight&    getLight() const noexcept;

        template<typename DirLightNodeModifier>
        inline DirLightNodeModifier& getDirLightNodeModifier();

        template<typename DirLightNodeModifier>
        inline const DirLightNodeModifier& getDirLightNodeModifier() const;

        template<typename DirLightNodeModifier>
        inline bool hasDirLightNodeModifier() const;

        virtual void update(float delta_time, const Transform& world_transform) override;

        static PtrDirectionLightNode make(
            const string_view   name    = "Direction Light Node",
            Scene::Node*        parent  = nullptr
        );

        static PtrDirectionLightNode make(
            const string_view               name,
            Scene::Node*                    parent,
            const DirectionLight::Builder&  light_builder
        );

        static PtrDirectionLightNode make(
            const string_view           name,
            Scene::Node*                parent,
            const PtrDirectionLight&    ptr_light
        );

        static PtrDirectionLightNode make(const DirectionLight::Builder& light_builder);
        static PtrDirectionLightNode make(const PtrDirectionLight& ptr_light);

    private:
        PtrDirectionLight                                           _ptr_light;    
        unordered_map<type_index, PtrIDirectionLightNodeModifier>   _dir_light_node_modifiers;
    };

    class IDirectionLightNodeModifier
    {
    public:
        IDirectionLightNodeModifier(const string_view name = "Direction Light Node Modifier");
        virtual ~IDirectionLightNodeModifier();

        virtual void update(DirectionLightNode* ptr_node, float delta_time) = 0;

        void setName(const string_view name);

        string&             getName() noexcept;
        const string&       getName() const noexcept;
        
        virtual type_index  getType() const = 0;

    private:
        string _name;
    };
}

#include "DirectionLightNode.inl"

#endif /* DirectionLightNode_hpp */
