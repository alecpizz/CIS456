#pragma once

#include "Core.h"

namespace Mage
{
    class Window;

    class Camera;

    class EventManager;

    class EntityManager;

    class ComponentManager;

    class SystemManager;

    class TextRenderer;

    class SpriteRenderer;

    class ShapeRenderer;

    class AudioManager;

    class MAGE_API Application
    {
    public:
        Application(const char *title, uint8_t swap_interval);

        Application(const char *title, uint32_t width, uint32_t height, uint8_t swap_interval);

        Application(Application &&) = delete;

        Application(const Application &) = delete;

        Application &operator=(Application &&) = delete;

        Application &operator=(const Application &) = delete;

        virtual ~Application();

        Window *get_window() const;

        EntityManager *get_entity_manager() const;

        SystemManager *get_system_manager() const;

        ComponentManager *get_component_manager() const;

        EventManager *get_event_manager() const;

        TextRenderer *get_text_renderer() const;

        SpriteRenderer *get_sprite_renderer() const;

        ShapeRenderer *get_shape_renderer() const;

        AudioManager *get_audio_manager() const;
        Camera *get_camera() const;

        void close();

        void run();

    private:
        struct Impl;
        Impl *_impl;
    };
}
