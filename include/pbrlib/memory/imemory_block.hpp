#pragma once

#include <cstddef>
#include <cstdint>

#include <memory>

namespace pbrlib
{
    struct IMemoryBlock
    {
        inline virtual ~IMemoryBlock()
        {}

        virtual void*       getPtrMemory()          = 0;
        virtual const void* getPtrMemory()  const   = 0;
        virtual size_t      getSize()       const   = 0;
        virtual uint32_t    getNumUsers()   const   = 0;

        virtual void setMemory(void* ptr_memory, size_t size, bool own) = 0;
        virtual void setNumUsers(uint32_t num_users)                    = 0;
    };
}
