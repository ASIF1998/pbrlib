//
//  material.inl
//  PBRLib
//
//  Created by Асиф Мамедов on 30/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

namespace pbrlib
{
    template<Material::Textures TextureType>
    void Material::setTexture(const PtrImageView& ptr_image_view)
    {
        _textures[util::enumCast(TextureType)] = ptr_image_view;
    }

    template<Material::Textures TextureType>
    PtrImageView& Material::getTexture()
    {
        return _textures[util::enumCast(TextureType)];
    }

    template<Material::Textures TextureType>
    const PtrImageView& Material::getTexture() const
    {
        return _textures[util::enumCast(TextureType)];
    }
}
