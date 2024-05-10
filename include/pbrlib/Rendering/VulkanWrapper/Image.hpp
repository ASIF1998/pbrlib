//
//  Image.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 06/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef Image_hpp
#define Image_hpp

#include "DeviceMemory.hpp"

#include <vector>
#include <memory>

#include <pbrlib/Memory/STLAlignedAllocator.hpp>

#include <pbrlib/math/vec2.hpp>
#include <pbrlib/math/vec3.hpp>
#include <pbrlib/math/vec4.hpp>

#include "DeviceQueue.hpp"

#include <memory>

using namespace std;
using namespace pbrlib::math;

namespace pbrlib
{
    class Image;
    class ImageView;
    class PrimaryCommandBuffer;
    class DeviceQueue;

    using PtrImage                  = shared_ptr<Image>;
    using PtrImageView              = shared_ptr<ImageView>;
    using PtrPrimaryCommandBuffer   = shared_ptr<PrimaryCommandBuffer>;
    using PtrDeviceQueue            = shared_ptr<DeviceQueue>;

    /**
     * @struct ImageInfo.
     * @brief Структура описывающая информацию об изображении.
    */
    struct ImageInfo
    {
        VkImageType             image_type;
        VkExtent3D              image_extend;
        VkFormat                format;
        VkImageUsageFlags       usage;
        VkSampleCountFlagBits   num_samples;
        VkImageTiling           tiling;
        uint32_t                mip_levels;
        uint32_t                array_layers;
    };

    class Image
    {
        friend class Swapchain;

    public:
        enum class TexelType
        {
            R = 1,
            RG,
            RGB,
            RGBA,
            Depth,
            DepthStencil
        };

        enum class NumBits
        {
            NB8,
            NB16,
            NB32
        };

        template<TexelType TexType, typename Type, NumBits NBits>
        class Builder
        {
        public:
            inline Builder();

            Builder(Builder&&)      = delete;
            Builder(const Builder&) = delete;

            inline virtual ~Builder();

            Builder& operator = (Builder&&)         = delete;
            Builder& operator = (const Builder&)    = delete;

            inline virtual void setExtend(uint32_t width, uint32_t height, uint32_t depth)  noexcept;
            inline virtual void setExtend(const VkExtent3D& extend)                         noexcept;
            inline void         setNumSamples(VkSampleCountFlagBits num_samples)            noexcept;
            inline void         setTiling(VkImageTiling tiling)                             noexcept;
            inline void         setUsage(VkImageUsageFlags usage)                           noexcept;
            inline void         setNumMipLevels(uint32_t mip_levels)                        noexcept;
            inline void         setImageType(VkImageType image_type)                        noexcept;
            inline void         setNumArrayLayers(uint32_t array_layers)                    noexcept;
            inline void         setDevice(const PtrDevice& ptr_device)                      noexcept;
            inline void         setMemoryTypeIndex(uint32_t memory_type_index)              noexcept;

            inline void addQueueFamilyIndex(uint32_t queue_family_index) noexcept;

            inline virtual Image       build()     const;
            inline virtual PtrImage    buildPtr()  const;

        protected:
            PtrDevice           _ptr_device;
            ImageInfo           _image_info;
            uint32_t            _memory_type_index;
            vector<uint32_t>    _queue_family_indicies;
        };

        template<
            TexelType   TexType,
            typename    Type,
            NumBits     NBits,
            typename    AllocatorType = STLAlignedAllocator<Type>
        >
        class BuilderWithData :
            public Builder<TexType, Type, NBits>
        {
        public:
            inline BuilderWithData();
            inline BuilderWithData(uint32_t width, uint32_t height, uint32_t depth);
            inline BuilderWithData(const VkExtent3D& extend);

            BuilderWithData(BuilderWithData&&) = delete;
            BuilderWithData(const BuilderWithData&) = delete;

            inline virtual ~BuilderWithData();

            BuilderWithData& operator = (BuilderWithData&&)         = delete;
            BuilderWithData& operator = (const BuilderWithData&)    = delete;

            inline virtual void setExtend(uint32_t width, uint32_t height, uint32_t depth)  noexcept override;
            inline virtual void setExtend(const VkExtent3D& extend)                         noexcept;

            template<typename Container>
            void setData(const Container& data);

            void setData(const Type* ptr, size_t size);

            void setDeviceLocalMemoryTypeIndex(uint32_t memory_type_index)      noexcept;
            void setHostLocalMemoryTypeIndex(uint32_t memory_type_index)        noexcept;
            void setCommandBuffer(const PtrPrimaryCommandBuffer& ptr_command_buffer);
            void setDeviceQueue(const PtrDeviceQueue& ptr_device_queue); 

            inline void pushBack(Type r);
            inline void pushBack(const Vec2<Type>& c);
            inline void pushBack(const Vec3<Type>& c);
            inline void pushBack(const Vec4<Type>& c);

            inline void set(size_t i, Type r);
            inline void set(size_t i, const Vec2<Type>& c);
            inline void set(size_t i, const Vec3<Type>& c);
            inline void set(size_t i, const Vec4<Type>& c);

            inline void set(size_t i, size_t j, Type r);
            inline void set(size_t i, size_t j, const Vec2<Type>& c);
            inline void set(size_t i, size_t j, const Vec3<Type>& c);
            inline void set(size_t i, size_t j, const Vec4<Type>& c);

            inline virtual Image       build()     const;
            inline virtual PtrImage    buildPtr()  const;

        private:
            vector<Type, AllocatorType> _data;
            uint32_t                    _device_local_memory_type_index;
            PtrPrimaryCommandBuffer     _ptr_command_buffer;
            PtrDeviceQueue              _ptr_queue;
        };

