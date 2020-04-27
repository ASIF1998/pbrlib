//
//  MemoryBlockTests.cpp
//  PBRLibTests
//
//  Created by Асиф Мамедов on 26/04/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <gtest/gtest.h>

#include "../../src/Memory/MemoryBlock.hpp"

using namespace testing;
using namespace pbrlib;

TEST(MemoryMemoryBlock, Constructor)
{
    constexpr size_t size = 1024;

    PtrIMemoryBlock ptr_memory_block = MemoryBlock::make(size);

    EXPECT_EQ(size, ptr_memory_block->getSize());
    EXPECT_EQ(0, ptr_memory_block->getNumUsers());
    EXPECT_NE(nullptr, ptr_memory_block->getPtrMemory());
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

    EXPECT_EQ(size2, ptr_memory_block->getSize());
    EXPECT_NE(nullptr, ptr_memory_block->getPtrMemory());
    EXPECT_EQ(num_users, ptr_memory_block->getNumUsers());
}