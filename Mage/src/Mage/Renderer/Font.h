#pragma once

#include "../Core/Core.h"

namespace Mage
{
    class MAGE_API Font
    {
    public:
        explicit Font(const char *font_file);

        Font(Font &&) = delete;

        Font(const Font &) = delete;

        Font &operator=(Font &&) = delete;

        Font &operator=(const Font &) = delete;

        ~Font();
        uint32_t get_line_height() const;
        uint32_t get_glyph_size_x(unsigned char glyph) const;
        uint32_t get_glyph_size_y(unsigned char glyph) const;
        uint32_t get_glyph_bearing_x(unsigned char glyph) const;
        uint32_t get_glyph_bearing_y(unsigned char glyph) const;
        uint32_t get_glyph_advance(unsigned char glyph) const;

        float measure_text(const char* text, float scale) const;
        void bind();
        void draw_glyph(unsigned char glyph, float x, float y, float scale);
        void unbind();
    private:
        struct Impl;
        Impl* _impl;
    };
}
