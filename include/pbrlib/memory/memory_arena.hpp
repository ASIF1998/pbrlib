//
//  MemoryArena.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 26/04/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef MemoryArena_hpp
#define MemoryArena_hpp

#include "imemory_block.hpp"

#include <map>

namespace pbrlib
{
    template<typename T>
    concept IsMemoryBlock = requires(T memory_block)
    {
        dynamic_cast<IMemoryBlock*>(&memory_block);
    };
}

namespace pbrlib
{
    /**
     * @class MemoryArena.
     * @brief
     *      Данный класс необходим для распределения памяти на основе
     *      арены, который позволяет пользователю быстро распределять 
     *      объекты из большой непрерывной области памяти.
     * 
     * @details
     *      Реализация блока памяти должна наследоваться от 
     *      IMemoryBlock.
     * 
     * @tparam TMemoryBlock Тип блока памяти.
    */
    template<IsMemoryBlock TMemoryBlock>
    class MemoryArena
    {
    public:
        /**
         * @brief   Конструктор.
         * @details По умолчанию выделяется 256 кб.
        */
        inline MemoryArena();

        inline MemoryArena(size_t num_memory_blocks, size_t memory_block_size);

        MemoryArena(MemoryArena&&)      = delete;
        MemoryArena(const MemoryArena&) = delete;

        inline pair<void*, std::shared_ptr<IMemoryBlock>> allocate(size_t size);

        template<typename Type>
        inline pair<Type*, std::shared_ptr<IMemoryBlock>> allocate(size_t n);

        inline void deallocate(void* ptr, std::shared_ptr<IMemoryBlock> ptr_memory_block);

        inline size_t getNumFreeBlocks()        const;
        inline size_t getNumAvailableBlocks()   const;
        inline size_t getNumBytes()             const;

    private:
        multimap<size_t, std::shared_ptr<IMemoryBlock>>   _free_blocks;
        map<void*, std::shared_ptr<IMemoryBlock>>         _available_blocks;

        std::shared_ptr<IMemoryBlock>   _current_memory_block;
        size_t                          _current_pos;
        size_t                          _current_memory_block_size;

        static constexpr size_t DefaultMemoryBlockSize = 262144;    /// 256 кб.
    };
}

#include "memory_arena.inl"

#endif /* MemoryArena_hpp */
