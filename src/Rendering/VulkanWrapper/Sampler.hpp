//
//  Sampler.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 22/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef Sampler_hpp
#define Sampler_hpp

#include "Device.hpp"

namespace pbrlib
{
    /**
     * @class SamplerInfo.
     * @brief Класс предназначенный для хранения информации о выборке.
    */
    class SamplerInfo :
        private VkSamplerCreateInfo
    {
    public:
        inline SamplerInfo() noexcept;

        /**
         * @brief Метод необходимый для задания режима фильтрации при увеличении изображения.
         * 
         * @param filter режим фильтрации.
        */
        inline void setMagFilter(VkFilter filter) noexcept;

        /**
         * @brief Метод необходимый для задания режима фильтрации при уменьшении изображения.
         * 
         * @param filter режим фильтрации.
        */
        inline void setMinFilter(VkFilter filter) noexcept;

        /**
         * @brief Метод необходимый для задания режима пирамидальной фильтрации.
         * 
         * @param mipmap_mode режим пирамидальной фильтрации.
        */
        inline void setMipmapMode(VkSamplerMipmapMode mipmap_mode) noexcept;

        /**
         * @brief 
         *      Метод необходимый для задания преобразования, 
         *      которое применяется к текстурным координатам, 
         *      оказавшимся вне изображения.
         * 
         * @param u преобразование к текстурной координате. 
         * @param v преобразование к текстурной координате. 
         * @param w преобразование к текстурной координате. 
        */
        inline void setAdressMode(VkSamplerAddressMode u = VkSamplerAddressMode::VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, 
                                  VkSamplerAddressMode v = VkSamplerAddressMode::VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, 
                                  VkSamplerAddressMode w = VkSamplerAddressMode::VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE) noexcept;

        /**
         * @brief 
         *      Метод задающий смещение, который добавляется к 
         *      вычисленному уровню детализации перед выбором 
         *      слоя в пирамиде.
         * 
         * @param mip_lod_bias смещение.
        */
        inline void setMipLodBias(float mip_lod_bias) noexcept;

        /**
         * @brief Метод позволяющий включать или отключать анизотропную фильтрацию.
         * 
         * @param is_enable флаг.
        */
        inline void anisotropyEnable(VkBool32 is_enable) noexcept;

        /**
         * @brief Метод устанавливающий максимальный уровень анизотропии.
         * 
         * @param max_anosotropy максимальный уровень анизотропии.
        */
        inline void setMaxAnisotropy(float max_anosotropy) noexcept;

        /**
         * @brief Метод включающий режим для выполнения операции сравнения.
         * 
         * @param is_enable флаг.
        */
        inline void compareEnable(VkBool32 is_enable) noexcept;

        /**
         * @brief Метод задающий операции для теста глубины.
         * 
         * @param compare_op операция сравнения.
        */
        inline void setCompareOp(VkCompareOp compare_op) noexcept;

        /**
         * @brief 
         *      Метод необходимый для задания ограничения чтения 
         *      определённым набором слоёв в пирамиде. 
         * 
         * @param min_lod наименьший слой в пирамиде.
         * @param max_lod наивысший слой в пирамиде.
        */
        inline void setLodRange(float min_lod, float max_lod) noexcept;

        /**
         * @brief Метод задающий цвет границы.
         * 
         * @param border_color цвет границы.
        */
        inline void setBorderColor(VkBorderColor border_color) noexcept;

        /**
         * @brief 
         *      Метод необходимый для того, что бы указывать в каких координатах
         *      читать из текстуры - заданных в текселях или в нормализованных ([0..1]).
         * 
         * @param unnormalized_coordinates флаг. Если VK_TRUE - то в координатах текселя, иначе в нормализованных.
        */
        inline void unnormalizedCoordinates(VkBool32 unnormalized_coordinates) noexcept;

        inline VkFilter getMagFilter() const noexcept;
        inline VkFilter getMinFilter() const noexcept;
        inline VkSamplerMipmapMode getMipmapMode() const noexcept;
        inline VkBool32 anisotropyEnable() const noexcept;
        inline float getMaxAnisotropy() const noexcept;
        inline VkBool32 compareEnable() const noexcept;
        inline VkCompareOp getCompareOp() const noexcept;
        inline float getMinLod() const noexcept;
        inline float getMaxLod() const noexcept;
        inline VkBorderColor getBorderColor() const noexcept;
        inline VkBool32 unnormalizedCoordinates() const noexcept;
    };  

    class Sampler
    {
    public:
        /**
         * @brief Конструктор.
         * 
         * @param ptr_device указатель на устройство.
         * @param sampler_info информация о создаваемой выборке.
        */
        inline Sampler(const shared_ptr<Device>& ptr_device,
                       const SamplerInfo& sampler_info);

        inline Sampler(Sampler&& sampler);
        Sampler(const Sampler&) = delete;

        inline ~Sampler();

        Sampler& operator = (Sampler&&) = delete;
        Sampler& operator = (const Sampler&) = delete;

        inline shared_ptr<Device>& getDevice() noexcept;
        inline const shared_ptr<Device>& getDevice() const noexcept;
        inline SamplerInfo& getSamplerInfo() noexcept;
        inline const SamplerInfo& getSamplerInfo() const noexcept;
        inline VkSampler getSamplerHandle() const noexcept;
        
    private:
        shared_ptr<Device> _ptr_device;
        SamplerInfo _sampler_info;
        VkSampler _sampler_handle;
    };
}

#include "Sampler.inl"

#endif /* Sampler_hpp */