    public:
        /**
         * @brief Конструктор.
         * @details На данный момент поддерживаются только типы float.
         * 
         * @param ptr_device            указатель на устройство.
         * @param memory_type_index     индекс типа памяти.
         * @param image_info            информация об изображении.
         * @param queue_family_index    индекс семейства очередей.
        */
        Image(
            const PtrDevice&    ptr_device,
            uint32_t            memory_type_index,
            const ImageInfo&    image_info,
            uint32_t            queue_family_index
        );

        /**
         * @brief Конструктор.
         * @details На данный момент поддерживаются только типы float.
         * 
         * @param ptr_device            указатель на устройство.
         * @param memory_type_index     индекс типа памяти.
         * @param image_info            информация об изображении.
         * @param queue_family_indices  индексы семейства очередей.
        */

        Image(
            const PtrDevice&        ptr_device,
            uint32_t                memory_type_index,
            const ImageInfo&        image_info,
            const vector<uint32_t>& queue_family_indices
        );

        /**
         * @brief Конструктор.
         * @details На данный момент поддерживаются только типы float.
         * 
         * @param ptr_device            указатель на устройство.
         * @param memory_type_index     индекс типа памяти.
         * @param image_info            информация об изображении.
         * @param queue_family_indices  индексы семейства очередей.
        */
        Image(
            const PtrDevice&    ptr_device,
            uint32_t            memory_type_index,
            const ImageInfo&    image_info,
            vector<uint32_t>&&  queue_family_indices
        );

        /**
         * @brief Конструктор.
         *
         * @param ptr_device            указатель на устройство.
         * @param image                 дескриптор уже созданного, но не прикреплённого к памяти изображения.
         * @param image_info            информация об изображении.
         * @param queue_family_index    индекс семейства очередей.
        */
        Image(
            const PtrDevice&    ptr_device,
            VkImage             image,
            ImageInfo           image_info,
            uint32_t            queue_family_index
        );

        /**
         * @brief Конструктор.
         *
         * @param ptr_device            указатель на устройство.
         * @param image                 дескриптор уже созданного, но не прикреплённого к памяти изображения.
         * @param image_info            информация об изображении.
         * @param queue_family_indicies индексы семейства очередей.
        */
        Image(
            const PtrDevice&        ptr_device,
            VkImage                 image,
            ImageInfo               image_info,
            const vector<uint32_t>& queue_family_indicies
        );

        /**
         * @brief Конструктор.
         *
         * @param ptr_device            указатель на устройство.
         * @param image                 дескриптор уже созданного, но не прикреплённого к памяти изображения.
         * @param image_info            информация об изображении.
         * @param queue_family_indicies индексы семейства очередей.
        */
        Image(
            const PtrDevice&    ptr_device,
            VkImage             image,
            ImageInfo           image_info,
            vector<uint32_t>&&  queue_family_indicies
        );

        Image(Image&& image);
        Image(const Image&) = delete;

        ~Image();

        Image& operator = (Image&&) = delete;
        Image& operator = (const Image&) = delete;

        ImageInfo&              getImageInfo();
        const ImageInfo&        getImageInfo()    const;
        const VkImage&          getImageHandle()  const;
        PtrDevice&              getDevice()       noexcept;
        const PtrDevice&        getDevice()       const noexcept;
        PtrDeviceMemory&        getDeviceMemory() noexcept;
        const PtrDeviceMemory&  getDeviceMemory() const noexcept;

