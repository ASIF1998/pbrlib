#include <backend/utils/scope_exit.hpp>
#include <backend/logger/logger.hpp>

#include <exception>

namespace pbrlib::backend::utils
{
    ScopeExit::~ScopeExit()
    {
        try
        {
            if (callback) [[likely]]
                callback();
        }
        catch(const std::exception& ex)
        {
            backend::log::error("[scope-exit] {}", ex.what());
        }
        catch(...)
        {
            backend::log::error("[scope-exit] unknown error");
        }
    }
}
