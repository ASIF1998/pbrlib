#pragma once

#include <entt/entt.hpp>

namespace pbrlib
{
    template<typename Callback>
    concept InvocableWithParams = requires
    {
        typename entt::nth_argument_t<0, Callback>;
    };

    class Listener
    {
    public:
        Listener() = default;

        Listener(Listener&& listener)       = default;
        Listener(const Listener& listener)  = delete;
        
        virtual ~Listener() = default;

        Listener& operator = (Listener&& listener)      = default;
        Listener& operator = (const Listener& listener) = delete;
    };

    class EventSystem
    {
    public:
        EventSystem() = default;

        EventSystem(EventSystem&& event_system)         = delete;
        EventSystem(const EventSystem& event_system)    = delete;

        EventSystem& operator = (EventSystem&& event_system)        = delete;
        EventSystem& operator = (const EventSystem& event_system)   = delete;

        template<typename Event>
        static void emit(const Event& event);

        template<InvocableWithParams Callback>
        void on(Callback callback);

    private:
        std::list<std::unique_ptr<Listener>> _listeners;

        static entt::dispatcher _dispatcher;
    };
}

#include <pbrlib/event_system.inl>
