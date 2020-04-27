//
//  MemoryArena.inl
//  PBRLib
//
//  Created by Асиф Мамедов on 26/04/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include "MemoryUtils.hpp"

namespace pbrlib
{
    template<typename TMemoryBlock>
    inline MemoryArena<TMemoryBlock>::MemoryArena() :
        _current_memory_block       (TMemoryBlock::make(DefaultMemoryBlockSize)),
        _current_pos                (0),
        _current_memory_block_size  (DefaultMemoryBlockSize)
    {}

    template<typename TMemoryBlock>
    inline MemoryArena<TMemoryBlock>::MemoryArena(
        size_t num_memory_blocks, 
        size_t memory_block_size
    ) :
        _current_memory_block       (nullptr), 
        _current_pos                (0),
        _current_memory_block_size  (getAlignSize(memory_block_size))
    {
        assert(num_memory_blocks && _current_memory_block_size);

        _current_memory_block = TMemoryBlock::make(_current_memory_block_size);

        for (size_t i{0}; i < num_memory_blocks - 1; i++) {
            _free_blocks.insert(make_pair(
                _current_memory_block_size, 
                TMemoryBlock::make(_current_memory_block_size)
            ));
        }
    }

    template<typename TMemoryBlock>
    inline pair<void*, PtrIMemoryBlock> MemoryArena<TMemoryBlock>::allocate(size_t size)
    {
        size = getAlignSize(size);

        if (_current_pos + size > _current_memory_block_size) {
            auto free_it = _free_blocks.lower_bound(size);

            if (_current_memory_block->getNumUsers()) {
                _available_blocks.insert(make_pair(
                    _current_memory_block->getPtrMemory(),
                    _current_memory_block
                ));
            } else {
                _free_blocks.insert(make_pair(
                    _current_memory_block_size,
                    _current_memory_block
                ));
            }

            _current_pos = 0;

            if (free_it != end(_free_blocks)) {
                _current_memory_block       = free_it->second;
                _current_memory_block_size  = _current_memory_block->getSize(); 
                _free_blocks.erase(free_it);
            } else {
                _current_memory_block_size = _current_memory_block_size > size ? 
                                                _current_memory_block_size : size;

                _current_memory_block = TMemoryBlock::make(_current_memory_block_size);
            }
        }

        void* rptr      =   static_cast<uint8_t*>(_current_memory_block->getPtrMemory()) + _current_pos;
        _current_pos    +=  size;

        _current_memory_block->setNumUsers(_current_memory_block->getNumUsers() + 1);

        return make_pair(rptr, _current_memory_block);
    }

    template<typename TMemoryBlock>
    template<typename Type>
    inline pair<Type*, PtrIMemoryBlock> MemoryArena<TMemoryBlock>::allocate(size_t n)
    {
        auto [vptr, ptr_memory_block] = allocate(sizeof(Type) * n);
        return make_pair(static_cast<Type*>(vptr), ptr_memory_block);
    }

    template<typename TMemoryBlock>
    inline void MemoryArena<TMemoryBlock>::deallocate(void* ptr, PtrIMemoryBlock ptr_memory_block)
    {
        ptr_memory_block->setNumUsers(ptr_memory_block->getNumUsers() - 1);

        if (!ptr_memory_block->getNumUsers()) {
            if (_current_memory_block == ptr_memory_block) {
                _current_pos = 0;
            } else {
                auto iter = _available_blocks.lower_bound(ptr_memory_block->getPtrMemory());

                if (iter != end(_available_blocks)) {
                    _available_blocks.erase(iter);
                    _free_blocks.insert(make_pair(
                        ptr_memory_block->getSize(),
                        ptr_memory_block
                    ));
                }
            }
        }
    }

    template<typename TMemoryBlock>
    inline size_t MemoryArena<TMemoryBlock>::getNumFreeBlocks() const
    {
        return _free_blocks.size();
    }

    template<typename TMemoryBlock>
    inline size_t MemoryArena<TMemoryBlock>::getNumAvailableBlocks() const
    {
        return _available_blocks.size();
    }

    template<typename TMemoryBlock>
    inline size_t MemoryArena<TMemoryBlock>::getNumBytes() const
    {
        size_t num_bytes = _current_memory_block_size;

        for (auto iter: _free_blocks) {
            num_bytes += iter.first;
        }

        for (auto iter: _available_blocks) {
            num_bytes += iter.second->getSize();
        }

        return num_bytes;
    }

    template<typename TMemoryBlock>
    inline PtrMemoryArena<TMemoryBlock> MemoryArena<TMemoryBlock>::make()
    {
        return make_shared<MemoryArena<TMemoryBlock>>();
    }

    template<typename TMemoryBlock>
    inline PtrMemoryArena<TMemoryBlock> MemoryArena<TMemoryBlock>::make(
        size_t num_memory_blocks, 
        size_t memory_block_size
    )
    {
        return make_shared<MemoryArena<TMemoryBlock>>(num_memory_blocks, memory_block_size);
    }
}
