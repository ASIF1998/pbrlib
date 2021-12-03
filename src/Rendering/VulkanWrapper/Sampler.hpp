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
    class Sampler;

    using PtrSampler = shared_ptr<Sampler>;

    /**
     * @class SamplerInfo.
     * @brief Класс предназначенный для хранения информации о выборке.
    */
    class SamplerInfo
    {
    public:
        SamplerInfo() noexcept;

        SamplerInfo(const SamplerInfo&) = default;

        SamplerInfo& operator = (SamplerInfo&&)         = delete;
        SamplerInfo& operator = (const SamplerInfo&)    = delete;

        /**
         * @brief Метод необходимый для задания режима фильтрации при увеличении изображения.
         * 
         * @param filter режим фильтрации.
        */
        void setMagFilter(VkFilter filter) noexcept;

        /**
         * @brief Метод необходимый для задания режима фильтрации при уменьшении изображения.
         * 
         * @param filter режим фильтрации.
        */
        void setMinFilter(VkFilter filter) noexcept;

        /**
         * @brief Метод необходимый для задания режима пирамидальной фильтрации.
         * 
         * @param mipmap_mode режим пирамидальной фильтрации.
        */
        void setMipmapMode(VkSamplerMipmapMode mipmap_mode) noexcept;

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
        void setAdressMode(
            VkSamplerAddressMode u = VkSamplerAddressMode::VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, 
            VkSamplerAddressMode v = VkSamplerAddressMode::VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, 
            VkSamplerAddressMode w = VkSamplerAddressMode::VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE
        ) noexcept;

        /**
         * @brief 
         *      Метод задающий смещение, который добавляется к 
         *      вычисленному уровню детализации перед выбором 
         *      слоя в пирамиде.
         * 
         * @param mip_lod_bias смещение.
        */
        void setMipLodBias(float mip_lod_bias) noexcept;

        /**
         * @brief Метод позволяющий включать или отключать анизотропную фильтрацию.
         * 
         * @param is_enable флаг.
        */
        void anisotropyEnable(VkBool32 is_enable) noexcept;

        /**
         * @brief Метод устанавливающий максимальный уровень анизотропии.
         * 
         * @param max_anisotropy максимальный уровень анизотропии.
        */
        void setMaxAnisotropy(float max_anisotropy) noexcept;

        /**
         * @brief Метод включающий режим для выполнения операции сравнения.
         * 
         * @param is_enable флаг.
        */
        void compareEnable(VkBool32 is_enable) noexcept;

        /**
         * @brief Метод задающий операции для теста глубины.
         * 
         * @param compare_op операция сравнения.
        */
        void setCompareOp(VkCompareOp compare_op) noexcept;

        /**
         * @brief 
         *      Метод необходимый для задания ограничения чтения 
         *      определённым набором слоёв в пирамиде. 
         * 
         * @param min_lod наименьший слой в пирамиде.
         * @param max_lod наивысший слой в пирамиде.
        */
        void setLodRange(float min_lod, float max_lod) noexcept;

        /**
         * @brief Метод задающий цвет границы.
         * 
         * @param border_color цвет границы.
        */
        void setBorderColor(VkBorderColor border_color) noexcept;

        /**
         * @brief 
         *      Метод необходимый для того, что бы указывать в каких координатах
         *      читать из текстуры - заданных в текселях или в нормализованных ([0..1]).
         * 
         * @param unnormalized_coordinates флаг. Если VK_TRUE - то в [0, tex_width] и [0, tex_height], иначе в нормализованных.
        */
        void unnormalizedCoordinates(VkBool32 unnormalized_coordinates) noexcept;

        const VkSamplerCreateInfo&  getSamplerCreateInfo()      const noexcept;
        VkSamplerCreateInfo&        getSamplerCreateInfo()      noexcept; 
        VkFilter                    getMagFilter()              const noexcept;
        VkFilter                    getMinFilter()              const noexcept;
        VkSamplerMipmapMode         getMipmapMode()             const noexcept;
        VkBool32                    anisotropyEnable()          const noexcept;
        float                       getMaxAnisotropy()          const noexcept;
        VkBool32                    compareEnable()             const noexcept;
        VkCompareOp                 getCompareOp()              const noexcept;
        float                       getMinLod()                 const noexcept;
        float                       getMaxLod()                 const noexcept;
        VkBorderColor               getBorderColor()            const noexcept;
        VkBool32                    unnormalizedCoordinates()   const noexcept;

    private:
        VkSamplerCreateInfo _sampler_ci;
    };  

    class Sampler
    {
    public:
        class Builder :
            public SamplerInfo
        {
        public:
            Builder();

            Builder(Builder&&)      = delete;
            Builder(const Builder&) = delete;

            Builder& operator = (Builder&&)      = delete;
            Builder& operator = (const Builder&) = delete;

            void setDevice(const PtrDevice& ptr_device);

            Sampler     build()     const;
            PtrSampler  buildPtr()  const;

        private:
            PtrDevice _ptr_device;
        };

    public:
        /**
         * @brief Конструктор.
         * 
         * @param ptr_device    указатель на устройство.
         * @param sampler_info  информация о создаваемой выборке.
        */
        Sampler(
            const PtrDevice&            ptr_device,
            const SamplerInfo&          sampler_info
        );

        Sampler(Sampler&& sampler);
        Sampler(const Sampler&) = delete;

        ~Sampler();

        Sampler& operator = (Sampler&&)         = delete;
        Sampler& operator = (const Sampler&)    = delete;

        PtrDevice&           getDevice()         noexcept;
        const PtrDevice&     getDevice()         const noexcept;
        SamplerInfo&         getSamplerInfo()    noexcept;
        const SamplerInfo&   getSamplerInfo()    const noexcept;
        const VkSampler&     getSamplerHandle()  const noexcept;
        
    private:
        PtrDevice   _ptr_device;
        SamplerInfo _sampler_info;
        VkSampler   _sampler_handle;
    };
}

#endif /* Sampler_hpp */
