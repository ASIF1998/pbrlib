//
//  MemoryArenaTests.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 27/04/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <gtest/gtest.h>

#include "../../src/Memory/MemoryBlock.hpp"
#include "../../src/Memory/MemoryArena.hpp"

#include <type_traits>
#include <cstdint>

using namespace testing;
using namespace pbrlib;
using namespace std;

TEST(MemoryMemoryArena, Constructor)
{
    constexpr size_t num_bytes1 = 262144;
    constexpr size_t num_bytes2 = 1024;
    constexpr size_t num_blocks = 10;

    PtrMemoryArena<MemoryBlock> ptr_memory_arena1 = MemoryArena<MemoryBlock>::make();
    PtrMemoryArena<MemoryBlock> ptr_memory_arena2 = MemoryArena<MemoryBlock>::make(num_blocks, num_bytes2);


    EXPECT_EQ(0, ptr_memory_arena1->getNumFreeBlocks());
    EXPECT_EQ(0, ptr_memory_arena1->getNumAvailableBlocks());
    EXPECT_EQ(num_bytes1, ptr_memory_arena1->getNumBytes());

    EXPECT_EQ(num_blocks - 1, ptr_memory_arena2->getNumFreeBlocks());
    EXPECT_EQ(0, ptr_memory_arena2->getNumAvailableBlocks());
    EXPECT_EQ(num_bytes2 * num_blocks, ptr_memory_arena2->getNumBytes());
}

TEST(MemoryMemoryArena, AllocateAndDeallocate)
{
    constexpr size_t size = 1024;

    MemoryArena<MemoryBlock> memory_arena;

    {
        auto [ptr, ptr_memory_block] = memory_arena.allocate(size);

        EXPECT_GE(ptr_memory_block->getSize(), size);
        EXPECT_NE(nullptr, ptr);
        EXPECT_EQ(ptr, ptr_memory_block->getPtrMemory());
        EXPECT_EQ(1, ptr_memory_block->getNumUsers());

        memory_arena.deallocate(ptr, ptr_memory_block);

        EXPECT_EQ(0, ptr_memory_block->getNumUsers());
    }

    {
        auto [ptr, ptr_memory_block] = memory_arena.allocate<int>(size);
        
        if (!is_same<decltype(ptr), int*>::value) {
            EXPECT_TRUE(false) << "Тип указателя ptr не равняется int*";
        }

        EXPECT_GE(ptr_memory_block->getSize(), size * sizeof(int));
        EXPECT_NE(nullptr, ptr);
        EXPECT_EQ(ptr, ptr_memory_block->getPtrMemory());
        EXPECT_EQ(1, ptr_memory_block->getNumUsers());

        memory_arena.deallocate(ptr, ptr_memory_block);

        EXPECT_EQ(0, ptr_memory_block->getNumUsers());
    }
}
