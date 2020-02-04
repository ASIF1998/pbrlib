//
//  CommandPool.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 04/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef CommandPool_hpp
#define CommandPool_hpp

#include <vulkan/vulkan.h>

#include <memory>

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
        ~CommandPool();

        shared_ptr<Device>& getDevice() noexcept;
        const shared_ptr<Device>& getDevice() const noexcept;
        uint32_t getFamilyIndex() const noexcept;
        VkCommandPool getHandle() const noexcept;

    private:
        shared_ptr<Device> _ptr_device;
        VkCommandPool _command_pool_handle;
        uint32_t _queue_family_index;
    };
}

#endif /* CommandPool_hpp */
