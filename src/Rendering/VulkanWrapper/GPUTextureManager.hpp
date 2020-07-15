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
    class PrimaryCommandBuffer;
    class DeviceQueue;

    using PtrPrimaryCommandBuffer   = shared_ptr<PrimaryCommandBuffer>;
    using PtrDeviceQueue            = shared_ptr<DeviceQueue>;

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
         * @param ptr_device                        указатель на устройство.
         * @param ptr_command_pool                  указатель на командный пул.
         * @param device_local_memory_type_index    тип памяти, являющаяся локальной для устройства (GPU).
         * @param host_local_memory_type_index      тип памяти, которая может быть отображена и читаться или записываться CPU.
         * @param ptr_device_queue                  указатель на очередь устройства.
         * @param image_tiling                      тип размещения изображения.
         * @param num_samples                       количество образцов в изображении.
        */
        GPUTextureManager(
            const PtrDevice&        ptr_device,
            const PtrCommandPool&   ptr_command_pool,
            uint32_t                device_local_memory_type_index,
            uint32_t                host_local_memory_type_index,
            PtrDeviceQueue          ptr_device_queue,
            VkImageTiling           image_tiling,
            VkSampleCountFlags      num_samples
        );

        /**
         * @brief 
         *      Метод необходимый для загрузки текстуры из файла.
         *      Тексель будет иметь только один красный канал.
         * 
         * @param path путь до файла.
         * @param name название текстуры.
         * @return true - если получилось загрузить текстуру.
        */
        bool loadR(const string_view path, const string_view name);

        /**
         * @brief 
         *      Метод необходимый для загрузки текстуры из файла.
         *      Тексель будет иметь красный и зелёный каналы.
         * 
         * @param path путь до файла.
         * @param name название текстуры.
         * @return true - если получилось загрузить текстуру.
        */
        bool loadRG(const string_view path, const string_view name);

        /**
         * @brief 
         *      Метод необходимый для загрузки текстуры из файла.
         *      Тексель будет иметь красный, зелёный и синий каналы.
         * 
         * @param path путь до файла.
         * @param name название текстуры.
         * @return true - если получилось загрузить текстуру.
        */
        bool loadRGB(const string_view path, const string_view name);

        /**
         * @brief 
         *      Метод необходимый для загрузки текстуры из файла.
         *      Тексель будет иметь красный, зелёный, синий и альфа-каналы.
         * 
         * @param path путь до файла.
         * @param name название текстуры.
         * @return true - если получилось загрузить текстуру.
        */
        bool loadRGBA(const string_view path, const string_view name);

        bool add(const PtrImageView& ptr_image_view, const string_view name);
        bool has(const string_view name);

        optional<PtrImageView>          get(const string_view name);
        optional<const PtrImageView>    get(const string_view name) const;

    private:
        unordered_map<string, PtrImageView> _textures;                          //!< Текстуры.
        PtrDevice                           _ptr_device;                        //!< Указатель на устройство.
        PtrPrimaryCommandBuffer             _ptr_command_buffer;                //!< Указатель на командный буфер.
        uint32_t                            _device_local_memory_type_index;    //!< Тип памяти, являющаяся локальной для устройства (GPU).
        uint32_t                            _host_local_memory_type_index;      //!< Тип памяти, которая может быть отображена и читаться или записываться CPU.
        PtrDeviceQueue                      _ptr_device_queue;                  //!< Указатель на очередь устройства.
        VkImageTiling                       _image_tiling;                      //!< Способ размещения текстуры на устройстве.
        VkSampleCountFlags                  _num_samples;                       //!< Количество выборок в текстуре на устройстве.
    };
}

#endif /* GPUTextureManager_hpp */
