namespace pbrlib
{
    template<typename Event>
    void EventSystem::emit(const Event& event)
    {
        _dispatcher.trigger(event);
    }

    template<InvocableWithParams Callback>
    void EventSystem::on(Callback callback)
    {
        using EventType = std::remove_cvref_t <
            entt::nth_argument_t<0, Callback>
        >;

        struct Subscription final :
            public Listener
        {
            explicit Subscription(Callback callback) :
                callback(callback)
            { 
                _dispatcher.sink<EventType>().template connect<&Subscription::handle>(this);
            }

            ~Subscription() override 
            {
                _dispatcher.sink<EventType>().template disconnect<&Subscription::handle>(this);
            }

            void handle(const EventType& event) const
            {
                callback(event);
            }

            Callback callback;
        };

        _listeners.emplace_front(std::make_unique<Subscription>(std::move(callback)));
    }
}
