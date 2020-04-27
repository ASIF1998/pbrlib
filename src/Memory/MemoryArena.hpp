//
//  MemoryArena.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 26/04/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef MemoryArena_hpp
#define MemoryArena_hpp

#include "IMemoryBlock.hpp"

#include <map>

using namespace std;

namespace pbrlib
{
    template<typename TMemoryBlock>
    class MemoryArena;
    
    template<typename TMemoryBlock>
    using PtrMemoryArena = shared_ptr<MemoryArena<TMemoryBlock>>;

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
     * @tparam Тип блока памяти.
    */
    template<typename TMemoryBlock>
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

        inline pair<void*, PtrIMemoryBlock> allocate(size_t size);

        template<typename Type>
        inline pair<Type*, PtrIMemoryBlock> allocate(size_t n);

        inline void deallocate(void* ptr, PtrIMemoryBlock ptr_memory_block);

        inline size_t getNumFreeBlocks()        const;
        inline size_t getNumAvailableBlocks()   const;
        inline size_t getNumBytes()             const;

        inline static PtrMemoryArena<TMemoryBlock> make();
        
        inline static PtrMemoryArena<TMemoryBlock> make(
            size_t num_memory_blocks, 
            size_t memory_block_size
        );

    private:
        multimap<size_t, PtrIMemoryBlock>   _free_blocks;
        map<void*, PtrIMemoryBlock>         _available_blocks;

        PtrIMemoryBlock _current_memory_block;
        size_t          _current_pos;
        size_t          _current_memory_block_size;

        static constexpr size_t DefaultMemoryBlockSize = 262144;    /// 256 кб.
    };
}

#include "MemoryArena.inl"

#endif /* MemoryArena_hpp */
