//
//  CommandPool.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 04/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef CommandPool_hpp
#define CommandPool_hpp

#include <memory>

#include "Device.hpp"

using namespace std;

namespace pbrlib
{
    class Device;

    class CommandPool
    {
    public:
        /**
         * @brief Конструктор.
         * 
         * @param ptr_device указатель на устройство. 
         * @param queue_family_index индекс семейства очередей.
        */
        CommandPool(const shared_ptr<Device>& ptr_device, uint32_t queue_family_index);
        inline ~CommandPool();

        inline shared_ptr<Device>& getDevice() noexcept;
        inline const shared_ptr<Device>& getDevice() const noexcept;
        inline uint32_t getFamilyIndex() const noexcept;
        inline VkCommandPool getCommandPoolHandle() const noexcept;

    private:
        shared_ptr<Device> _ptr_device;
        VkCommandPool _command_pool_handle;
        uint32_t _queue_family_index;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inline CommandPool::~CommandPool()
    {
        vkDestroyCommandPool(_ptr_device->getDeviceHandle(), _command_pool_handle, nullptr);
    }

    inline shared_ptr<Device>& CommandPool::getDevice() noexcept
    {
        return _ptr_device;
    }

    inline const shared_ptr<Device>& CommandPool::getDevice() const noexcept
    {
        return _ptr_device;
    }

    inline uint32_t CommandPool::getFamilyIndex() const noexcept
    {
        return _queue_family_index;
    }

    inline VkCommandPool CommandPool::getCommandPoolHandle() const noexcept
    {
        return _command_pool_handle;
    }
}

#endif /* CommandPool_hpp */
