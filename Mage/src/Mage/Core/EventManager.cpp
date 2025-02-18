#include "EventManager.h"

namespace Mage
{

    struct EventManager::Impl
    {
        Window *window = nullptr;
        bool motion_event_logging = false;
        bool sensor_event_logging = false;
        bool window_event_logging = true;
        bool keyboard_event_logging = true;
        bool mouse_event_logging = true;
        bool controller_event_logging = true;
        std::vector<OnAppClosingEventListener *> on_app_closing_event_listeners;
        std::vector<OnKeyDownEventListener*> on_key_down_event_listeners;
        std::vector<OnWindowMinimizedEventListener*> on_window_minimized_event_listeners;
        std::vector<OnWindowRestoredEventListener*> on_window_restored_event_listeners;
        std::vector<OnWindowMouseEnteredEventListener*> on_window_mouse_entered_event_listeners;
        std::vector<OnWindowMouseLeftEventListener*> on_window_mouse_left_event_listeners;
        std::vector<OnWindowFocusGainedEventListener*> on_window_focus_gained_event_listeners;
        std::vector<OnWindowFocusLostEventListener*> on_window_focus_lost_event_listeners;
        std::vector<OnWindowResizedEventListener*> on_window_resized_event_listeners;
        std::vector<OnKeyUpEventListener*> on_key_up_event_listeners;
        std::vector<OnMouseMotionEventListener*> on_mouse_motion_event_listeners;
        std::vector<OnMouseButtonUpEventListener*> on_mouse_button_up_event_listeners;
        std::vector<OnMouseButtonDownEventListener*> on_mouse_button_down_event_listeners;
        std::vector<OnMouseWheelEventListener*> on_mouse_wheel_event_listeners;
        std::vector<OnControllerAxisMotionEventListener*> on_controller_axis_motion_event_listeners;
        std::vector<OnControllerButtonDownEventListener*> on_controller_button_down_event_listeners;
        std::vector<OnControllerButtonUpEventListener*> on_controller_button_up_event_listeners;
        std::vector<OnControllerTouchpadDownEventListener*> on_controller_touchpad_down_event_listeners;
        std::vector<OnControllerTouchpadMotionEventListener*> on_controller_touchpad_motion_event_listeners;
        std::vector<OnControllerSensorUpdateEventListener*> on_controller_sensor_update_event_listeners;
        //TODO: all of the other vectors for event listeners

        void on_app_closing()
        {
            LOG_E_INFO("Calling app_closing() on all OnAppClosingEventListeners");
            if(on_app_closing_event_listeners.empty())
            {
                LOG_E_WARN("App is closing and no OnAppClosingEventListeners have been added to the EventManager.");
            }
            for(auto p : on_app_closing_event_listeners)
            {
                p->on_app_closing();
            }
        }

        void on_key_down(const SDL_Event& event)
        {
            auto key = translate_key(event.key.keysym.scancode);
            uint16_t modifiers = event.key.keysym.mod;
            uint8_t repeat = event.key.repeat;
            if(keyboard_event_logging)
            {
                LOG_E_INFO("Key down; key %d; modifiers: %d; repeat %d", key, modifiers, repeat);
            }
            for(auto p : on_key_down_event_listeners)
            {
                p->on_key_down(key, modifiers, repeat);
            }
        }
        //TODO: all of the other event dispatch methods
    };

    EventManager::EventManager(Window &window)
    {
        LOG_E_INFO("Event manager starting");
        _impl = new Impl();
        _impl->window = &window;

    }

    EventManager::~EventManager()
    {
        LOG_E_INFO("Event manager stopping");
        delete _impl;
    }

