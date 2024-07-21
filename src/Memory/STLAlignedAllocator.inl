//
//  STLAlignedAllocator.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 05/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

namespace pbrlib
{
    template<typename Type>
    inline Type* STLAlignedAllocator<Type>::allocate(size_t n) noexcept
    {
        size_t size = getAlignSize(sizeof(Type) * n);
        return reinterpret_cast<Type*>(allocAligned(size));
    }

    template<typename Type>
    inline void STLAlignedAllocator<Type>::deallocate(pointer p, size_t) noexcept
    {
        freeAligned(p);
    }

    template<typename Type>
    template<typename U>
    inline bool STLAlignedAllocator<Type>::operator == (const STLAlignedAllocator<U>&) const noexcept
    {
        return true;
    }

    template<typename Type>
    template<typename U>
    inline bool STLAlignedAllocator<Type>::operator != (const STLAlignedAllocator<U>&) const noexcept
    {
        return true;
    }
}
