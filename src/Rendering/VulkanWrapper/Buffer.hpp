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

#include <vector>
#include <memory>
#include "../../Memory/STLAlignedAllocator.hpp"

using namespace std;

namespace pbrlib
{
    class DeviceMemory;
    class Buffer;

    using PtrBuffer = shared_ptr<Buffer>;

    class Buffer :
        public DeviceMemory
    {
    public:
        template<typename Type>
        class Builder
        {
        public:
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
            inline void setDevice(const PtrDevice& ptr_device);

            /**
             * @brief Метод устанавливающий индекс типа памяти.
             * 
             * @param memory_type_index индекс типа памяти.
            */
            inline void setMemoryTypeIndex(uint32_t memory_type_index);

            inline virtual Buffer      build()     const;
            inline virtual PtrBuffer   buildPtr()  const;

        protected:
            PtrDevice           _ptr_device;
            vector<uint32_t>    _queue_family_indicies;
            size_t              _size;
            VkBufferUsageFlags  _usage;
            uint32_t            _memory_type_index;
        };

        template<typename Type, typename AllocatorType = STLAlignedAllocator<Type>>
        class BuilderWithData :
            public Builder<Type>
        {
        public:
            inline BuilderWithData() = default;
            inline BuilderWithData(size_t n);

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

            inline virtual Buffer      build()     const;
            inline virtual PtrBuffer   buildPtr()  const;

        private:
            vector<Type, AllocatorType> _data;
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
            const PtrDevice&    ptr_device, 
            VkDeviceSize        size, 
            VkBufferUsageFlags  usage, 
            uint32_t            memory_type_index, 
            uint32_t            queue_family_index
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
            const PtrDevice&    ptr_device, 
            VkDeviceSize        size, 
            VkBufferUsageFlags  usage, 
            uint32_t            memory_type_index, 
            vector<uint32_t>    queue_family_indices
        );

        Buffer(Buffer&& buffer);
        Buffer(const Buffer&) = delete;

        ~Buffer();

        Buffer& operator = (Buffer&&)       = delete;
        Buffer& operator = (const Buffer&)  = delete;

        const VkBuffer& getBufferHandle() const noexcept;

    private:
        VkBuffer            _buffer_handle;
        VkBufferUsageFlags  _usage;
        vector<uint32_t>    _queue_family_indicies;
    };
}

#include "Buffer.inl"

#endif /* Buffer_hpp */
