//
//  MaterialManager.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 01/06/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <pbrlib/Rendering/Material/MaterialManager.hpp>

using namespace std;

namespace pbrlib
{
    bool MaterialManager::addMaterial(
        const PtrMaterial&  ptr_material, 
        const string_view   material_name
    )
    {
        if (_materials.find(material_name.data()) == end(_materials)) {
            _materials.insert(make_pair(material_name, ptr_material));
            return true;
        }

        return false;
    }

    bool MaterialManager::addMaterial(
        Material::Builder&  material_builder, 
        const string_view   material_name
    )
    {
        if (_materials.find(material_name.data()) == end(_materials)) {
            _materials.insert(make_pair(material_name, material_builder.buildPtr()));
        }
        
        return false;
    }

    optional<PtrMaterial> MaterialManager::getMaterial(const string_view material_name)
    {
        auto it = _materials.find(material_name.data());
        return it != end(_materials) ? make_optional(it->second) : nullopt;
    }

    optional<const PtrMaterial> MaterialManager::getMaterial(const string_view material_name) const
    {
        auto it = _materials.find(material_name.data());
        return it != end(_materials) ? make_optional(it->second) : nullopt;
    }
}
