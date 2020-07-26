//
//  IMemoryBlock.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 26/04/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef IMemoryBlock_hpp
#define IMemoryBlock_hpp

#include <cstddef>
#include <cstdint>

#include <memory>

using namespace std;

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

    using PtrIMemoryBlock = shared_ptr<IMemoryBlock>;
}

#endif /* IMemoryBlock_hpp */