    void EventManager::poll_events()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                //TODO: handle all SDL to Mage event transitions
                case SDL_QUIT:
                    _impl->on_app_closing();
                    break;
                default:;
            }
        }
    }

    void EventManager::motion_event_logging(bool enabled)
    {
        _impl->motion_event_logging = enabled;
        LOG_E_INFO("Event Manager Motion event logging changed %d", enabled);
    }

    void EventManager::sensor_event_logging(bool enabled)
    {
        _impl->sensor_event_logging = enabled;
        LOG_E_INFO("Event Manager Sensor event logging changed %d", enabled);
    }

    void EventManager::window_event_logging(bool enabled)
    {
        _impl->window_event_logging = enabled;
        LOG_E_INFO("Event Manager Window event logging changed %d", enabled);
    }

    void EventManager::keyboard_event_logging(bool enabled)
    {
        _impl->keyboard_event_logging = enabled;
        LOG_E_INFO("Event Manager Keyboard event logging changed %d", enabled);
    }

    void EventManager::mouse_event_logging(bool enabled)
    {
        _impl->mouse_event_logging = enabled;
        LOG_E_INFO("Event Manager Mouse event logging changed %d", enabled);
    }

    void EventManager::controller_event_logging(bool enabled)
    {
        _impl->controller_event_logging = enabled;
        LOG_E_INFO("Event Manager Controller event logging changed %d", enabled);
    }


    void EventManager::add_on_app_closing_event_listener(OnAppClosingEventListener *listener)
    {
        _impl->on_app_closing_event_listeners.push_back(listener);
        LOG_E_INFO("OnAppClosingEventListener added: %s", typeid(*listener).name());
    }

    void EventManager::add_on_window_minimized_event_listener(OnWindowMinimizedEventListener *listener)
    {
        _impl->on_window_minimized_event_listeners.push_back(listener);
    }

    void EventManager::add_on_window_restored_event_listener(OnWindowRestoredEventListener *listener)
    {
        _impl->on_window_restored_event_listeners.push_back(listener);
    }

    void EventManager::add_on_window_mouse_entered_event_listener(OnWindowMouseEnteredEventListener *listener)
    {
        _impl->on_window_mouse_entered_event_listeners.push_back(listener);
    }

    void EventManager::add_on_window_mouse_left_event_listener(OnWindowMouseLeftEventListener *listener)
    {
        _impl->on_window_mouse_left_event_listeners.push_back(listener);
    }

    void EventManager::add_on_window_focus_gained_event_listener(OnWindowFocusGainedEventListener *listener)
    {
        _impl->on_window_focus_gained_event_listeners.push_back(listener);
    }

    void EventManager::add_on_window_focus_lost_event_listener(OnWindowFocusLostEventListener *listener)
    {
        _impl->on_window_focus_lost_event_listeners.push_back(listener);
    }

    void EventManager::add_on_window_resized_event_listener(OnWindowResizedEventListener *listener)
    {
        _impl->on_window_resized_event_listeners.push_back(listener);
    }

    void EventManager::add_on_key_down_event_listener(OnKeyDownEventListener *listener)
    {
        _impl->on_key_down_event_listeners.push_back(listener);
    }

    void EventManager::add_on_key_up_event_listener(OnKeyUpEventListener *listener)
    {
        _impl->on_key_up_event_listeners.push_back(listener);
    }

    void EventManager::add_on_mouse_motion_event_listener(OnMouseMotionEventListener *listener)
    {
        _impl->on_mouse_motion_event_listeners.push_back(listener);
    }

    void EventManager::add_on_mouse_button_up_event_listener(OnMouseButtonUpEventListener *listener)
    {
        _impl->on_mouse_button_up_event_listeners.push_back(listener);
    }

    void EventManager::add_on_mouse_button_down_event_listener(OnMouseButtonDownEventListener *listener)
    {
        _impl->on_mouse_button_down_event_listeners.push_back(listener);
    }

    void EventManager::add_on_mouse_wheel_event_listener(OnMouseWheelEventListener *listener)
    {
        _impl->on_mouse_wheel_event_listeners.push_back(listener);
    }

    void EventManager::add_on_controller_axis_motion_event_listener(OnControllerAxisMotionEventListener *listener)
    {
        _impl->on_controller_axis_motion_event_listeners.push_back(listener);
    }

    void EventManager::add_on_controller_button_down_event_listener(OnControllerButtonDownEventListener *listener)
    {
        _impl->on_controller_button_down_event_listeners.push_back(listener);
    }

    void EventManager::add_on_controller_button_up_event_listener(OnControllerButtonUpEventListener *listener)
    {
        _impl->on_controller_button_up_event_listeners.push_back(listener);
    }

    void EventManager::add_on_controller_touchpad_down_event_listener(OnControllerTouchpadDownEventListener *listener)
    {
        _impl->on_controller_touchpad_down_event_listeners.push_back(listener);
    }

    void EventManager::add_on_controller_touchpad_motion_event_listener(
        OnControllerTouchpadMotionEventListener *listener)
    {
        _impl->on_controller_touchpad_motion_event_listeners.push_back(listener);
    }

    void EventManager::add_on_controller_sensor_update_event_listener(OnControllerSensorUpdateEventListener *listener)
    {
        _impl->on_controller_sensor_update_event_listeners.push_back(listener);
    }

}
