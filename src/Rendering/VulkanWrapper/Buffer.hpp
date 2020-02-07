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
         * @param ptr_device указатель на устройство.
         * @param size размер буффера.
         * @param usage флаг говорящий о том, как мы собираемся использоват буффер.
         * @param memory_type_index индекс типа памяти.
         * @param queue_family_index индекс семейства очередей.
        */
        Buffer(const shared_ptr<Device>& ptr_device, 
               VkDeviceSize size, 
               VkBufferUsageFlags usage, 
               uint32_t memory_type_index, 
               uint32_t queue_family_index);

        /**
         * @brief Конструктор.
         * 
         * @param ptr_device указатель на устройство.
         * @param size размер буффера.
         * @param usage флаг говорящий о том, как мы собираемся использоват буффер.
         * @param memory_type_index индекс типа памяти.
         * @param queue_family_indices индексы семейства очередей.
        */
        Buffer(const shared_ptr<Device>& ptr_device, 
               VkDeviceSize size, 
               VkBufferUsageFlags usage, 
               uint32_t memory_type_index, 
               vector<uint32_t> queue_family_indices);

        inline ~Buffer();

        inline VkBuffer getBufferHandle() const noexcept;

    private:
        VkBuffer _buffer_handle;
        VkBufferUsageFlags _usage;
        vector<uint32_t> _queue_family_indicies;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inline Buffer::~Buffer()
    {
        vkDestroyBuffer(_ptr_device->getDeviceHandle(), _buffer_handle, nullptr);
    }

    inline VkBuffer Buffer::getBufferHandle() const noexcept
    {
        return _buffer_handle;
    }
}

#endif /* Buffer_hpp */
