//
//  PBRLibResources.inl
//  PBRLib
//
//  Created by Асиф Мамедов on 16.09.2021.
//

#include <stdexcept>

namespace pbrlib
{
    inline PBRLibResources::PBRLibResources() :
        _vk_resources()
    {
        if (SDL_Init(SDL_INIT_VIDEO))
            throw std::runtime_error(SDL_GetError());
    }

    inline PBRLibResources::~PBRLibResources()
    {
        SDL_Quit();
    }

    inline PtrPBRLibResources PBRLibResources::init()
    {
        return std::shared_ptr<PBRLibResources>(new PBRLibResources);
    }
}