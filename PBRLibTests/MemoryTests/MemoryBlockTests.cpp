//
//  MemoryBlockTests.cpp
//  PBRLibTests
//
//  Created by Асиф Мамедов on 26/04/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include "../utils.hpp"

#include "../../src/Memory/MemoryBlock.hpp"

using namespace pbrlib;

TEST(MemoryMemoryBlock, Constructor)
{
    constexpr size_t size = 1024;

    PtrIMemoryBlock ptr_memory_block = MemoryBlock::make(size);

    pbrlib::testing::utils::equality(size, ptr_memory_block->getSize());
    pbrlib::testing::utils::equality(0, ptr_memory_block->getNumUsers());
    pbrlib::testing::utils::notEquality(static_cast<size_t>(0), reinterpret_cast<size_t>(ptr_memory_block->getPtrMemory()));
}

TEST(MemoryMemoryBlock, SetMethods)
{
    constexpr size_t    size1       = 1024;
    constexpr size_t    size2       = 512;
    constexpr uint32_t  num_users   = 123;

    PtrIMemoryBlock ptr_memory_block    = MemoryBlock::make(size1);    
    void*           new_memory          = allocAligned(size2);

    ptr_memory_block->setMemory(new_memory, size2, true);
    ptr_memory_block->setNumUsers(num_users);

    pbrlib::testing::utils::equality(size2, ptr_memory_block->getSize());
    pbrlib::testing::utils::notEquality(static_cast<size_t>(0), reinterpret_cast<size_t>(ptr_memory_block->getPtrMemory()));
    pbrlib::testing::utils::equality(num_users, ptr_memory_block->getNumUsers());
}