//
//  MaterialManager.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 01/06/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef MaterialManager_hpp
#define MaterialManager_hpp

#include <unordered_map>

#include <optional>

#include <string>
#include <string_view>

#include "Material.hpp"

namespace pbrlib
{
    class MaterialManager
    {
    public:
        MaterialManager() = default;

        bool addMaterial(const PtrMaterial& ptr_material, const std::string_view material_name);
        bool addMaterial(Material::Builder& material_builder, const std::string_view material_name);

        std::optional<PtrMaterial>          getMaterial(const std::string_view material_name);
        std::optional<const PtrMaterial>    getMaterial(const std::string_view material_name) const;

    private:
        std::unordered_map<std::string, PtrMaterial> _materials;
    };
}

#endif /* MaterialManager_hpp */
