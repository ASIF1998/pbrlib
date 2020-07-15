//
//  Material.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 29/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef Material_hpp
#define Material_hpp

#include <string>
#include <string_view>

#include "../VulkanWrapper/Image.hpp"

#include "../../Util/enumCast.hpp"

namespace pbrlib
{
    class Material;
    class GPUTextureManager;

    using PtrMaterial = shared_ptr<Material>;

    class Material
    {
    public:
        enum class Textures :
            size_t
        {
            Albedo = 0,
            NormalMap,
            Metallic,
            Roughness,
            BakedAO,

            Count
        };

    public:
        class Builder
        {
        public:
            Builder(GPUTextureManager& texture_manager);

            /**
             * @brief Метод предназначенный для установки альбедо.
             * 
             * @param albedo указатель на текстуру.
            */
            void setAlbedo(const PtrImageView& albedo);

            /**
             * @brief Метод, предназначенный для установки альбедо.
             * @details Поиск текстуры будет осуществляться в менеджере текстур.
             * 
             * @param texture_name название текстуры.
             * @return false - если не удалось найти текстуру.
            */
            bool setAlbedo(const string_view texture_name);

            /**
             * @brief Метод, предназначенный для загрузки и установки альбедо.
             * @details После загрузки текстуры, она будет добавлена в менеджер текстур.
             * 
             * @param path_to_albedo    путь до текстуры с альбедо.
             * @param texture_name      наименование текстуры альбедо.
             * @return false - не удалось загрузить текстуру.
            */
            bool setAlbedo(const string_view path_to_albedo, const string_view texture_name);

            /**
             * @brief Метод, предназначенный для установки карты нормалей.
             * 
             * @param normal_map указатель на текстуру.
            */
            void setNormalMap(const PtrImageView& normal_map);

            /**
             * @brief Метод, предназначенный для установки карты нормалей.
             * @details Поиск текстуры будет осуществляться в менеджере текстур.
             * 
             * @param texture_name наименование текстуры.
             * @return false - если не удалось найти текстуру.
            */
            bool setNormalMap(const string_view texture_name);

            /**
             * @brief Метод, предназначенный для загрузки и установки карты нормалей.
             * @details После загрузки текстуры, она будет добавлена в менеджер текстур.
             * 
             * @param path_to_normal_map    путь до текстуры с картой нормалей.
             * @param texture_name          наименование текстуры.
             * @return false - не удалось загрузить текстуру.
            */
            bool setNormalMap(const string_view path_to_normal_map, const string_view texture_name);

            /**
             * @brief Метод, предназначенный для установки текстуры с металичностью.
             * 
             * @param metallic указатель на текстуру.
            */
            void setMetallic(const PtrImageView& metallic);

            /**
             * @brief Метод, предназначенный для установки текстуры с металичностью.
             * @details Поиск текстуры будет осуществляться в менеджере текстур.
             * 
             * @param texture_name наименование текстуры.
             * @return false - если не удалось найти текстуру.
            */
            bool setMetallic(const string_view texture_name);

            /**
             * @brief Метод, предназначенный для загрузки и установки текстуры с металичностью.
             * @details После загрузки текстуры, она будет добавлена в менеджер текстур.
             * 
             * @param path_to_metallic  путь до текстуры с металичностью.
             * @param texture_name      наименование текстуры.
             * @return false - не удалось загрузить текстуру.
            */
            bool setMetallic(const string_view path_to_metallic, const string_view texture_name);

            /**
             * @brief Метод, предназначенный для установки текстуры с шероховатостью.
             * 
             * @param roughness указатель на текстуру.
            */
            void setRoughness(const PtrImageView& roughness);

            /**
             * @brief Метод, предназначенный для установки текстуры с шероховатостью.
             * @details Поиск текстуры будет осуществляться в менеджере текстур.
             * 
             * @param texture_name наименование текстуры.
             * @return false - если не удалось найти текстуру.
            */
            bool setRoughness(const string_view texture_name);

            /**
             * @brief Метод, предназначенный для загрузки и установки текстуры с шероховатостью.
             * @details После загрузки текстуры, она будет добавлена в менеджер текстур.
             * 
             * @param path_to_roughness     путь до текстуры с шероховатостью.
             * @param texture_name          наименование текстуры.
             * @return false - не удалось загрузить текстуру.
            */
            bool setRoughness(const string_view path_to_roughness, const string_view texture_name);

            /**
             * @brief Метод, предназначенный для установки запечённого ambient occlusion.
             * 
             * @param baked_AO указатель на текстуру.
            */
            void setBakedAO(const PtrImageView& baked_AO);

            /**
             * @brief Метод, предназначенный для установки текстуры с запечённой ambient occlusion.
             * @details Поиск текстуры будет осуществляться в менеджере текстур.
             * 
             * @param texture_name наименование текстуры.
             * @return false - если не удалось найти текстуру.
            */
            bool setBakedAO(const string_view texture_name);

            /**
             * @brief Метод, предназначенный для загрузки и установки текстуры с запечённой ambient occlusion.
             * @details После загрузки текстуры, она будет добавлена в менеджер текстур.
             * 
             * @param path_to_baked_AO  путь до текстуры с запечённой ambient occlusion.
             * @param texture_name      наименование текстуры.
             * @return false - не удалось загрузить текстуру.
            */
            bool setBakedAO(const string_view path_to_baked_AO, const string_view texture_name);

            Material    build();
            PtrMaterial buildPtr();

        private:    
            PtrImageView    _albedo;
            PtrImageView    _normal_map;
            PtrImageView    _metallic;
            PtrImageView    _roughness;
            PtrImageView    _baked_AO;

            GPUTextureManager& _texture_manager;
        };

    public:
        Material();
        Material(const Material& material);
        Material(Material&& material);
        ~Material();

        template<Textures TextureType>
        void setTexture(const PtrImageView& ptr_image_view);

        void setTexture(size_t i, const PtrImageView& ptr_image_view);

        template<Textures TextureType>
        PtrImageView& getTexture();

        template<Textures TextureType>
        const PtrImageView& getTexture() const;

        PtrImageView&       getTexture(size_t i);
        const PtrImageView& getTexture(size_t i) const;

        static PtrMaterial make();

    private:
        union
        {
            struct 
            {
                PtrImageView _albedo;
                PtrImageView _normal_map;
                PtrImageView _metallic;
                PtrImageView _roughness;
                PtrImageView _baked_AO;
            };

            PtrImageView _textures[util::enumCast(Textures::Count)];
        };
    };
}

#include "Material.inl"

#endif /* Material_hpp */
