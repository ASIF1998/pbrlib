#include <backend/initialize.hpp>

#include <pbrlib/event_system.hpp>
#include <backend/events.hpp>

#include <pbrlib/exceptions.hpp>

#include <backend/logger/logger.hpp>

#include <backend/renderer/vulkan/shader_compiler.hpp>

#include <SDL3/SDL.h>

#include <format>

namespace pbrlib::backend
{
    void initialize()
    {
        EventSystem::on([] ([[maybe_unused]] const events::Initialize& event) 
        {
            if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) [[unlikely]]
            {
                std::string error_msg = SDL_GetError();
                SDL_ClearError();

                throw exception::InitializeError(std::format("[window] SDL: {}", error_msg));
            }
        });

        EventSystem::on([] ([[maybe_unused]] const events::Finalize& event) 
        {
            SDL_Quit();
        });

        EventSystem::on([] ([[maybe_unused]] const events::Initialize& event) 
        {
            log::priv::EngineLogger::init();
            log::priv::AppLogger::init();
        });

        EventSystem::on([] ([[maybe_unused]] const events::Initialize& event) 
        {
            vk::shader::initCompiler();
        });

        EventSystem::on([] ([[maybe_unused]] const events::Finalize& event) 
        {
            vk::shader::finalizeCompiler();
        });
    }
}