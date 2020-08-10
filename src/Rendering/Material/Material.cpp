//
//  Material.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 29/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include "Material.hpp"

#include "../VulkanWrapper/GPUTextureManager.hpp"

namespace pbrlib
{
    Material::Material() :
        _albedo             (nullptr),
        _normal_map         (nullptr),
        _metallic           (nullptr),
        _roughness          (nullptr),
        _baked_AO           (nullptr),
        _anisotropy         (0.0f)
    {}

    Material::Material(const Material& material) :
        _albedo             (material._albedo),
        _normal_map         (material._normal_map),
        _metallic           (material._metallic),
        _roughness          (material._roughness),
        _baked_AO           (material._baked_AO),
        _anisotropy         (material._anisotropy)
    {}

    Material::Material(Material&& material) :
        _albedo             (move(material._albedo)),
        _normal_map         (move(material._normal_map)),
        _metallic           (move(material._metallic)),
        _roughness          (move(material._roughness)),
        _baked_AO           (move(material._baked_AO)),
        _anisotropy         (material._anisotropy)
    {}

    Material::~Material()
    {}

    void Material::setTexture(size_t i, const PtrImageView& ptr_image_view)
    {
        assert(i < util::enumCast(Textures::Count));
        _textures[i] = ptr_image_view;
    }

    void Material::setAnisotropy(float anisotropy)
    {
        assert(anisotropy >= -1.0f && anisotropy <= 1.0f);
        _anisotropy = anisotropy;
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

    float Material::getAnisotropy() const noexcept
    {
        return _anisotropy;
    }

    PtrMaterial Material::make()
    {
        return make_shared<Material>();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Material::Builder::Builder(GPUTextureManager& texture_manager) :
        _albedo                     (nullptr),
        _normal_map                 (nullptr),
        _metallic                   (nullptr),
        _roughness                  (nullptr),
        _baked_AO                   (nullptr),
        _anisotropy                 (0.0f),
        _texture_manager            (texture_manager)
    {}

    void Material::Builder::setAlbedo(const PtrImageView& albedo)
    {
        _albedo = albedo;
    }

    bool Material::Builder::setAlbedo(const string_view texture_name)
    {
        if (auto texture = _texture_manager.get(texture_name); texture != nullopt) {
            _albedo = texture.value();
            return true;
        }

        return false;
    }

    bool Material::Builder::setAlbedo(const string_view path_to_albedo, const string_view texture_name)
    {
        if (!_texture_manager.loadRGBA(path_to_albedo, texture_name)) {
            /// Не удалось загрузить тектуру.
            return false;
        }

        _albedo = _texture_manager.get(texture_name).value();

        return true;
    }

    void Material::Builder::setNormalMap(const PtrImageView& normal_map)
    {
        _normal_map = normal_map;
    }

    bool Material::Builder::setNormalMap(const string_view texture_name)
    {
        if (auto texture = _texture_manager.get(texture_name); texture != nullopt) {
            _normal_map = texture.value();
            return true;
        }

        return false;
    }

    bool Material::Builder::setNormalMap(const string_view path_to_normal_map, const string_view texture_name)
    {
        if (!_texture_manager.loadRGBA(path_to_normal_map, texture_name)) {
            /// Не удалось загрузить тектуру.
            return false;
        }

        _normal_map = _texture_manager.get(texture_name).value();

        return true;
    }

    void Material::Builder::setMetallic(const PtrImageView& metallic)
    {
        _metallic = metallic;
    }

    bool Material::Builder::setMetallic(const string_view texture_name)
    {
        if (auto texture = _texture_manager.get(texture_name); texture != nullopt) {
            _metallic = texture.value();
            return true;
        }

        return false;
    }

    bool Material::Builder::setMetallic(const string_view path_to_metallic, const string_view texture_name)
    {
        if (!_texture_manager.loadR(path_to_metallic, texture_name)) {
            /// Не удалось загрузить тектуру.
            return false;
        }

        _metallic = _texture_manager.get(texture_name).value();

        return true;
    }

    void Material::Builder::setRoughness(const PtrImageView& roughness)
    {
        _roughness = roughness;
    }

    bool Material::Builder::setRoughness(const string_view texture_name)
    {
        if (auto texture = _texture_manager.get(texture_name); texture != nullopt) {
            _roughness = texture.value();
            return true;
        }

        return false;
    }

    bool Material::Builder::setRoughness(const string_view path_to_roughness, const string_view texture_name)
    {
        if (!_texture_manager.loadR(path_to_roughness, texture_name)) {
            /// Не удалось загрузить тектуру.
            return false;
        }

        _roughness = _texture_manager.get(texture_name).value();

        return true;
    }

    void Material::Builder::setBakedAO(const PtrImageView& baked_AO)
    {
        _baked_AO = baked_AO;
    }

    bool Material::Builder::setBakedAO(const string_view texture_name)
    {
        if (auto texture = _texture_manager.get(texture_name); texture != nullopt) {
            _baked_AO = texture.value();
            return true;
        }

        return false;
    }

    bool Material::Builder::setBakedAO(const string_view path_to_baked_AO, const string_view texture_name)
    {
        if (!_texture_manager.loadR(path_to_baked_AO, texture_name)) {
            /// Не удалось загрузить тектуру.
            return false;
        }

        _baked_AO = _texture_manager.get(texture_name).value();

        return true;
    }

    void Material::Builder::setAnisotropy(float anisotropy)
    {
        assert(anisotropy >= -1.0f && anisotropy <= 1.0f);
        _anisotropy = anisotropy;
    }

   Material Material::Builder::build()
   {
        assert(_albedo && _normal_map && _metallic && _roughness && _baked_AO);

        Material material;

        material._albedo                = _albedo;
        material._normal_map            = _normal_map;
        material._metallic              = _metallic;
        material._roughness             = _roughness;
        material._baked_AO              = _baked_AO;
        material._anisotropy            = _anisotropy;

        return material;
   }

    PtrMaterial Material::Builder::buildPtr()
    {
        assert(_albedo && _normal_map && _metallic && _roughness && _baked_AO);

        PtrMaterial ptr_material = Material::make();

        ptr_material->_albedo               = _albedo;
        ptr_material->_normal_map           = _normal_map;
        ptr_material->_metallic             = _metallic;
        ptr_material->_roughness            = _roughness;
        ptr_material->_baked_AO             = _baked_AO;
        ptr_material->_anisotropy           = _anisotropy;

        return ptr_material;;
    }
}
