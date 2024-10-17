//
//  STLAlignedAllocator.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 05/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef STLAlignedAllocator_hpp
#define STLAlignedAllocator_hpp

#include "memory_utils.hpp"

namespace pbrlib
{
    template<typename Type>
    class STLAlignedAllocator
    {
    public:
        using value_type        = Type;
        using pointer           = value_type*;
        using const_poiner      = const value_type*;
        using reference         = value_type&;
        using const_reference   = const value_type&;
        using size_type         = size_t;
        using difference_type   = ptrdiff_t;

    public:
        template<typename U>
        struct rebind
        {
            using other = STLAlignedAllocator<U>;
        };

    public:
        inline STLAlignedAllocator()    noexcept = default;
        inline ~STLAlignedAllocator()   noexcept = default;

        inline STLAlignedAllocator(STLAlignedAllocator&&)       = default;
        inline STLAlignedAllocator(const STLAlignedAllocator&)  = default;

        template<typename U>
        STLAlignedAllocator(const STLAlignedAllocator<U>& other) 
        {}

        inline pointer  allocate(size_type n)           noexcept;
        inline void     deallocate(pointer p, size_t)   noexcept;

        template<typename U, typename... Args>
        void construct(U* ptr, Args&&... args)
        {
            new (reinterpret_cast<void*>(ptr)) U { std::forward<Args>(args)... };
        }

        template<typename U>
        void destroy(U* ptr)
        {
            ptr->~U();
        }

        template<typename U>
        inline bool operator == (const STLAlignedAllocator<U>&) const noexcept;

        template<typename U>
        inline bool operator != (const STLAlignedAllocator<U>&) const noexcept;
    };
}

#include "stl_aligned_allocator.inl"

#endif /* STLAlignedAllocator_hpp */
