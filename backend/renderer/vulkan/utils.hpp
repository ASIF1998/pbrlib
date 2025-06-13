#pragma once

#include <span>
#include <concepts>

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