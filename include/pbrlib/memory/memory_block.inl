#include "memory_utils.hpp"

namespace pbrlib
{
    inline MemoryBlock::MemoryBlock(size_t size) :
        _ptr_memory (allocAligned(size)),
        _size       (size),
        _num_users  (0),
        _owns       (true)
    {}

    inline MemoryBlock::~MemoryBlock()
    {
        if (_owns)
            freeAligned(_ptr_memory);
    }

    inline void* MemoryBlock::getPtrMemory()
    {
        return _ptr_memory;
    }

    inline const void* MemoryBlock::getPtrMemory() const
    {
        return _ptr_memory;
    }

    inline size_t MemoryBlock::getSize() const
    {
        return _size;
    }

    inline uint32_t MemoryBlock::getNumUsers() const
    {
        return _num_users;
    }

    inline void MemoryBlock::setMemory(void* ptr_memory, size_t size, bool owns)
    {
        if (_owns)
            freeAligned(_ptr_memory);

        _ptr_memory = ptr_memory;
        _size       = size;
        _owns       = owns;
    }

    inline void MemoryBlock::setNumUsers(uint32_t num_users)
    {
        _num_users = num_users;
    }
}
