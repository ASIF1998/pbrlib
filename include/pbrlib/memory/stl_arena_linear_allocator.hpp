#pragma once

#include "memory_arena.hpp"
#include "memory_block.hpp"

namespace pbrlib
{
    template<typename Type>
    class STLArenaLinearAllocator
    {
    public:
        using value_type                = Type;
        using pointer                   = value_type*;
        using const_poiner              = const value_type*;
        using reference                 = value_type&;
        using const_reference           = const value_type&;
        using size_type                 = size_t;
        using difference_type           = ptrdiff_t;

        using MemoryBlockType = MemoryBlock;

    public:
        template<typename OtherType>
        struct rebind
        {
            using other = STLArenaLinearAllocator<OtherType>;
        };

    public:
        inline STLArenaLinearAllocator(
            std::shared_ptr<MemoryArena<MemoryBlockType>>  ptr_memory_arena, 
            size_t                                  size
        );

        template<typename U>
        inline STLArenaLinearAllocator(STLArenaLinearAllocator<U>& linear_allocator);

        inline STLArenaLinearAllocator(STLArenaLinearAllocator&& linear_allocator);
        inline STLArenaLinearAllocator(const STLArenaLinearAllocator& linear_allocator);

        inline ~STLArenaLinearAllocator();

        inline pointer      address(reference x)        const;
        inline const_poiner address(const_reference x)  const;

        inline pointer  allocate(size_type n);
        inline void     deallocate(pointer p, size_type n);

        inline size_type max_size() const;

        inline void destroy(pointer p);

        inline std::shared_ptr<IMemoryBlock>                         getMemoryBlock();
        inline std::shared_ptr<const IMemoryBlock>                   getMemoryBlock()    const;
        inline std::shared_ptr<MemoryArena<MemoryBlockType>>         getMemoryArena();
        inline std::shared_ptr<const MemoryArena<MemoryBlockType>>   getMemoryArena()    const;
        inline pointer                                  getPtrMemory();
        inline const_poiner                             getPtrMemory()      const;

    private:
        std::shared_ptr<IMemoryBlock>                 _ptr_memory_block;
        std::shared_ptr<MemoryArena<MemoryBlockType>> _ptr_memory_arena;

        pointer _ptr_memory;
        size_t  _memory_size;
        size_t  _current_pos;
    };
}

#include "stl_arena_linear_allocator.inl"
