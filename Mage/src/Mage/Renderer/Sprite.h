#pragma once

#include "../Core/Core.h"

namespace Mage
{
    class MAGE_API Sprite
    {
    public:
        Sprite(const char *sprite_image_file, uint32_t frames, float frame_time);

        Sprite(Sprite &&) = delete;

        Sprite(const Sprite &) = delete;

        Sprite &operator=(Sprite &&) = delete;

        Sprite &operator=(const Sprite &) = delete;

        ~Sprite();

        uint32_t get_width() const;

        uint32_t get_height() const;

        bool get_flip_x() const;

        void set_flip_x(bool flip);

        void start_over();

        void draw_sprite(float elapsed_time) const;

    private:
        struct Impl;
        Impl *_impl;
    };
}
