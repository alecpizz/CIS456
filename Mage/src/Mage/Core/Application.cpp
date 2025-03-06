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
        std::unique_ptr<Camera> camera;
        bool closing = false;

        void construct(const char *title, bool full_screen = true,
                       uint32_t w = 0, uint32_t h = 0, uint8_t swap_interval = 0)
        {
            if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) < 0)
            {
                throw Exception((std::string("Failed to initialize SDL: ") + SDL_GetError()).c_str());
            }
            window = std::unique_ptr<Window>(new Window(title, full_screen, w, h, swap_interval));

            entity_manager = std::unique_ptr<EntityManager>(new EntityManager());
            system_manager = std::unique_ptr<SystemManager>(new SystemManager());
            event_manager = std::unique_ptr<EventManager>(new EventManager(*window));
            component_manager = std::unique_ptr<ComponentManager>(new ComponentManager());
            //TODO: initialize text renderer, sprite renderer
            camera = std::unique_ptr<Camera>(new Camera());
            shape_renderer = std::unique_ptr<ShapeRenderer>(new ShapeRenderer(*camera));
            sprite_renderer = std::unique_ptr<SpriteRenderer>(new SpriteRenderer(*camera));
            component_manager->set_system_manager(*system_manager);
            entity_manager->set_system_manager(*system_manager);
            system_manager->set_component_manager(*component_manager);
            entity_manager->set_component_manager(*component_manager);
            camera->left = 0.0f;
            camera->right = static_cast<float>(window->get_width());
            camera->top = static_cast<float>(window->get_height());
            camera->bottom = 0.0f;
        }
    };

    Application::Application(const char *title, uint8_t swap_interval)
    {
        LOG_E_INFO("Application Starting (in full screen mode)");
        _impl = new Impl();
        _impl->construct(title, true, 0, 0, swap_interval);
    }

    Application::Application(const char *title, uint32_t width, uint32_t height, uint8_t swap_interval)
    {
        LOG_E_INFO("Application Starting (in windowed mode)");
        _impl = new Impl();
        _impl->construct(title, false, width, height, swap_interval);
    }

    Application::~Application()
    {
        LOG_E_INFO("Application Stopping");
        SDL_Quit();
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
            _impl->entity_manager->update();

            _impl->event_manager->poll_events();
            _impl->window->clear_window();
            for(auto s : _impl->system_manager->get_all_systems())
            {
                s->update(*_impl->component_manager, et.elapsed);
            }
            _impl->window->present();
            et.update();
        }
    }



}
