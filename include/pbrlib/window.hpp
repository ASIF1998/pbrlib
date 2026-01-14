#pragma once

#include <string>
#include <string_view>

#include <cstdint>

namespace pbrlib::backend::vk
{
    class Surface;
}

namespace pbrlib
{
    class Window final
    {
        friend class pbrlib::backend::vk::Surface;

        explicit Window(void* ptr_window) noexcept;

    public:
        class Builder;

    public:
        Window(Window&& window);
        Window(const Window& window) = delete;

        ~Window();

        Window& operator = (Window&& window);
        Window& operator = (const Window& window) = delete;

        [[nodiscard]]
        std::string title() const;
        void        title(std::string_view title);

        [[nodiscard]]
        std::pair<int32_t, int32_t> size() const noexcept;

        void messageBox(std::string_view title, std::string_view msg) const;

    private:
        void* _ptr_window = nullptr;
    };

    class Window::Builder final
    {
        void validate();

    public:
        Builder() = default;

        Builder(Builder&& builder)      = delete;
        Builder(const Builder& builder) = delete;

        Builder& operator = (Builder&& builder)         = delete;
        Builder& operator = (const Builder& builder)    = delete;

        Builder& title(std::string_view title);
        Builder& size(uint32_t width, uint32_t height)  noexcept;
        Builder& resizable(bool is_resizable)           noexcept;

        Window build();

    private:
        std::string _title;

        uint32_t _width     = 0;
        uint32_t _height    = 0;

        bool _is_resizable = false;
    };
}
