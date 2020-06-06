//
//  GPUTextureManager.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 02/06/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef GPUTextureManager_hpp
#define GPUTextureManager_hpp

#include <unordered_map>

#include <string>
#include <string_view>

#include <optional>

#include "Image.hpp"

using namespace std;

namespace pbrlib
{
    /**
     * @class GPUTextureManager.
     * @brief 
     *      Данный класс необходим для загрузки текстур,
     *      не приводящей к ненужной повторной загрузке.
    */
    class GPUTextureManager
    {
    public:
        /**
         * @brief Конструктор.
         * 
         * @param ptr_device            указатель на устройство.
         * @param memory_type_index     тип памяти на устройстве.
         * @param image_tiling          способ размещение текстуры на устройстве.
         * @param num_samples           количество выборок на устройстве.
        */
        GPUTextureManager(
            const PtrDevice&    ptr_device,
            uint32_t            memory_type_index,
            VkImageTiling       image_tiling,
            VkSampleCountFlags  num_samples
        );

        /**
         * @brief Метод необходимый для загрузки текстуры из файла.
         * @details 
         *      Если текстура с таким именем уже существует, то
         *      загрузка не будет осуществлена.
         * 
         * @param path путь до файла с текстурой.
         * @param name название текстуры.
         * @return true - если получилось загрузить текстуру.
        */
        bool load(const string_view path, const string_view name);

        bool add(const PtrImageView& ptr_image_view, const string_view name);

        bool has(const string_view name);

        optional<PtrImageView>          get(const string_view name);
        optional<const PtrImageView>    get(const string_view name) const;

    private:
        unordered_map<string, PtrImageView> _textures;          //!< Текстуры.
        PtrDevice                           _ptr_device;        //!< Указатель на устройство.
        uint32_t                            _memory_type_index; //!< Индекс типа памяти на устройстве.
        VkImageTiling                       _image_tiling;      //!< Способ размещения текстуры на устройстве.
        VkSampleCountFlags                  _num_samples;       //!< Количество выборок в текстуре на устройстве.
    };
}

#endif /* GPUTextureManager_hpp */
