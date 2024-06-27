//
//  Material.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 29/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <pbrlib/Rendering/Material/Material.hpp>

#include <pbrlib/Rendering/VulkanWrapper/GPUTextureManager.hpp>

namespace pbrlib
{
    Material::Material() :
        _albedo     (nullptr),
        _normal_map (nullptr),
        _metallic   (nullptr),
        _roughness  (nullptr),
        _baked_AO   (nullptr),
        _anisotropy (0.0f)
    {}

    Material::Material(const Material& material) :
        _albedo     (material._albedo),
        _normal_map (material._normal_map),
        _metallic   (material._metallic),
        _roughness  (material._roughness),
        _baked_AO   (material._baked_AO),
        _anisotropy (material._anisotropy)
    {}

    Material::Material(Material&& material) :
        _albedo     (move(material._albedo)),
        _normal_map (move(material._normal_map)),
        _metallic   (move(material._metallic)),
        _roughness  (move(material._roughness)),
        _baked_AO   (move(material._baked_AO)),
        _anisotropy (material._anisotropy)
    {}

    Material::~Material()
    {}

    void Material::setTexture(size_t i, std::shared_ptr<const ImageView> ptr_image_view)
    {
        assert(i < utils::enumCast(Textures::Count));
        _textures[i] = ptr_image_view;
    }

    void Material::setAnisotropy(float anisotropy)
    {
        assert(anisotropy >= -1.0f && anisotropy <= 1.0f);
        _anisotropy = anisotropy;
    }

    std::shared_ptr<const ImageView> Material::getTexture(size_t i) const
    {
        assert(i < utils::enumCast(Textures::Count));
        return _textures[i];
    }

    float Material::getAnisotropy() const noexcept
    {
        return _anisotropy;
    }

    std::unique_ptr<Material> Material::make()
    {
        return std::make_unique<Material>();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Material::Builder::Builder(GPUTextureManager& texture_manager) :
        _albedo             (nullptr),
        _normal_map         (nullptr),
        _metallic           (nullptr),
        _roughness          (nullptr),
        _baked_AO           (nullptr),
        _anisotropy         (0.0f),
        _texture_manager    (texture_manager)
    {}

    void Material::Builder::setAlbedo(std::shared_ptr<const ImageView> albedo)
    {
        _albedo = albedo;
    }

    bool Material::Builder::setAlbedo(const std::string_view texture_name)
    {
        if (auto texture = _texture_manager.get(texture_name); texture) 
        {
            _albedo = texture;
            return true;
        }

        return false;
    }

    bool Material::Builder::setAlbedo(const std::string_view path_to_albedo, const std::string_view texture_name)
    {
        if (!_texture_manager.loadRGBA(path_to_albedo, texture_name))
            return false;

        _albedo = _texture_manager.get(texture_name);

        return true;
    }

    void Material::Builder::setNormalMap(std::shared_ptr<const ImageView> normal_map)
    {
        _normal_map = normal_map;
    }

    bool Material::Builder::setNormalMap(const std::string_view texture_name)
    {
        if (auto texture = _texture_manager.get(texture_name); texture) {
            _normal_map = texture;
            return true;
        }

        return false;
    }

    bool Material::Builder::setNormalMap(const std::string_view path_to_normal_map, const std::string_view texture_name)
    {
        if (!_texture_manager.loadRGBA(path_to_normal_map, texture_name))
            return false;

        _normal_map = _texture_manager.get(texture_name);

        return true;
    }

    void Material::Builder::setMetallic(std::shared_ptr<const ImageView> metallic)
    {
        _metallic = metallic;
    }

    bool Material::Builder::setMetallic(const std::string_view texture_name)
    {
        if (auto texture = _texture_manager.get(texture_name); texture) 
        {
            _metallic = texture;
            return true;
        }

        return false;
    }

    bool Material::Builder::setMetallic(const std::string_view path_to_metallic, const std::string_view texture_name)
    {
        if (!_texture_manager.loadR(path_to_metallic, texture_name))
            return false;

        _metallic = _texture_manager.get(texture_name);

        return true;
    }

    void Material::Builder::setRoughness(std::shared_ptr<const ImageView> roughness)
    {
        _roughness = roughness;
    }

    bool Material::Builder::setRoughness(const std::string_view texture_name)
    {
        if (auto texture = _texture_manager.get(texture_name); texture) 
        {
            _roughness = texture;
            return true;
        }

        return false;
    }

    bool Material::Builder::setRoughness(const std::string_view path_to_roughness, const std::string_view texture_name)
    {
        if (!_texture_manager.loadR(path_to_roughness, texture_name))
            return false;

        _roughness = _texture_manager.get(texture_name);

        return true;
    }

    void Material::Builder::setBakedAO(std::shared_ptr<const ImageView> baked_AO)
    {
        _baked_AO = baked_AO;
    }

    bool Material::Builder::setBakedAO(const std::string_view texture_name)
    {
        if (auto texture = _texture_manager.get(texture_name); texture) 
        {
            _baked_AO = texture;
            return true;
        }

        return false;
    }

    bool Material::Builder::setBakedAO(const std::string_view path_to_baked_AO, const std::string_view texture_name)
    {
        if (!_texture_manager.loadR(path_to_baked_AO, texture_name))
            return false;

        _baked_AO = _texture_manager.get(texture_name);

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

        material._albedo        = _albedo;
        material._normal_map    = _normal_map;
        material._metallic      = _metallic;
        material._roughness     = _roughness;
        material._baked_AO      = _baked_AO;
        material._anisotropy    = _anisotropy;

        return material;
    }

    std::unique_ptr<Material> Material::Builder::buildPtr()
    {
        assert(_albedo && _normal_map && _metallic && _roughness && _baked_AO);

        auto ptr_material = Material::make();

        ptr_material->_albedo       = _albedo;
        ptr_material->_normal_map   = _normal_map;
        ptr_material->_metallic     = _metallic;
        ptr_material->_roughness    = _roughness;
        ptr_material->_baked_AO     = _baked_AO;
        ptr_material->_anisotropy   = _anisotropy;

        return ptr_material;;
    }
}
