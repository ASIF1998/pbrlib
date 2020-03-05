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

/**
 * TODO: Добавить функцию создающую командный буфер.
*/

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
         * @param ptr_device            указатель на устройство. 
         * @param queue_family_index    индекс семейства очередей.
        */
        CommandPool(const shared_ptr<Device>& ptr_device, uint32_t queue_family_index);

        inline CommandPool(CommandPool&& command_pool);
        CommandPool(const CommandPool&) = delete;

        inline ~CommandPool();

        CommandPool& operator = (CommandPool&&)         = delete;
        CommandPool& operator = (const CommandPool&)    = delete;

        inline shared_ptr<Device>&          getDevice()             noexcept;
        inline const shared_ptr<Device>&    getDevice()             const noexcept;
        inline uint32_t                     getFamilyIndex()        const noexcept;
        inline const VkCommandPool&         getCommandPoolHandle()  const noexcept;

        inline static shared_ptr<CommandPool> make(const shared_ptr<Device>& ptr_device, uint32_t queue_family_index);

    private:
        shared_ptr<Device>  _ptr_device;
        VkCommandPool       _command_pool_handle;
        uint32_t            _queue_family_index;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inline CommandPool::CommandPool(CommandPool&& command_pool) :
        _ptr_device         (command_pool._ptr_device),
        _command_pool_handle(VK_NULL_HANDLE),
        _queue_family_index (command_pool._queue_family_index)
    {
        swap(_command_pool_handle, command_pool._command_pool_handle);
    }

    inline CommandPool::~CommandPool()
    {
        if (_command_pool_handle != VK_NULL_HANDLE) {
            vkDestroyCommandPool(_ptr_device->getDeviceHandle(), _command_pool_handle, nullptr);
        }
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

    inline const VkCommandPool& CommandPool::getCommandPoolHandle() const noexcept
    {
        return _command_pool_handle;
    }

    inline shared_ptr<CommandPool> CommandPool::make(
        const shared_ptr<Device>&   ptr_device, 
        uint32_t                    queue_family_index
    )
    {
        return make_shared<CommandPool>(ptr_device, queue_family_index);
    }
}

#endif /* CommandPool_hpp */
