//
//  Material.inl
//  PBRLib
//
//  Created by Асиф Мамедов on 30/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

namespace pbrlib
{
    template<Material::Textures TextureType>
    void Material::setTexture(std::shared_ptr<const ImageView> ptr_image_view)
    {
        _textures[utils::enumCast(TextureType)] = ptr_image_view;
    }

    template<Material::Textures TextureType>
    std::shared_ptr<const ImageView> Material::getTexture() const
    {
        return _textures[utils::enumCast(TextureType)];
    }
}
