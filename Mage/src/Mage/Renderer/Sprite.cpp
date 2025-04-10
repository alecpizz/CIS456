#include "Sprite.h"

namespace Mage
{
    struct Sprite::Impl
    {
        bool is_instanced = false;
        std::string sprite_image_file;
        uint32_t frames = 0;
        uint32_t current_frame = 0;
        float frame_time = 0.0f;
        float elapsed_time_frame_switch = 0.0f;
        uint32_t width = 0;
        uint32_t height = 0;
        bool flip_x = false;
        GLuint texture_id = 0;
        GLuint vao = 0;
        GLuint vbo = 0;
    };

    Sprite::Sprite(const Sprite *sprite)
    {
        LOG_E_INFO("Copying sprite instance %s", sprite->_impl->sprite_image_file.c_str());
        _impl = new Impl();
        _impl->sprite_image_file = sprite->_impl->sprite_image_file;
        _impl->frames = sprite->_impl->frames;
        _impl->current_frame = sprite->_impl->current_frame;
        _impl->frame_time = sprite->_impl->frame_time;
        _impl->elapsed_time_frame_switch = sprite->_impl->elapsed_time_frame_switch;
        _impl->width = sprite->_impl->width;
        _impl->height = sprite->_impl->height;
        _impl->flip_x = sprite->_impl->flip_x;
        _impl->texture_id = sprite->_impl->texture_id;
        _impl->vao = sprite->_impl->vao;
        _impl->vbo = sprite->_impl->vbo;
        _impl->is_instanced = true;
    }

    Sprite::Sprite(const char *sprite_image_file, uint32_t frames, float frame_time)
    {
        LOG_E_INFO("Loading sprite '%s': ", sprite_image_file);
        _impl = new Impl();
        _impl->sprite_image_file = sprite_image_file;
        _impl->frames = frames;
        _impl->frame_time = frame_time;
        _impl->is_instanced = false;

        stbi_set_flip_vertically_on_load(true);

        int width, height, channels;
        auto image_data = stbi_load(sprite_image_file, &width, &height,
                                    &channels, STBI_rgb_alpha);
        if (!image_data)
        {
            throw Exception((std::string("Failed to load sprite file: ") + sprite_image_file).c_str());
        }

        _impl->width = static_cast<uint32_t>(width) / frames;
        _impl->height = static_cast<uint32_t>(height);

        glGenTextures(1, &_impl->texture_id);
        glBindTexture(GL_TEXTURE_2D, _impl->texture_id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                     GL_UNSIGNED_BYTE, image_data);
        stbi_image_free(image_data);
        glBindTexture(GL_TEXTURE_2D, 0);

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

    Sprite::~Sprite()
    {
        if (!_impl->is_instanced)
        {
            LOG_E_INFO("Destroyed sprite '%s'", _impl->sprite_image_file.c_str());
            glDeleteTextures(1, &_impl->texture_id);
            glDeleteVertexArrays(1, &_impl->vao);
            glDeleteBuffers(1, &_impl->vbo);
        }
        else
        {
            LOG_E_INFO("Unloading sprite instance '%s'", _impl->sprite_image_file.c_str());
        }
        delete _impl;
    }

    uint32_t Sprite::get_width() const
    {
        return _impl->width;
    }

    uint32_t Sprite::get_height() const
    {
        return _impl->height;
    }

    bool Sprite::get_flip_x() const
    {
        return _impl->flip_x;
    }

    void Sprite::set_flip_x(bool flip)
    {
        _impl->flip_x = flip;
    }

    void Sprite::start_over()
    {
        _impl->flip_x = false;
        _impl->current_frame = 0;
        _impl->elapsed_time_frame_switch = 0.0f;
    }

    void Sprite::draw_sprite(float elapsed_time) const
    {
        _impl->elapsed_time_frame_switch += elapsed_time;
        if (_impl->elapsed_time_frame_switch >= _impl->frame_time)
        {
            _impl->elapsed_time_frame_switch = 0.0f;
            _impl->current_frame = (_impl->current_frame + 1) % _impl->frames;
        }

        float x_start = static_cast<float>(_impl->current_frame) / static_cast<float>(_impl->frames);
        float x_end = static_cast<float>(_impl->current_frame + 1) / static_cast<float>(_impl->frames);

        if (_impl->flip_x)
        {
            std::swap(x_start, x_end);
        }

        float vertices[6][4] = {
            //pos           //uv
            {0.0f, 1.0f, x_start, 1.0f},
            {1.0f, 0.0f, x_end, 0.0f},
            {0.0f, 0.0f, x_start, 0.0f},
            {0.0f, 1.0f, x_start, 1.0f},
            {1.0f, 1.0f, x_end, 1.0f},
            {1.0f, 0.0f, x_end, 0.0f}
        };

        glBindVertexArray(_impl->vao);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, _impl->texture_id);
        glBindBuffer(GL_ARRAY_BUFFER, _impl->vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}
