#include "Application.h"
#include "../Renderer/Camera.h"
#include "../Renderer/TextRenderer.h"
#include "../Renderer/SpriteRenderer.h"
#include "../Renderer/ShapeRenderer.h"
#include "../Scene/EntityManager.h"
#include "../Scene/ComponentManager.h"
#include "../Scene/SystemManager.h"
#include "../Scene/SystemList.h"

namespace Mage
{
    struct ElapsedTime
    {
        std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
        std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
        float elapsed = 0.0f;

        void update()
        {
            end = std::chrono::system_clock::now();
            elapsed = static_cast<std::chrono::duration<float>>(end - start).count();
            start = end;
        }
    };

    struct Application::Impl final
    {
        std::unique_ptr<Window> window;
        std::unique_ptr<EntityManager> entity_manager;
        std::unique_ptr<ComponentManager> component_manager;
        std::unique_ptr<SystemManager> system_manager;
        std::unique_ptr<EventManager> event_manager;
        std::unique_ptr<TextRenderer> text_renderer;
        std::unique_ptr<SpriteRenderer> sprite_renderer;
        std::unique_ptr<ShapeRenderer> shape_renderer;
        bool closing = false;
    };

    Application::Application()
    {
        LOG_E_INFO("Application Starting");
        _impl = new Impl();
    }

    Application::~Application()
    {
        LOG_E_INFO("Application Stopping");
        delete _impl;
    }

    Window *Application::get_window() const
    {
        return _impl->window.get();
    }

    EntityManager *Application::get_entity_manager() const
    {
        return _impl->entity_manager.get();
    }

    SystemManager *Application::get_system_manager() const
    {
        return _impl->system_manager.get();
    }

    ComponentManager *Application::get_component_manager() const
    {
        return _impl->component_manager.get();
    }

    EventManager *Application::get_event_manager() const
    {
        return _impl->event_manager.get();
    }

    TextRenderer *Application::get_text_renderer() const
    {
        return _impl->text_renderer.get();
    }

    SpriteRenderer *Application::get_sprite_renderer() const
    {
        return _impl->sprite_renderer.get();
    }

    ShapeRenderer *Application::get_shape_renderer() const
    {
        return _impl->shape_renderer.get();
    }

    void Application::close()
    {
        LOG_E_INFO("Application::closed called");
        _impl->closing = true;
    }

    void Application::run()
    {
        LOG_E_INFO("Application running");
        ElapsedTime et;

        while (!_impl->closing)
        {
            // update entity manager
            // poll events
            // clear window

            //for each system
            //  call update on system (using deltaTime)

            // present the window
        }
    }

}
