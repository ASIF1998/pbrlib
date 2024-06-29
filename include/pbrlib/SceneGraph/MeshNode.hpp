//
//  MeshNode.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 19/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef MeshNode_hpp
#define MeshNode_hpp

#include <pbrlib/Rendering/Geometry/Mesh.hpp>

#include "Scene.hpp"

namespace pbrlib
{
    class MeshNode :
        public SceneItem
    {
    public:
        MeshNode(const std::string_view name = "Mesh Node");
        MeshNode(const std::string_view name, std::shared_ptr<Mesh> ptr_mesh);
        MeshNode(std::shared_ptr<Mesh> ptr_mesh);

        void setMesh(std::shared_ptr<Mesh> ptr_mesh);

        std::shared_ptr<const Mesh> getMesh() const noexcept;

        virtual void update(const InputStay* ptr_input_stay, float delta_time, const Transform& world_transform) override;

        static std::unique_ptr<MeshNode> make(const std::string_view name = "Mesh Node");
        static std::unique_ptr<MeshNode> make(const std::string_view name, std::shared_ptr<Mesh> ptr_mesh);
        static std::unique_ptr<MeshNode> make(std::shared_ptr<Mesh> ptr_mesh);

    private:
        std::shared_ptr<Mesh> _ptr_mesh;
    };
}

#endif /* MeshNode_hpp */
