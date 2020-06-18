//
//  DescriptorSet.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 27/03/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef DescriptorSet_hpp
#define DescriptorSet_hpp

#include "DescriptorPool.hpp"
#include "PipelineLayout.hpp"

#include <memory>

using namespace std;

namespace pbrlib
{
    class Sampler;
    class ImageView;
    class Buffer;
    class DescriptorSet;

    using PtrDescriptorSet = shared_ptr<DescriptorSet>;

    class DescriptorSet
    {
    public:
        /**
         * @brief Конструкор.
         * 
         * @param ptr_descriptor_pool       указатель на пул дескрипторов.
         * @param ptr_descriptor_set_layout указатель на объект размещения множества дескрипторов.
        */
        DescriptorSet(
            const PtrDescriptorPool&        ptr_descriptor_pool, 
            const PtrDescriptorSetLayout&   ptr_descriptor_set_layout
        );

        DescriptorSet(DescriptorSet&& descriptor_set);
        DescriptorSet(const DescriptorSet&) = delete;

        ~DescriptorSet();

        DescriptorSet& operator = (DescriptorSet&&)         = delete;
        DescriptorSet& operator = (const DescriptorSet&)    = delete;

        PtrDevice&                       getDevice()                 noexcept;
        const PtrDevice&                 getDevice()                 const noexcept;
        PtrDescriptorPool&               getDescriptorPool()         noexcept;
        const PtrDescriptorPool&         getDescriptorPool()         const noexcept;
        PtrDescriptorSetLayout&          getDescriptorSetLayout()    noexcept;
        const PtrDescriptorSetLayout&    getDescriptorSetLayout()    const noexcept;
        const VkDescriptorSet&           getDescriptorSetHandle()    const noexcept;

        /**
         * @brief 
         *      Метод позволяющий привязать вид изображения к множеству 
         *      дескрипторов путём записи во множество дескрипторов.
         * 
         * @param image_view        вид изображения.
         * @param image_layout      размещение.
         * @param sampler           сэмплер.
         * @param binding           индекс привязки.
         * @param descriptor_type   тип обновляемого ресурса.
        */
        void writeImageView(
            const ImageView&    image_view,
            VkImageLayout       image_layout,
            const Sampler&      sampler,
            uint32_t            binding, 
            VkDescriptorType    descriptor_type
        );

        /**
         * @brief 
         *      Метод позволяющий привязать буфер к множеству 
         *      дескрипторов путём записи во множество дескрипторов.
         * 
         * @param buffer            буфер.
         * @param offset            смещение.
         * @param range             диапазон.
         * @param binding           индекс привязки.
         * @param descriptor_type   тип обновляемого ресурса.
        */
        void writeBuffer(
            const Buffer&       buffer,
            VkDeviceSize        offset,
            VkDeviceSize        range,
            uint32_t            binding, 
            VkDescriptorType    descriptor_type
        );
        
        /**
         * @brief Статический метод, создающий указатель на DescriptorSet.
         *
         * @param ptr_descriptor_pool       указатель на пул дескрипторов.
         * @param ptr_descriptor_set_layout указатель на объект размещения множества дескрипторов.
        */
        static PtrDescriptorSet make(
             const PtrDescriptorPool&        ptr_descriptor_pool,
             const PtrDescriptorSetLayout&   ptr_descriptor_set_layout
         );

    private:
        PtrDescriptorPool       _ptr_descriptor_pool;
        PtrDescriptorSetLayout  _ptr_descriptor_set_layout;
        VkDescriptorSet         _descriptor_set_handle;
    };
}

#endif /* DescriptorSet_hpp */
