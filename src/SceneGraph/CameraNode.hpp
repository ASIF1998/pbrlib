//
//  CameraNode.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 18/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef CameraNode_hpp
#define CameraNode_hpp

#include "../Rendering/Camera/PerspectiveCamera.hpp"
#include "Scene.hpp"

namespace pbrlib
{
    class CameraNode;
    class ICameraNodeModifier;

    using PtrCameraNode             = shared_ptr<CameraNode>;
    using PtrICameraNodeModifier    = unique_ptr<ICameraNodeModifier>;

    class CameraNode :
        public Scene::Node
    {
    public:
        CameraNode(const string_view name = "Camera Node");
        CameraNode(const string_view name, const PtrICamera& ptr_camera);
        CameraNode(const string_view name, const PerspectiveCamera::Builder& camera_builder);
        CameraNode(const PerspectiveCamera::Builder& camera_builder);
        CameraNode(const PtrICamera& ptr_camera);

        void setCamera(const PtrICamera& ptr_camera);
        void setCamera(const PerspectiveCamera::Builder& camera_builder);

        void addCameraNodeModifier(ICameraNodeModifier* ptr_camera_modifire);

        PtrICamera&         getCamera() noexcept;
        const PtrICamera&   getCamera() const noexcept;

        template<typename CameraNodeModifier>
        inline CameraNodeModifier& getCameraNodeModifier();

        template<typename CameraNodeModifier>
        inline const CameraNodeModifier& getCameraNodeModifier() const;

        template<typename CameraNodeModifier>
        inline bool hasCameraNodeModifier() const;

        virtual void update(float delta_time, const Transform& world_transform) override;

        static PtrCameraNode make(const string_view name = "Camera Node");
        static PtrCameraNode make(const string_view name, const PtrICamera& ptr_camera);
        static PtrCameraNode make(const string_view name, const PerspectiveCamera::Builder& camera_builder);
        static PtrCameraNode make(const PerspectiveCamera::Builder& camera_builder);
        static PtrCameraNode make(const PtrICamera& ptr_camera);

    private:
        PtrICamera                                          _ptr_camera;
        unordered_map<type_index, PtrICameraNodeModifier>   _camera_node_modifiers;
    };

    class ICameraNodeModifier
    {
    public:
        ICameraNodeModifier(const string_view name = "Camera Node Modifier");
        virtual ~ICameraNodeModifier();

        virtual void update(CameraNode* ptr_node, float delta_time) = 0;

        void setName(const string_view name);

        string&             getName() noexcept;
        const string&       getName() const noexcept;
        
        virtual type_index  getType() const = 0;

    private:
        string _name;
    };
}

#include "CameraNode.inl"

#endif /* CameraNode_hpp */
