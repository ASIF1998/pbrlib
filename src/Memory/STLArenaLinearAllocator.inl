//
//  STLArenaLinearAllocator.inl
//  PBRLib
//
//  Created by Асиф Мамедов on 26/04/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

namespace pbrlib
{
    template<typename Type>
    inline STLArenaLinearAllocator<Type>::STLArenaLinearAllocator(
        const PtrMemoryArena<MemoryBlockType>&  ptr_memory_arena, 
        size_t                                  size
    ) :
        _ptr_memory_arena   (ptr_memory_arena),
        _memory_size        (size),
        _current_pos        (0)
    {
        auto [ptr_memory, ptr_memory_block] = _ptr_memory_arena->allocate<Type>(size);

        _ptr_memory         = ptr_memory;
        _ptr_memory_block   = ptr_memory_block;
    }

    template<typename Type>
    inline STLArenaLinearAllocator<Type>::STLArenaLinearAllocator(
        STLArenaLinearAllocator&& linear_allocator
    ) :
        _ptr_memory_arena   (nullptr),
        _ptr_memory_block   (nullptr),
        _ptr_memory         (linear_allocator._ptr_memory),
        _memory_size        (linear_allocator._memory_size),
        _current_pos        (linear_allocator._current_pos)
    {
        swap(_ptr_memory_arena, linear_allocator._ptr_memory_arena);
        swap(_ptr_memory_block, linear_allocator._ptr_memory_block);
    }

    template<typename Type>
    inline STLArenaLinearAllocator<Type>::STLArenaLinearAllocator(
        const STLArenaLinearAllocator& linear_allocator
    ) :
        _ptr_memory_arena   (linear_allocator._ptr_memory_arena),
        _ptr_memory_block   (nullptr),
        _memory_size        (linear_allocator._memory_size),
        _current_pos        (0)
    {
        auto[ptr_memory, ptr_memory_block] = _ptr_memory_arena->allocate<Type>(_memory_size);

        _ptr_memory         = ptr_memory;
        _ptr_memory_block   = ptr_memory_block;
    }

    template<typename Type>
    inline STLArenaLinearAllocator<Type>::~STLArenaLinearAllocator()
    {
        if (_ptr_memory_arena) {
            _ptr_memory_arena->deallocate(_ptr_memory, _ptr_memory_block);
        }
    }

    template<typename Type>
    inline Type* STLArenaLinearAllocator<Type>::address(Type& x) const
    {
        return &x;
    }

    template<typename Type>
    inline const Type* STLArenaLinearAllocator<Type>::address(const Type& x) const
    {
        return &x;
    }

    template<typename Type>
    inline Type* STLArenaLinearAllocator<Type>::allocate(size_t n)
    {
        if (_current_pos + n <= _memory_size) {
            pointer rptr =  _ptr_memory + _current_pos;
            _current_pos += n;
            return rptr;
        }

        return nullptr;
    }

    template<typename Type>
    inline void STLArenaLinearAllocator<Type>::deallocate(Type* p, size_t n)
    {}

    template<typename Type>
    inline size_t STLArenaLinearAllocator<Type>::max_size() const
    {
        return _memory_size;
    }

    template<typename Type>
    inline void STLArenaLinearAllocator<Type>::destroy(pointer p)
    {
        p->~value_type();
    }

    template<typename Type>
    inline PtrIMemoryBlock& STLArenaLinearAllocator<Type>::getMemoryBlock()
    {
        return _ptr_memory_block;
    }

    template<typename Type>
    inline const PtrIMemoryBlock& STLArenaLinearAllocator<Type>::getMemoryBlock() const
    {
        return _ptr_memory_block;
    }

    template<typename Type>
    inline PtrMemoryArena<MemoryBlock>& STLArenaLinearAllocator<Type>::getMemoryArena()
    {
        return _ptr_memory_arena;
    }

    template<typename Type>
    inline const PtrMemoryArena<MemoryBlock>& STLArenaLinearAllocator<Type>::getMemoryArena() const
    {
        return _ptr_memory_arena;
    }

    template<typename Type>
    inline Type* STLArenaLinearAllocator<Type>::getPtrMemory()
    {
        return _ptr_memory;
    }

    template<typename Type>
    inline const Type* STLArenaLinearAllocator<Type>::getPtrMemory() const
    {
        return _ptr_memory;
    }
}
