#include "TextRenderer.h"

namespace Mage
{
    struct TextRenderer::Impl
    {
        const char *vs = ""; //TODO
        const char *fs = ""; //TODO
        Shader text_shader = Shader("TextShader", vs, fs);
        Window *window = nullptr;
        GLint window_size_uniform_loc = 0;
        GLint text_color_uniform_loc = 0;
    };

    TextRenderer::TextRenderer(Window &window)
    {
        _impl = new Impl();
        _impl->window = &window;
        _impl->window_size_uniform_loc = glGetUniformLocation(_impl->text_shader.get_shader_id(),
            "window_size");
        _impl->text_color_uniform_loc = glGetUniformLocation(_impl->text_shader.get_shader_id(),
            "text_color");
    }

    TextRenderer::~TextRenderer()
    {
        LOG_E_INFO("Destroying TextRenderer");
        delete _impl;
    }

    void TextRenderer::render_text(Font &font, const char *text, float x, float y, float scale, const Color &color)
    {
        auto ww = static_cast<float>(_impl->window->get_width());
        auto wh = static_cast<float>(_impl->window->get_height());

        _impl->text_shader.use();
        glUniform2f(_impl->window_size_uniform_loc, ww, wh);
        glUniform4f(_impl->text_color_uniform_loc,
                    color.r_float(), color.g_float(), color.b_float(), color.a_float());
        font.bind();

        for (size_t i = 0; i < std::strlen(text); i++)
        {
            auto size_y = font.get_glyph_size_y(text[i]);
            auto bearing_x = font.get_glyph_bearing_x(text[i]);
            auto bearing_y = font.get_glyph_bearing_y(text[i]);

            float x_pos = x + static_cast<float>(bearing_x) * scale;
            float y_pos = (wh - y) - static_cast<float>(size_y - bearing_y) * scale;
            font.draw_glyph(text[i], x_pos, y_pos, scale);

            //now advance "cursor" for the next glyph, noting that advance is number of 1/64 pixels
            //bit shift by 6 to get value in pixels (2^6) = 64
            x += static_cast<float>(font.get_glyph_advance(text[i]) >> 6) * scale;
        }
    }
}
