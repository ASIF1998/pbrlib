//
//  STLArenaLinearAllocatorTests.cpp
//  PBRLibTests
//
//  Created by Асиф Мамедов on 27/04/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <gtest/gtest.h>

#include "../../src/Memory/MemoryBlock.hpp"
#include "../../src/Memory/MemoryArena.hpp"
#include "../../src/Memory/STLArenaLinearAllocator.hpp" 

#include <vector>

using namespace testing;
using namespace pbrlib;
using namespace std;

using TMemoryBlock = STLArenaLinearAllocator<int>::MemoryBlockType;
using TMemoryArena = MemoryArena<TMemoryBlock>;
using PMemoryArena = PtrMemoryArena<TMemoryBlock>;

TEST(MemorySTLArenaLinearAllocator, Constructor)
{
    constexpr size_t size = 1024;

    PMemoryArena                    ptr_memory_arena = TMemoryArena::make();
    STLArenaLinearAllocator<int>    linear_allocator1 (ptr_memory_arena, size);
    STLArenaLinearAllocator<int>    linear_allocator2 (ptr_memory_arena, size);

    EXPECT_NE(nullptr, linear_allocator1.getPtrMemory());
    EXPECT_NE(nullptr, linear_allocator2.getPtrMemory());

    EXPECT_EQ(size, linear_allocator1.max_size());
    EXPECT_EQ(size, linear_allocator2.max_size());

    EXPECT_EQ(linear_allocator1.getMemoryArena(), linear_allocator2.getMemoryArena());
    EXPECT_EQ(linear_allocator1.getMemoryBlock(), linear_allocator2.getMemoryBlock());

    EXPECT_EQ(linear_allocator1.getPtrMemory() + size, linear_allocator2.getPtrMemory());
}

TEST(MemorySTLArenaLinearAllocator, AllocateAndDeallocate)
{
    constexpr size_t size = 1024;

    PMemoryArena                    ptr_memory_arena = TMemoryArena::make();
    STLArenaLinearAllocator<int>    linear_allocator (ptr_memory_arena, size);

    int* ptr = linear_allocator.allocate(size);

    for (size_t i{0}; i < size; i++) {
        EXPECT_EQ(&ptr[i], linear_allocator.getPtrMemory() + i);
    }

    linear_allocator.deallocate(ptr, size);
}

TEST(MemorySTLArenaLinearAllocator, UsingInSTLVector)
{
    using Type              = size_t;
    using TLinearAllocator  = STLArenaLinearAllocator<Type>;

    constexpr size_t size = 1024 * 1024;

    PMemoryArena                    ptr_memory_arena    = TMemoryArena::make();
    vector<Type, TLinearAllocator>  tvec                (TLinearAllocator (ptr_memory_arena, size));

    tvec.reserve(size);

    for (Type i{0}; i < size; i++) {
        tvec.push_back(i);
    }

    EXPECT_EQ(size, tvec.size());

    for (Type i{0}; i < tvec.size(); i++) {
        EXPECT_EQ(i, tvec[i]);
    }
}
