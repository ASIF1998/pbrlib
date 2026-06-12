#pragma once

#include <functional>

namespace pbrlib::backend::utils
{
    struct ScopeExit final
    {
        explicit ScopeExit(std::function<void()>&& callback) :
            callback(std::move(callback))
        { }

        ScopeExit(const ScopeExit& scope_exit) = delete;

        ~ScopeExit()
        {
            if (callback) [[likely]]
                callback();
        }

        ScopeExit& operator = (const ScopeExit& scope_exit) = delete;

        std::function<void()> callback;
    };
}