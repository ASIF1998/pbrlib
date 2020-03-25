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

using namespace std;

namespace pbrlib
{
    class Buffer :
        public DeviceMemory
    {
    public:
        /**
         * @brief Конструктор.
         * 
         * @param ptr_device            указатель на устройство.
         * @param size                  размер буффера.
         * @param usage                 флаг говорящий о том, как мы собираемся использоват буффер.
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
         * @param size                  размер буффера.
         * @param usage                 флаг говорящий о том, как мы собираемся использоват буффер.
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

        inline Buffer(Buffer&& buffer);
        inline Buffer(const Buffer&) = delete;

        inline ~Buffer();

        Buffer& operator = (Buffer&&)       = delete;
        Buffer& operator = (const Buffer&)  = delete;

        inline const VkBuffer& getBufferHandle() const noexcept;

    private:
        VkBuffer            _buffer_handle;
        VkBufferUsageFlags  _usage;
        vector<uint32_t>    _queue_family_indicies;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inline Buffer::Buffer(Buffer&& buffer) :
        DeviceMemory            (move(buffer)),
        _buffer_handle          (VK_NULL_HANDLE),
        _usage                  (buffer._usage),
        _queue_family_indicies  (move(buffer._queue_family_indicies))
    {
        swap(_buffer_handle, buffer._buffer_handle);
    }

    inline Buffer::~Buffer()
    {
        if (_buffer_handle != VK_NULL_HANDLE) {
            vkDestroyBuffer(_ptr_device->getDeviceHandle(), _buffer_handle, nullptr);
        }
    }

    inline const VkBuffer& Buffer::getBufferHandle() const noexcept
    {
        return _buffer_handle;
    }
}

#endif /* Buffer_hpp */
