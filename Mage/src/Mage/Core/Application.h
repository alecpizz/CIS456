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

    class MAGE_API Application
    {
    public:
        Application();
        Application(Application&& ) = delete;
        Application(const Application& ) = delete;
        Application& operator=(Application&&) = delete;
        Application& operator=(const Application&) = delete;
        virtual ~Application();

        Window* get_window() const;
        EntityManager* get_entity_manager() const;
        //finish for ComponentManager, SystemManager, EventManager, TextRenderer, SpriteRenderer, ShapeRenderer

        void close();
        void run();

    private:
        struct Impl;
        Impl* _impl;
    };
}
