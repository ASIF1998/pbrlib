//
//  MemoryBlock.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 26/04/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef MemoryBlock_hpp
#define MemoryBlock_hpp

#include "IMemoryBlock.hpp"

using namespace std;

namespace pbrlib
{
    /**
     * @class MemoryBlock.
     * @brief 
     *      Данный класс можно  использовать, если есть необходимость 
     *      в быстром распределении памяти (обычно требуется просто приращение
     *      указателя), или если необходима улучшить локальность ссылок и 
     *      уменьшить количество промахов кэша.
     * 
     * @details
     *      Начальный адрес блока памяти, при выделении и освобождении, если
     *      память принадлежит данному блоку памяти, кратен размеру кэш линии.
     *      Размер кэш линии определяет константа препроцессора PBRLIB_L1_CACHE_LINE_SIZE.
     *      Изначально эта константа препроцессора равна 64 байтам.
    */
    class MemoryBlock :
        public IMemoryBlock
    {
    public:
        inline MemoryBlock(size_t size);
        inline ~MemoryBlock();
        
        inline virtual void*       getPtrMemory()          override;
        inline virtual const void* getPtrMemory()  const   override;
        inline virtual size_t      getSize()       const   override;
        inline virtual uint32_t    getNumUsers()   const   override;

        /**
         * @brief Метод, позволяющий установить новую память.
         * @details 
         *      В случае, если аргумент own = true, то указатель 
         *      ptr_memory должен указывать на область памяти, 
         *      выделенную с помощью функции allocAligned(...).
         * 
         * @param ptr_memory    указатель на новую память.
         * @param size          размер памяти.
         * @param owns          если true, то блок памяти владеет памятью, на которую указывает ptr_memory.
        */
        inline virtual void setMemory(void* ptr_memory, size_t size, bool owns) override;
        
        inline virtual void setNumUsers(uint32_t num_users) override;

        inline static PtrIMemoryBlock make(size_t size);

    private:
        void*       _ptr_memory;
        size_t      _size;
        uint32_t    _num_users;
        bool        _owns;
    };
}

#include "MemoryBlock.inl"

#endif /* MemoryBlock_hpp */
