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

        //finish for ComponentManager, SystemManager, EventManager, TextRenderer, SpriteRenderer, ShapeRenderer
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

    //finish for ComponentManager, SystemManager, EventManager, TextRenderer, SpriteRenderer, ShapeRenderer

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
