//
//  PBRLibResources.inl
//  PBRLib
//
//  Created by Асиф Мамедов on 16.09.2021.
//

#include <glslang/Include/glslang_c_interface.h>

#include <stdexcept>

namespace pbrlib
{
    inline PBRLibResources::PBRLibResources() :
        _vk_resources()
    {
        if (SDL_Init(SDL_INIT_VIDEO))
            throw std::runtime_error(SDL_GetError());

        if (!glslang_initialize_process())
            throw std::runtime_error("Failed initialize glslang library");
    }

    inline PBRLibResources::~PBRLibResources()
    {
        glslang_finalize_process();
        SDL_Quit();
    }

    inline std::unique_ptr<PBRLibResources> PBRLibResources::init()
    {
        return std::unique_ptr<PBRLibResources>(new PBRLibResources);
    }
}