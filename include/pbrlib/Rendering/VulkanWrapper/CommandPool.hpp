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

namespace pbrlib
{
    class CommandPool
    {
    public:
        /**
         * @brief Конструктор.
         * 
         * @param ptr_device            указатель на устройство. 
         * @param queue_family_index    индекс семейства очередей.
        */
        CommandPool(const Device* ptr_device, uint32_t queue_family_index);

        CommandPool(CommandPool&& command_pool);
        CommandPool(const CommandPool&) = delete;

        ~CommandPool();

        CommandPool& operator = (CommandPool&&)         = delete;
        CommandPool& operator = (const CommandPool&)    = delete;

        const Device*   getDevice()             const noexcept;
        uint32_t                        getFamilyIndex()        const noexcept;
        const VkCommandPool&            getCommandPoolHandle()  const noexcept;

        static std::unique_ptr<CommandPool> make(
            const Device*   ptr_device, 
            uint32_t        queue_family_index
        );

    private:
        const Device*   _ptr_device;
        VkCommandPool   _command_pool_handle;
        uint32_t        _queue_family_index;
    };
}

#endif /* CommandPool_hpp */
