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
        if constexpr (TextureType == Textures::Albedo) {
            _albedo = ptr_image_view;
        } else if constexpr (TextureType == Textures::NormalMap) {
            _normal_map = ptr_image_view;
        } else if constexpr (TextureType == Textures::MetallicRoughness) {
            _metallic_roughness = ptr_image_view;
        } else if constexpr (TextureType == Textures::BakedAO) {
            _baked_AO = ptr_image_view;
        }
    }

    template<Material::Textures TextureType>
    PtrImageView& Material::getTexture()
    {
        if constexpr (TextureType == Textures::Albedo) {
            return _albedo;
        } else if constexpr (TextureType == Textures::NormalMap) {
            return _normal_map;
        } else if constexpr (TextureType == Textures::MetallicRoughness) {
            return _metallic_roughness;
        } else if constexpr (TextureType == Textures::BakedAO) {
            return _baked_AO;
        }
    }

    template<Material::Textures TextureType>
    const PtrImageView& Material::getTexture() const
    {
        if constexpr (TextureType == Textures::Albedo) {
            return _albedo;
        } else if constexpr (TextureType == Textures::NormalMap) {
            return _normal_map;
        } else if constexpr (TextureType == Textures::MetallicRoughness) {
            return _metallic_roughness;
        } else if constexpr (TextureType == Textures::BakedAO) {
            return _baked_AO;
        }
    }
}