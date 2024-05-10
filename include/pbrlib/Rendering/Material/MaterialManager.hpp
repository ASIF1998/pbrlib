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

using namespace std;

namespace pbrlib
{
    class MaterialManager
    {
    public:
        MaterialManager() = default;

        bool addMaterial(const PtrMaterial& ptr_material, const string_view material_name);
        bool addMaterial(Material::Builder& material_builder, const string_view material_name);

        optional<PtrMaterial>       getMaterial(const string_view material_name);
        optional<const PtrMaterial> getMaterial(const string_view material_name) const;

    private:
        unordered_map<string, PtrMaterial> _materials;
    };
}

#endif /* MaterialManager_hpp */
