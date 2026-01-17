#pragma once

#include <entt/entt.hpp>

namespace pbrlib
{
    template<typename Callback>
    concept InvocableWithParams = requires
    {
        typename entt::nth_argument_t<0, Callback>;
    };

    class EventSystem final
    {
    public:
        EventSystem() = delete;

        EventSystem(EventSystem&& event_system)         = delete;
        EventSystem(const EventSystem& event_system)    = delete;

        EventSystem& operator = (EventSystem&& event_system)        = delete;
        EventSystem& operator = (const EventSystem& event_system)   = delete;

        template<typename Event>
        static void emmit(const Event& event);

        template<InvocableWithParams Callback>
        static void on(Callback callback);

    private:
        static entt::dispatcher _dispatcher;
    };
}

#include <pbrlib/event_system.inl>
