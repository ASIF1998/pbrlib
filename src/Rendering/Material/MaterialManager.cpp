//
//  MaterialManager.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 01/06/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <pbrlib/Rendering/Material/MaterialManager.hpp>

namespace pbrlib
{
    bool MaterialManager::addMaterial(
        std::shared_ptr<const Material> ptr_material, 
        const std::string_view          material_name
    )
    {
        if (_materials.find(material_name.data()) == end(_materials)) 
        {
            _materials.insert(make_pair(material_name, ptr_material));
            return true;
        }

        return false;
    }

    bool MaterialManager::addMaterial(
        Material::Builder&      material_builder, 
        const std::string_view  material_name
    )
    {
        return addMaterial(material_builder.buildPtr(), material_name);
    }

    std::shared_ptr<const Material> MaterialManager::getMaterial(const std::string_view material_name) const
    {
        auto it = _materials.find(material_name.data());
        return it != end(_materials) ? it->second : nullptr;
    }
}
