#pragma once

#include "Core.h"

namespace Mage
{
    class Window;

    class MAGE_API OnAppClosingEventListener : public EventListenerBase
    {
    public:
        virtual void on_app_closing() = 0;
    };

    class MAGE_API OnWindowMinimizedEventListener : public EventListenerBase
    {

    };

    class MAGE_API OnWindowMaximizedEventListener : public EventListenerBase
    {

    };

    class MAGE_API OnWindowRestoredEventListener : public EventListenerBase
    {

    };

    class MAGE_API OnWindowMouseEnteredEventListener : public EventListenerBase
    {

    };

    class MAGE_API OnWindowMouseLeftEventListener : public EventListenerBase
    {

    };

    class MAGE_API OnWindowFocusGainedEventListener : public EventListenerBase
    {

    };

    class MAGE_API OnWindowFocusLostEventListener : public EventListenerBase
    {

    };

    class MAGE_API OnWindowResizedEventListener : public EventListenerBase
    {

    };

    class MAGE_API OnKeyDownEventListener : public EventListenerBase
    {
    public:
        virtual void on_key_down(Key key, uint16_t key_modifiers, uint8_t repeat_count)=0;
    };

    class MAGE_API OnKeyUpEventListener : public EventListenerBase
    {

    };

    class MAGE_API OnMouseMotionEventListener : public EventListenerBase
    {

    };

    class MAGE_API OnMouseButtonDownEventListener : public EventListenerBase
    {

    };

    class MAGE_API OnMouseButtonUpEventListener : public EventListenerBase
    {

    };

    class MAGE_API OnMouseWheelEventListener : public EventListenerBase
    {

    };

    class MAGE_API OnControllerAxisMotionEventListener : public EventListenerBase
    {

    };

    class MAGE_API OnControllerButtonDownEventListener : public EventListenerBase
    {

    };

    class MAGE_API OnControllerButtonUpEventListener : public EventListenerBase
    {

    };

    class MAGE_API OnControllerTouchpadDownEventListener : public EventListenerBase
    {

    };

    class MAGE_API OnControllerTouchpadMotionEventListener : public EventListenerBase
    {

    };

    class MAGE_API OnControllerSensorUpdateEventListener : public EventListenerBase
    {

    };

    class MAGE_API EventManager
    {
        friend class Application;

    public:
        ~EventManager();

        EventManager(EventManager &&) = delete;

        EventManager(const EventManager &) = delete;

        EventManager &operator=(EventManager &&) = delete;

        EventManager &operator=(const EventManager &) = delete;

        void poll_events();

        void motion_event_logging(bool enabled);

        void sensor_event_logging(bool enabled);

        void window_event_logging(bool enabled);

        void keyboard_event_logging(bool enabled);

        void mouse_event_logging(bool enabled);

        void controller_event_logging(bool enabled);

        void add_on_app_closing_event_listener(OnAppClosingEventListener* listener);
        void add_on_window_minimized_event_listener(OnWindowMinimizedEventListener* listener);
        void add_on_window_restored_event_listener(OnWindowRestoredEventListener* listener);
        void add_on_window_mouse_entered_event_listener(OnWindowMouseEnteredEventListener* listener);
        void add_on_window_mouse_left_event_listener(OnWindowMouseLeftEventListener* listener);
        void add_on_window_focus_gained_event_listener(OnWindowFocusGainedEventListener* listener);
        void add_on_window_focus_lost_event_listener(OnWindowFocusLostEventListener* listener);
        void add_on_window_resized_event_listener(OnWindowResizedEventListener* listener);
        void add_on_key_down_event_listener(OnKeyDownEventListener* listener);
        void add_on_key_up_event_listener(OnKeyUpEventListener* listener);
        void add_on_mouse_motion_event_listener(OnMouseMotionEventListener* listener);
        void add_on_mouse_button_up_event_listener(OnMouseButtonUpEventListener* listener);
        void add_on_mouse_button_down_event_listener(OnMouseButtonDownEventListener* listener);
        void add_on_mouse_wheel_event_listener(OnMouseWheelEventListener* listener);
        void add_on_controller_axis_motion_event_listener(OnControllerAxisMotionEventListener* listener);
        void add_on_controller_button_down_event_listener(OnControllerButtonDownEventListener* listener);
        void add_on_controller_button_up_event_listener(OnControllerButtonUpEventListener* listener);
        void add_on_controller_touchpad_down_event_listener(OnControllerTouchpadDownEventListener* listener);
        void add_on_controller_touchpad_motion_event_listener(OnControllerTouchpadMotionEventListener* listener);
        void add_on_controller_sensor_update_event_listener(OnControllerSensorUpdateEventListener* listener);

    private:
        explicit EventManager(Window &window);

        struct Impl;
        Impl *_impl;
    };
}
