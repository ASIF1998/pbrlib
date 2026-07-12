#pragma once

#include <span>

namespace pbrlib::backend::vk
{
    template<typename T>
    concept SpanCastable = requires(T container)
    {
        std::span(container);
    };

    template<typename T>
    concept NotSequenceContainer = !SpanCastable<T>;
}
