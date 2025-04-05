#include "Font.h"

namespace Mage
{
    struct Font::Impl
    {
        struct Character
        {
            GLuint texture;
            glm::ivec2 size;
            glm::ivec2 bearing;
            unsigned int advance;
        };

        std::map<unsigned char, Character> characters;
        std::string font_file;
        uint32_t line_height;
        GLuint vao = 0;
        GLuint vbo = 0;
    };

    Font::Font(const char *font_file)
    {
        LOG_E_INFO("Loading font %s", font_file);
        _impl = new Impl();
        _impl->font_file = font_file;

        FT_Library ft;
        if (FT_Init_FreeType(&ft))
        {
            throw Exception("Failed to initialize FreeType");
        }
        FT_Face face;
        if (FT_New_Face(ft, font_file, 0, &face))
        {
            throw Exception((std::string("Failed to load font") + font_file).c_str());
        }

        FT_Set_Pixel_Sizes(face, 0, 48);
        if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
        {
            throw Exception("Failed loading X glyph from font(needed to determine line height");
        }

        _impl->line_height = face->glyph->bitmap.rows;

        //these images are single color channel, so we need one byte alignment
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        for (unsigned char c = 0; c < 128; c++)
        {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                LOG_WARN("Failed loading glyph %c", c);
                continue;
            }

            GLuint texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            //GL_RED because its single channel
            glTexImage2D(texture, 0, GL_RED, static_cast<GLsizei>(face->glyph->bitmap.width),
                         static_cast<GLsizei>(face->glyph->bitmap.rows), 0, GL_RED,
                         GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            _impl->characters.insert(std::pair(c, Impl::Character{
                                                   texture,
                                                   glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                                                   glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                                                   static_cast<unsigned int>(face->glyph->advance.x)
                                               }));
        }
        glBindTexture(GL_TEXTURE_2D, 0);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        FT_Done_Face(face);
        FT_Done_FreeType(ft);

        auto first_pos_byte = reinterpret_cast<void *>(0);
        auto first_uv_byte = reinterpret_cast<void *>(sizeof(GLfloat) * 2);

        glGenVertexArrays(1, &_impl->vao);
        glGenBuffers(1, &_impl->vbo);
        glBindVertexArray(_impl->vao);
        glBindBuffer(GL_ARRAY_BUFFER, _impl->vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
                              4 * sizeof(GLfloat), first_pos_byte);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
                              4 * sizeof(GLfloat), first_uv_byte);
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    Font::~Font()
    {
        LOG_E_INFO("Unloading font %s", _impl->font_file.c_str());
        for (auto &p: _impl->characters)
        {
            glDeleteTextures(1, &p.second.texture);
        }
        glDeleteBuffers(1, &_impl->vbo);
        glDeleteVertexArrays(1, &_impl->vao);
        delete _impl;
    }

    uint32_t Font::get_line_height() const
    {
        return _impl->line_height;
    }

    uint32_t Font::get_glyph_size_x(unsigned char glyph) const
    {
        return _impl->characters[glyph].size.x;
    }

    uint32_t Font::get_glyph_size_y(unsigned char glyph) const
    {
        return _impl->characters[glyph].size.y;
    }

    uint32_t Font::get_glyph_bearing_x(unsigned char glyph) const
    {
        return _impl->characters[glyph].bearing.x;
    }

    uint32_t Font::get_glyph_bearing_y(unsigned char glyph) const
    {
        return _impl->characters[glyph].bearing.y;
    }

    uint32_t Font::get_glyph_advance(unsigned char glyph) const
    {
        return _impl->characters[glyph].advance;
    }

    float Font::measure_text(const char *text, float scale) const
    {
        auto x = 0.0f;
        for (size_t i = 0; i < std::strlen(text); i++)
        {
            //advance to next glyph (note that advance is number of 1/64 pixels)
            //bit shift by 6 to get value in pixels (2^6 = 64)
            x += static_cast<float>(get_glyph_advance(text[i]) >> 6) * scale;
        }
        return x;
    }

    void Font::bind()
    {
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(_impl->vao);
    }

    void Font::unbind()
    {
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Font::draw_glyph(unsigned char glyph, float x, float y, float scale)
    {
        auto &c = _impl->characters[glyph];
        auto width = static_cast<float>(c.size.x) * scale;
        auto height = static_cast<float>(c.size.y) * scale;

        float vertices[6][4] =
        {
            //position          uv
            {x, y + height, 0.0f, 0.0f},
            {x, y, 0.0f, 1.0f},
            {x + width, y, 1.0f, 1.0f},
            {x, y + height, 0.0f, 0.0f},
            {x + width, y, 1.0f, 1.0f},
            {x + width, y + height, 1.0f, 0.0f}
        };
        glBindTexture(GL_TEXTURE_2D, c.texture);
        glBindBuffer(GL_ARRAY_BUFFER, _impl->vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}
