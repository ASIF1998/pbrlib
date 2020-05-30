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

    using PtrMaterial = shared_ptr<Material>;

    class Material
    {
    public:
        enum class Textures :
            size_t
        {
            Albedo = 0,
            NormalMap,
            MetallicRoughness,
            BakedAO,

            Count
        };

    public:
        class Builder
        {
        public:
            Builder();

            /**
             * @brief Метод предназначенный для установки альбедо.
             * @details
             *      Вслучае если пользователь не установил текстуру альбедо,
             *      то будет осуществляться поиск этой текстуры. Путь для поиска
             *      устанавливается с помощью метода setPathToAlbedo(...).
             * 
             * @param albedo указатель на текстуру.
            */
            void setAlbedo(const PtrImageView& albedo);

            /**
             * @brief Метод предназначенный для установки карты нормалей.
             * @details 
             *      В случае если пользователь не установил текстуру карту нормалей,
             *      то будет осуществляться поиск этой текстуры. Путь для поиска
             *      устанавливается с помощью метода setPathToNormalMap(...).
             * 
             * @param normal_map указатель на текстуру.
            */
            void setNormalMap(const PtrImageView& normal_map);

            /**
             * @brief Метод предназначенный для установки текстуры с металичностью и шероховатостью.
             * @details 
             *      В случае если пользователь не установил текстуру с металичностью 
             *      и шероховатостью, то будет осуществляться поиск этой текстуры. Путь 
             *      для поиска устанавливается с помощью метода setPathToMetallicRoughness(...).
             * 
             * @param metallic_roughness указатель на текстуру.
            */
            void setMetallicRoughness(const PtrImageView& metallic_roughness);

            /**
             * @brief Метод предназначенный для установки запечённого ambient occlusion.
             * @details 
             *      В случае если пользователь не установил текстуру с запечённой AO,
             *      то будет осуществляться поиск этой текстуры. Путь для поиска 
             *      устанавливается с помощью метода setPathToBakedAO(...).
             * 
             * @param baked_AO указатель на текстуру.
            */
            void setBakedAO(const PtrImageView& baked_AO);

            void setPathToAlbedo(const string_view path_to_albedo);
            void setPathToNormalMap(const string_view path_to_normal_map);
            void setPathToMetallicRoughness(const string_view path_to_metallic_roughness);
            void setPathToBakedAO(const string_view path_to_baked_AO);

            Material    build();
            PtrMaterial buildPtr();

        private:
            void _loading_missing_texture();

        private:    
            PtrImageView    _albedo;
            PtrImageView    _normal_map;
            PtrImageView    _metallic_roughness;
            PtrImageView    _baked_AO;
            string          _path_to_albedo;
            string          _path_to_normal_map;
            string          _path_to_metallic_roughness;
            string          _path_to_baked_AO;
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
                PtrImageView _metallic_roughness;
                PtrImageView _baked_AO;
            };

            PtrImageView _textures[util::enumCast(Textures::Count)];
        };
    };
}

#include "Material.inl"

#endif /* Material_hpp */
