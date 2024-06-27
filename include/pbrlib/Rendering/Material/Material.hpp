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

#include <pbrlib/Rendering/VulkanWrapper/Image.hpp>
#include <pbrlib/Util/enumCast.hpp>

namespace pbrlib
{
    class GPUTextureManager;

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
            void setAlbedo(std::shared_ptr<const ImageView> albedo);

            /**
             * @brief Метод, предназначенный для установки альбедо.
             * @details Поиск текстуры будет осуществляться в менеджере текстур.
             * 
             * @param texture_name название текстуры.
             * @return false - если не удалось найти текстуру.
            */
            bool setAlbedo(const std::string_view texture_name);

            /**
             * @brief Метод, предназначенный для загрузки и установки альбедо.
             * @details После загрузки текстуры, она будет добавлена в менеджер текстур.
             * 
             * @param path_to_albedo    путь до текстуры с альбедо.
             * @param texture_name      наименование текстуры альбедо.
             * @return false - не удалось загрузить текстуру.
            */
            bool setAlbedo(const std::string_view path_to_albedo, const std::string_view texture_name);

            /**
             * @brief Метод, предназначенный для установки карты нормалей.
             * 
             * @param normal_map указатель на текстуру.
            */
            void setNormalMap(std::shared_ptr<const ImageView> normal_map);

            /**
             * @brief Метод, предназначенный для установки карты нормалей.
             * @details Поиск текстуры будет осуществляться в менеджере текстур.
             * 
             * @param texture_name наименование текстуры.
             * @return false - если не удалось найти текстуру.
            */
            bool setNormalMap(const std::string_view texture_name);

            /**
             * @brief Метод, предназначенный для загрузки и установки карты нормалей.
             * @details После загрузки текстуры, она будет добавлена в менеджер текстур.
             * 
             * @param path_to_normal_map    путь до текстуры с картой нормалей.
             * @param texture_name          наименование текстуры.
             * @return false - не удалось загрузить текстуру.
            */
            bool setNormalMap(const std::string_view path_to_normal_map, const std::string_view texture_name);

            /**
             * @brief Метод, предназначенный для установки текстуры с металичностью.
             * 
             * @param metallic указатель на текстуру.
            */
            void setMetallic(std::shared_ptr<const ImageView> metallic);

            /**
             * @brief Метод, предназначенный для установки текстуры с металичностью.
             * @details Поиск текстуры будет осуществляться в менеджере текстур.
             * 
             * @param texture_name наименование текстуры.
             * @return false - если не удалось найти текстуру.
            */
            bool setMetallic(const std::string_view texture_name);

            /**
             * @brief Метод, предназначенный для загрузки и установки текстуры с металичностью.
             * @details После загрузки текстуры, она будет добавлена в менеджер текстур.
             * 
             * @param path_to_metallic  путь до текстуры с металичностью.
             * @param texture_name      наименование текстуры.
             * @return false - не удалось загрузить текстуру.
            */
            bool setMetallic(const std::string_view path_to_metallic, const std::string_view texture_name);

            /**
             * @brief Метод, предназначенный для установки текстуры с шероховатостью.
             * 
             * @param roughness указатель на текстуру.
            */
            void setRoughness(std::shared_ptr<const ImageView> roughness);

            /**
             * @brief Метод, предназначенный для установки текстуры с шероховатостью.
             * @details Поиск текстуры будет осуществляться в менеджере текстур.
             * 
             * @param texture_name наименование текстуры.
             * @return false - если не удалось найти текстуру.
            */
            bool setRoughness(const std::string_view texture_name);

            /**
             * @brief Метод, предназначенный для загрузки и установки текстуры с шероховатостью.
             * @details После загрузки текстуры, она будет добавлена в менеджер текстур.
             * 
             * @param path_to_roughness     путь до текстуры с шероховатостью.
             * @param texture_name          наименование текстуры.
             * @return false - не удалось загрузить текстуру.
            */
            bool setRoughness(const std::string_view path_to_roughness, const std::string_view texture_name);

            /**
             * @brief Метод, предназначенный для установки запечённого ambient occlusion.
             * 
             * @param baked_AO указатель на текстуру.
            */
            void setBakedAO(std::shared_ptr<const ImageView> baked_AO);

            /**
             * @brief Метод, предназначенный для установки текстуры с запечённой ambient occlusion.
             * @details Поиск текстуры будет осуществляться в менеджере текстур.
             * 
             * @param texture_name наименование текстуры.
             * @return false - если не удалось найти текстуру.
            */
            bool setBakedAO(const std::string_view texture_name);

            /**
             * @brief Метод, предназначенный для загрузки и установки текстуры с запечённой ambient occlusion.
             * @details После загрузки текстуры, она будет добавлена в менеджер текстур.
             * 
             * @param path_to_baked_AO  путь до текстуры с запечённой ambient occlusion.
             * @param texture_name      наименование текстуры.
             * @return false - не удалось загрузить текстуру.
            */
            bool setBakedAO(const std::string_view path_to_baked_AO, const std::string_view texture_name);

            /**
             * @brief Метод, позволяющий установить значение анизотропности материала.
             * @details anisotropy >= -1.0f && anisotropy <= 1.0f.
             * 
             * @param anisotropy анизотропность.
            */
            void setAnisotropy(float anisotropy);

            Material                    build();
            std::unique_ptr<Material>   buildPtr();

        private:    
            std::shared_ptr<const ImageView> _albedo;
            std::shared_ptr<const ImageView> _normal_map;
            std::shared_ptr<const ImageView> _metallic;
            std::shared_ptr<const ImageView> _roughness;
            std::shared_ptr<const ImageView> _baked_AO;

            float _anisotropy;

            GPUTextureManager& _texture_manager;
        };

    public:
        Material();
        Material(const Material& material);
        Material(Material&& material);
        ~Material();

        template<Textures TextureType>
        void setTexture(std::shared_ptr<const ImageView> ptr_image_view);

        void setTexture(size_t i, std::shared_ptr<const ImageView> ptr_image_view);
        void setAnisotropy(float anisotropy);

        template<Textures TextureType>
        std::shared_ptr<const ImageView> getTexture() const;

        std::shared_ptr<const ImageView>    getTexture(size_t i)    const;
        float                               getAnisotropy()         const noexcept;

        static std::unique_ptr<Material> make();

    private:
        union
        {
            struct 
            {
                std::shared_ptr<const ImageView> _albedo;
                std::shared_ptr<const ImageView> _normal_map;
                std::shared_ptr<const ImageView> _metallic;
                std::shared_ptr<const ImageView> _roughness;
                std::shared_ptr<const ImageView> _baked_AO;
            };

            std::shared_ptr<const ImageView> _textures[utils::enumCast(Textures::Count)];
        };

        float _anisotropy;
    };
}

#include "Material.inl"

#endif /* Material_hpp */
