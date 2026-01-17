namespace pbrlib
{
    template<typename Event>
    void EventSystem::emmit(const Event& event)
    {
        _dispatcher.trigger(event);
    }

    template<InvocableWithParams Callback>
    void EventSystem::on(Callback callback)
    {
        using EventType = std::remove_cvref_t <
            entt::nth_argument_t<0, Callback>
        >;

        struct Listener final
        {
            explicit Listener(Callback callback) :
                callback(callback)
            { }

            Listener(Listener&& listener)       = default;
            Listener(const Listener& listener)  = delete;

            Listener& operator = (Listener&& listener)      = default;
            Listener& operator = (const Listener& listener) = delete;

            void handle(const EventType& event) const
            {
                callback(event);
            }

            Callback callback;
        };

        static std::list<Listener> listeners;

        listeners.emplace_front(std::move(callback));
        _dispatcher.sink<EventType>().connect<&Listener::handle>(&listeners.front());
    }
}
