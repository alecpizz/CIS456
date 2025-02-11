#pragma once

#include "Core.h"

namespace Mage
{

    class Color;

    class MAGE_API Window
    {
        friend class Application;

    public:
        ~Window();

        Window(Window &&) = delete;

        Window(const Window &) = delete;

        Window &operator=(Window &&) = delete;

        Window &operator=(const Window &) = delete;

        uint32_t get_width() const;

        uint32_t get_height() const;

        void set_clear_color(const Color &c);

        void clear_window() const;

        void present() const;

    private:
        Window(const char *title, bool fullscreen, uint32_t width, uint32_t height, uint8_t swap_interval);

        struct Impl;
        Impl *_impl;
    };
}

