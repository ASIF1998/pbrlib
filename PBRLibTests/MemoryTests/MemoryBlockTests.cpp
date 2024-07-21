//
//  MemoryBlockTests.cpp
//  PBRLibTests
//
//  Created by Асиф Мамедов on 26/04/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include "../utils.hpp"

#include <pbrlib/Memory/MemoryBlock.hpp>

using namespace pbrlib;

TEST(MemoryMemoryBlock, Constructor)
{
    constexpr size_t size = 1024;

    MemoryBlock memory_block(size);

    pbrlib::testing::utils::equality(size, memory_block.getSize());
    pbrlib::testing::utils::equality(0, memory_block.getNumUsers());
    pbrlib::testing::utils::notEquality(static_cast<size_t>(0), reinterpret_cast<size_t>(memory_block.getPtrMemory()));
}

TEST(MemoryMemoryBlock, SetMethods)
{
    constexpr size_t    size1       = 1024;
    constexpr size_t    size2       = 512;
    constexpr uint32_t  num_users   = 123;

    MemoryBlock memory_block(size1);    

    auto ptr_new_memory = allocAligned(size2);

    memory_block.setMemory(ptr_new_memory, size2, true);
    memory_block.setNumUsers(num_users);

    pbrlib::testing::utils::equality(size2, memory_block.getSize());
    pbrlib::testing::utils::notEquality(static_cast<size_t>(0), reinterpret_cast<size_t>(memory_block.getPtrMemory()));
    pbrlib::testing::utils::equality(num_users, memory_block.getNumUsers());
}