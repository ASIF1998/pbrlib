//
//  Material.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 29/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include "Material.hpp"

namespace pbrlib
{
    Material::Material() :
       _albedo             (nullptr),
       _normal_map         (nullptr),
       _metallic_roughness (nullptr),
       _baked_AO           (nullptr)
    {}

    Material::Material(const Material& material) :
        _albedo             (material._albedo),
        _normal_map         (material._normal_map),
        _metallic_roughness (material._metallic_roughness),
        _baked_AO           (material._baked_AO)
    {}

    Material::Material(Material&& material) :
        _albedo             (move(material._albedo)),
        _normal_map         (move(material._normal_map)),
        _metallic_roughness (move(material._metallic_roughness)),
        _baked_AO           (move(material._baked_AO))
    {}

    Material::~Material()
    {}

    void Material::setTexture(size_t i, const PtrImageView& ptr_image_view)
    {
        assert(i < util::enumCast(Textures::Count));
        _textures[i] = ptr_image_view;
    }

    PtrImageView& Material::getTexture(size_t i)
    {
        assert(i < util::enumCast(Textures::Count));
        return _textures[i];
    }

    const PtrImageView& Material::getTexture(size_t i) const
    {
        assert(i < util::enumCast(Textures::Count));
        return _textures[i];
    }

    PtrMaterial Material::make()
    {
        return make_shared<Material>();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Material::Builder::Builder() :
        _albedo                     (nullptr),
        _normal_map                 (nullptr),
        _metallic_roughness         (nullptr),
        _baked_AO                   (nullptr),
        _path_to_albedo             (""),
        _path_to_normal_map         (""),
        _path_to_metallic_roughness (""),
        _path_to_baked_AO           ("")
    {}

    void Material::Builder::setAlbedo(const PtrImageView& albedo)
    {
        _albedo = albedo;
    }

    void Material::Builder::setNormalMap(const PtrImageView& normal_map)
    {
        _normal_map = normal_map;
    }

    void Material::Builder::setMetallicRoughness(const PtrImageView& metallic_roughness)
    {
        _metallic_roughness = metallic_roughness;
    }

    void Material::Builder::setBakedAO(const PtrImageView& baked_AO)
    {
        _baked_AO = baked_AO;
    }

    void Material::Builder::setPathToAlbedo(const string_view path_to_albedo)
    {
        _path_to_albedo = path_to_albedo;
    }

    void Material::Builder::setPathToNormalMap(const string_view path_to_normal_map)
    {
        _path_to_normal_map = path_to_normal_map;
    }

    void Material::Builder::setPathToMetallicRoughness(const string_view path_to_metallic_roughness)
    {
        _path_to_metallic_roughness = path_to_metallic_roughness;
    }

    void Material::Builder::setPathToBakedAO(const string_view path_to_baked_AO)
    {
        _path_to_baked_AO = path_to_baked_AO;
    }

    void Material::Builder::_loading_missing_texture()
    {
        if (!_albedo) {
            /**
             * TODO: Загрузить текстуру с albedo.
            */
        }

        if (!_normal_map) {
            /**
             * TODO: Загрузить текстуру с normal map.
            */
        }

        if (!_metallic_roughness) {
            /**
             * TODO: Загрузить текстуру с metallic и roughness.
            */
        }

        if (!_baked_AO) {
            /**
             * TODO: Загрузить текстуру с запечённым ambient occlusion.
            */
        }
    }

   Material Material::Builder::build()
   {

        Material material;

        _loading_missing_texture();

        material._albedo                = _albedo;
        material._normal_map            = _normal_map;
        material._metallic_roughness    = _metallic_roughness;
        material._baked_AO              = _baked_AO;

        return material;
   }

    PtrMaterial Material::Builder::buildPtr()
    {
        PtrMaterial ptr_material = Material::make();

        _loading_missing_texture();

        ptr_material->_albedo               = _albedo;
        ptr_material->_normal_map           = _normal_map;
        ptr_material->_metallic_roughness   = _metallic_roughness;
        ptr_material->_baked_AO             = _baked_AO;

        return ptr_material;;
    }
}
