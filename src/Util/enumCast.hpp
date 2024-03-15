//
//  enumCast.h
//  PBRLib
//
//  Created by Асиф Мамедов on 29/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef enumCast_h
#define enumCast_h

#include <type_traits>

using namespace std;

namespace pbrlib::utils
{
    template<typename T>
    constexpr typename underlying_type<T>::type enumCast(T x)
    {
        return static_cast<typename underlying_type<T>::type>(x);
    }
}

#endif /* enumCast_h */
