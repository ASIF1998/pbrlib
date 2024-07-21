//
//  STLArenaLinearAllocatorTests.cpp
//  PBRLibTests
//
//  Created by Асиф Мамедов on 27/04/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include "../utils.hpp"

#include <pbrlib/Memory/MemoryBlock.hpp>
#include <pbrlib/Memory/MemoryArena.hpp>
#include <pbrlib/Memory/STLArenaLinearAllocator.hpp>

#include <vector>

using namespace pbrlib;
using namespace std;

using TMemoryBlock = STLArenaLinearAllocator<int>::MemoryBlockType;
using TMemoryArena = MemoryArena<TMemoryBlock>;
using PMemoryArena = std::shared_ptr<MemoryArena<TMemoryBlock>>;

TEST(MemorySTLArenaLinearAllocator, Constructor)
{
    constexpr size_t size = 1024;

    PMemoryArena                    ptr_memory_arena = std::make_shared<TMemoryArena>(); 85eaf9a023daf36c2cc18cf1baef815b065826ad
    STLArenaLinearAllocator<int>    linear_allocator1 (ptr_memory_arena, size);
    STLArenaLinearAllocator<int>    linear_allocator2 (ptr_memory_arena, size);

    pbrlib::testing::utils::notEquality(static_cast<size_t>(0), reinterpret_cast<size_t>(linear_allocator1.getPtrMemory()));
    pbrlib::testing::utils::notEquality(static_cast<size_t>(0), reinterpret_cast<size_t>(linear_allocator2.getPtrMemory()));

    pbrlib::testing::utils::equality(size, linear_allocator1.max_size());
    pbrlib::testing::utils::equality(size, linear_allocator2.max_size());

    pbrlib::testing::utils::equality(linear_allocator1.getMemoryArena(), linear_allocator2.getMemoryArena());
    pbrlib::testing::utils::equality(linear_allocator1.getMemoryBlock(), linear_allocator2.getMemoryBlock());

    pbrlib::testing::utils::equality(linear_allocator1.getPtrMemory() + size, linear_allocator2.getPtrMemory());
}

TEST(MemorySTLArenaLinearAllocator, AllocateAndDeallocate)
{
    constexpr size_t size = 1024;

    PMemoryArena                    ptr_memory_arena = std::make_shared<TMemoryArena>();
    STLArenaLinearAllocator<int>    linear_allocator (ptr_memory_arena, size);

    int* ptr = linear_allocator.allocate(size);

    for (size_t i{0}; i < size; i++) {
        pbrlib::testing::utils::equality(reinterpret_cast<size_t>(&ptr[i]), reinterpret_cast<size_t>(linear_allocator.getPtrMemory() + i));
    }

    linear_allocator.deallocate(ptr, size);
}

TEST(MemorySTLArenaLinearAllocator, UsingInSTLVector)
{
    using Type              = size_t;
    using TLinearAllocator  = STLArenaLinearAllocator<Type>;

    constexpr size_t size = 1024 * 1024;

    PMemoryArena                    ptr_memory_arena    = std::make_shared<TMemoryArena>();
    vector<Type, TLinearAllocator>  tvec                (TLinearAllocator (ptr_memory_arena, size));

    tvec.reserve(size);

    for (Type i{0}; i < size; i++) {
        tvec.push_back(i);
    }

    pbrlib::testing::utils::equality(size, tvec.size());

    for (Type i{0}; i < tvec.size(); i++) {
        pbrlib::testing::utils::equality(i, tvec[i]);
    }
}
