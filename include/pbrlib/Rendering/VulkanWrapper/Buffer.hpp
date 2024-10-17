//
//  Buffer.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 05/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef Buffer_hpp
#define Buffer_hpp

#include "DeviceMemory.hpp"
#include <pbrlib/memory/stl_aligned_allocator.hpp>

#include <vector>
#include <span>
#include <memory>

namespace pbrlib
{
    class Buffer
    {
    public:
        template<typename Type>
        class Builder
        {
        public:
            Builder() = default;
            
            Builder(Builder&&)      = delete;
            Builder(const Builder&) = delete;

            Builder& operator = (Builder&&)        = delete;
            Builder& operator = (const Builder&)   = delete;

            /**
             * @brief   Метод устанавливающий размер буфера.
             * @details Окончательный размер буфера равняется sizeof(Type) * size.
             * 
             * @param size размер буфера.
            */
            inline virtual void setSize(size_t size);

            /**
             * @brief Метод устанавливающий флаг говорящий о том, как мы собираемся использовать буфер.
             * 
             * @param usage флаг говорящий о том, как мы собираемся использовать буфер.
            */
            inline void setUsage(VkBufferUsageFlags usage);

            /**
             * @brief Метод позволяющий добавлять индекс семейства очередей.
             * 
             * @param queue_family индекс семейства очередей.
            */
            inline void addQueueFamily(uint32_t queue_family);

            /**
             * @brief Метод устанавливающий указатель на устройство.
             * 
             * @param ptr_device указатель на устройство.
            */
            inline void setDevice(const Device* ptr_device);

            /**
             * @brief Метод устанавливающий индекс типа памяти.
             * 
             * @param memory_type_index индекс типа памяти.
            */
            inline void setMemoryTypeIndex(uint32_t memory_type_index);

            inline virtual Buffer                   build()     const;
            inline virtual std::unique_ptr<Buffer>  buildPtr()  const;

        protected:
            const Device*           _ptr_device;
            std::vector<uint32_t>   _queue_family_indicies;
            size_t                  _size;
            VkBufferUsageFlags      _usage;
            uint32_t                _memory_type_index;
        };

        template<typename Type, typename AllocatorType = STLAlignedAllocator<Type>>
        class BuilderWithData :
            public Builder<Type>
        {
        public:
            inline BuilderWithData() = default;
            inline BuilderWithData(size_t n);

            BuilderWithData(BuilderWithData&&)      = delete;
            BuilderWithData(const BuilderWithData&) = delete;

            BuilderWithData& operator = (BuilderWithData&&)      = delete;
            BuilderWithData& operator = (const BuilderWithData&) = delete;

            /**
             * @brief   Метод устанавливающий размер буфера.
             * @details Окончательный размер буфера равняется sizeof(Type) * size.
             * 
             * @param size размер буфера.
            */
            inline virtual void setSize(size_t size);
            inline void         reserve(size_t size);
            
            inline void pushBack(Type v);

            template<typename Container>
            inline void setData(const Container& data);

            inline void setData(const Type* ptr, size_t size);

            inline size_t getSize() const;

            inline void addData(const Type& data);
            inline void addData(Type&& data);

            inline Type& operator [] (size_t i) noexcept;

            inline virtual Buffer                   build()     const;
            inline virtual std::unique_ptr<Buffer>  buildPtr()  const;

        private:
            std::vector<Type, AllocatorType> _data;
        };

    public:
        /**
         * @brief Конструктор.
         * 
         * @param ptr_device            указатель на устройство.
         * @param size                  размер буфера.
         * @param usage                 флаг говорящий о том, как мы собираемся использовать буфер.
         * @param memory_type_index     индекс типа памяти.
         * @param queue_family_index    индекс семейства очередей.
        */
        Buffer(
            const Device*                   ptr_device, 
            VkDeviceSize                    size, 
            VkBufferUsageFlags              usage, 
            uint32_t                        memory_type_index, 
            uint32_t                        queue_family_index
        );

        /**
         * @brief Конструктор.
         * 
         * @param ptr_device            указатель на устройство.
         * @param size                  размер буфера.
         * @param usage                 флаг говорящий о том, как мы собираемся использовать буфер.
         * @param memory_type_index     индекс типа памяти.
         * @param queue_family_indices  индексы семейства очередей.
        */
        Buffer(
            const Device*                   ptr_device, 
            VkDeviceSize                    size, 
            VkBufferUsageFlags              usage, 
            uint32_t                        memory_type_index, 
            std::span<const uint32_t>       queue_family_indices
        );

        Buffer(Buffer&& buffer);
        Buffer(const Buffer&) = delete;

        ~Buffer();

        Buffer& operator = (Buffer&&)       = delete;
        Buffer& operator = (const Buffer&)  = delete;

        const VkBuffer&                 getBufferHandle()   const noexcept;
        const Device*   getDevice()         const noexcept;
        DeviceMemory*                   getDeviceMemory()   noexcept;
        const DeviceMemory*             getDeviceMemory()   const noexcept;
        VkDeviceSize                    getSize()           const noexcept;

    private:
        VkBuffer                        _buffer_handle;
        VkBufferUsageFlags              _usage;
        std::vector<uint32_t>           _queue_family_indicies;
        const Device*                   _ptr_device;
        std::unique_ptr<DeviceMemory>   _ptr_device_memory;
        VkDeviceSize                    _size;
    };
}

#include "Buffer.inl"

#endif /* Buffer_hpp */