        /**
         * @brief Статический метод для создания Image.
         * 
         * @param ptr_device            указатель на устройство.
         * @param memory_type_index     индекс типа памяти.
         * @param image_info            информация об изображении.
         * @param queue_family_index    индекс семейства очередей.
         * @return указатель на Image.
        */
        static PtrImage make(
            const PtrDevice&    ptr_device,
            uint32_t            memory_type_index,
            const ImageInfo&    image_info,
            uint32_t            queue_family_index
        );

        /**
         * @brief Статический метод для создания Image.
         * 
         * @param ptr_device            указатель на устройство.
         * @param memory_type_index     индекс типа памяти.
         * @param image_info            информация об изображении.
         * @param queue_family_indices  индексы семейства очередей.
         * @return указатель на Image.
        */
        static PtrImage make(
            const PtrDevice&        ptr_device,
            uint32_t                memory_type_index,
            const ImageInfo&        image_info,
            const vector<uint32_t>& queue_family_indices
        );

        /**
         * @brief Статический метод для создания Image.
         *
         * @param ptr_device            указатель на устройство.
         * @param image                 дескриптор уже созданного, но не прикреплённого к памяти изображения.
         * @param image_info            информация об изображении.
         * @param queue_family_index    индекс семейства очередей.
         * @return указатель на Image.
        */
        static PtrImage make(
            const PtrDevice&    ptr_device,
            VkImage             image,
            const ImageInfo&    image_info,
            uint32_t            queue_family_index
        );

        /**
         * @brief Статический метод для создания Image.
         *
         * @param ptr_device            указатель на устройство.
         * @param image                 дескриптор уже созданного, но не прикреплённого к памяти изображения.
         * @param image_info            информация об изображении.
         * @param queue_family_indicies индексы семейства очередей.
         * @return указатель на Image.
        */
        static PtrImage make(
            const PtrDevice&        ptr_device,
            VkImage                 image,
            const ImageInfo&        image_info,
            const vector<uint32_t>& queue_family_indicies
        );

    private:
        void _create(uint32_t memory_type_index);

    private:
        VkImage             _image_handle;
        ImageInfo           _image_info;
        vector<uint32_t>    _queue_family_indicies;
        PtrDevice           _ptr_device;
        PtrDeviceMemory     _ptr_device_memory;
    };

    class ImageView
    {
        friend class Swapchain;

    public:
        class Builder
        {
        public:
            inline void setImage(const PtrImage& ptr_image);
            inline void setFormat(VkFormat format)                     noexcept;
            inline void setAspectMask(VkImageAspectFlags aspect_mask)  noexcept;
            inline void setBaseMipLevel(uint32_t base_mip_level)       noexcept;
            inline void setLevelCount(uint32_t level_count)            noexcept;
            inline void setBaseArrayLayer(uint32_t base_array_layer)   noexcept;
            inline void setLayerCount(uint32_t layer_count)            noexcept;
            inline void setType(VkImageViewType type)                  noexcept;

            inline ImageView       build()     const;
            inline PtrImageView    buildPtr()  const;

        private:
            PtrImage                _ptr_image;
            VkFormat                _format;
            VkImageSubresourceRange _subresource_range;
            VkImageViewType         _image_view_type;
        };

    public:
        /**
         * @brief Конструктор.
         * 
         * @param ptr_image         указатель нк изображение.
         * @param format            формат текселя изображения.
         * @param subresource_range аргумент задающий подмножество изображения на которое указывает ptr_image.
         * @param type              тип вида изображения.
        */
        ImageView(
            const PtrImage&                 ptr_image,
            VkFormat                        format,
            const VkImageSubresourceRange&  subresource_range,
            VkImageViewType                 type
        );

        ImageView(ImageView&& image_view);
        ImageView(const ImageView&) = delete;

        ~ImageView();

        ImageView& operator = (ImageView&&)         = delete;
        ImageView& operator = (const ImageView&)    = delete;

        PtrImage&                       getImage()              noexcept;
        const PtrImage&                 getImage()              const noexcept;
        VkImageViewType                 getImageViewType()      const noexcept;
        VkFormat                        getFormat()             const noexcept;
        VkImageSubresourceRange&        getSubresourceRange()   noexcept;
        const VkImageSubresourceRange&  getSubresourceRange()   const noexcept;
        const VkImageView&              getImageViewHandle()    const noexcept;

    private:
        PtrImage                _ptr_image;
        VkImageView             _image_view_handle;
        VkFormat                _format;
        VkImageSubresourceRange _subresource_range;
        VkImageViewType         _type;
    };
}

#include "Image.inl"

#endif /* Image_hpp */
