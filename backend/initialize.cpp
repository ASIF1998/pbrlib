#include <backend/initialize.hpp>

#include <pbrlib/event_system.hpp>
#include <backend/events.hpp>

#include <pbrlib/exceptions.hpp>

#include <backend/logger/logger.hpp>

#include <backend/renderer/vulkan/shader_compiler.hpp>

#include <backend/profiling.hpp>

#include <backend/scene/assimp_importer.hpp>

#include <SDL3/SDL.h>

#include <format>

namespace pbrlib::backend
{
    void initialize()
    {
        EventSystem::on([] ([[maybe_unused]] const events::Initialize& event)
        {
            if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) [[unlikely]]
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

        EventSystem::on([] ([[maybe_unused]] const events::Initialize& event)
        {
            EventSystem::on([] (const events::AssimpImporter& event)
            {
                PBRLIB_PROFILING_ZONE_SCOPED;

                backend::log::info("[importer] load model: '{}'", event.filename.string());

                const bool res = backend::AssimpImporter()
                    .device(event.ptr_device)
                    .scene(event.ptr_scene)
                    .filename(event.filename)
                    .materialManager(event.ptr_material_manager)
                    .meshManager(event.ptr_mesh_manager)
                    .transform(event.transform)
                    .import();

                if (!res)
                    throw exception::RuntimeError(std::format("[importer] failed load model: {}", event.filename.string()));
            });
        });
    }
}
