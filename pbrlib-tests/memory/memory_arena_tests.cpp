//
//  MemoryArenaTests.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 27/04/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include "../utils.hpp"

#include <pbrlib/memory/memory_block.hpp>
#include <pbrlib/memory/memory_arena.hpp>

#include <type_traits>
#include <cstdint>

using namespace pbrlib;
using namespace std;

TEST(MemoryMemoryArena, Constructor)
{
    constexpr size_t num_bytes1 = 262144;
    constexpr size_t num_bytes2 = 1024;
    constexpr size_t num_blocks = 10;

    MemoryArena<MemoryBlock> memory_arena1;
    MemoryArena<MemoryBlock> memory_arena2 (num_blocks, num_bytes2);

    pbrlib::testing::utils::equality(0, memory_arena1.getNumFreeBlocks());
    pbrlib::testing::utils::equality(0, memory_arena1.getNumAvailableBlocks());
    pbrlib::testing::utils::equality(num_bytes1, memory_arena1.getNumBytes());

    pbrlib::testing::utils::equality(num_blocks - 1, memory_arena2.getNumFreeBlocks());
    pbrlib::testing::utils::equality(0, memory_arena2.getNumAvailableBlocks());
    pbrlib::testing::utils::equality(num_bytes2 * num_blocks, memory_arena2.getNumBytes());
}

TEST(MemoryMemoryArena, AllocateAndDeallocate)
{
    constexpr size_t size = 1024;

    MemoryArena<MemoryBlock> memory_arena;

    {
        auto [ptr, ptr_memory_block] = memory_arena.allocate(size);

        pbrlib::testing::utils::greaterEquality(ptr_memory_block->getSize(), size);
        pbrlib::testing::utils::notEquality(static_cast<size_t>(0), reinterpret_cast<size_t>(ptr));
        pbrlib::testing::utils::equality(ptr, ptr_memory_block->getPtrMemory());
        pbrlib::testing::utils::equality(1, ptr_memory_block->getNumUsers());

        memory_arena.deallocate(ptr, ptr_memory_block);

        pbrlib::testing::utils::equality(0, ptr_memory_block->getNumUsers());
    }

    {
        auto [ptr, ptr_memory_block] = memory_arena.allocate<int>(size);
        
        if (!is_same<decltype(ptr), int*>::value) {
            pbrlib::testing::utils::thisTrue(false, "Тип указателя ptr не равняется int*");
        }

        pbrlib::testing::utils::greaterEquality(ptr_memory_block->getSize(), size * sizeof(int));
        pbrlib::testing::utils::notEquality(static_cast<size_t>(0), reinterpret_cast<size_t>(ptr));
        pbrlib::testing::utils::equality(reinterpret_cast<size_t>(ptr), reinterpret_cast<size_t>(ptr_memory_block->getPtrMemory()));
        pbrlib::testing::utils::equality(1, ptr_memory_block->getNumUsers());

        memory_arena.deallocate(ptr, ptr_memory_block);

        pbrlib::testing::utils::equality(0, ptr_memory_block->getNumUsers());
    }
}
