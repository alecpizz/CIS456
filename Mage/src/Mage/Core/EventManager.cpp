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
        std::vector<OnKeyDownEventListener *> on_key_down_event_listeners;
        std::vector<OnWindowMinimizedEventListener *> on_window_minimized_event_listeners;
        std::vector<OnWindowMaximizedEventListener *> on_window_maximized_event_listeners;
        std::vector<OnWindowRestoredEventListener *> on_window_restored_event_listeners;
        std::vector<OnWindowMouseEnteredEventListener *> on_window_mouse_entered_event_listeners;
        std::vector<OnWindowMouseLeftEventListener *> on_window_mouse_left_event_listeners;
        std::vector<OnWindowFocusGainedEventListener *> on_window_focus_gained_event_listeners;
        std::vector<OnWindowFocusLostEventListener *> on_window_focus_lost_event_listeners;
        std::vector<OnWindowResizedEventListener *> on_window_resized_event_listeners;
        std::vector<OnKeyUpEventListener *> on_key_up_event_listeners;
        std::vector<OnMouseMotionEventListener *> on_mouse_motion_event_listeners;
        std::vector<OnMouseButtonUpEventListener *> on_mouse_button_up_event_listeners;
        std::vector<OnMouseButtonDownEventListener *> on_mouse_button_down_event_listeners;
        std::vector<OnMouseWheelEventListener *> on_mouse_wheel_event_listeners;
        std::vector<OnControllerAxisMotionEventListener *> on_controller_axis_motion_event_listeners;
        std::vector<OnControllerButtonDownEventListener *> on_controller_button_down_event_listeners;
        std::vector<OnControllerButtonUpEventListener *> on_controller_button_up_event_listeners;
        std::vector<OnControllerTouchpadDownEventListener *> on_controller_touchpad_down_event_listeners;
        std::vector<OnControllerTouchpadUpEventListener *> on_controller_touchpad_up_event_listeners;
        std::vector<OnControllerTouchpadMotionEventListener *> on_controller_touchpad_motion_event_listeners;
        std::vector<OnControllerSensorUpdateEventListener *> on_controller_sensor_update_event_listeners;

        void on_app_closing()
        {
            LOG_E_INFO("Calling app_closing() on all OnAppClosingEventListeners");
            if (on_app_closing_event_listeners.empty())
            {
                LOG_E_WARN("App is closing and no OnAppClosingEventListeners have been added to the EventManager.");
            }
            for (auto p: on_app_closing_event_listeners)
            {
                p->on_app_closing();
            }
        }
        void on_window_resized(const SDL_Event &event)
        {
            uint32_t new_width = event.window.data1;
            uint32_t new_height = event.window.data2;

            if (window_event_logging)
            {
                LOG_E_DEBUG("Window resized height %d; width %d;", new_height, new_width);
            }
            for (auto p: on_window_resized_event_listeners)
            {
                p->on_window_resized(new_width, new_height);
            }
        }

        void on_window_minimized()
        {
            if (window_event_logging)
            {
                LOG_E_DEBUG("Window minimized");
            }
            for (auto p: on_window_minimized_event_listeners)
            {
                p->on_window_minimized();
            }
        }

        void on_window_maximized()
        {
            if (window_event_logging)
            {
                LOG_E_DEBUG("Window maximized");
            }
            for (auto p: on_window_maximized_event_listeners)
            {
                p->on_window_maximized();
            }
        }

        void on_window_restored()
        {
            if (window_event_logging)
            {
                LOG_E_DEBUG("Window restored");
            }
            for (auto p: on_window_restored_event_listeners)
            {
                p->on_window_restored();
            }
        }

        void on_window_mouse_entered()
        {
            if (window_event_logging)
            {
                LOG_E_DEBUG("Window mouse entered");
            }

            for (auto p: on_window_mouse_entered_event_listeners)
            {
                p->on_window_mouse_entered();
            }
        }

        void on_window_mouse_left()
        {
            if (window_event_logging)
            {
                LOG_E_DEBUG("Window mouse left");
            }
            for (auto p: on_window_mouse_left_event_listeners)
            {
                p->on_window_mouse_left();
            }
        }

        void on_window_focus_gained()
        {
            if (window_event_logging)
            {
                LOG_E_DEBUG("Window focus gained");
            }
            for (auto p: on_window_focus_gained_event_listeners)
            {
                p->on_window_focus_gained();
            }
        }

        void on_window_focus_lost()
        {
            if (window_event_logging)
            {
                LOG_E_DEBUG("Window focus lost");
            }

            for (auto p: on_window_focus_lost_event_listeners)
            {
                p->on_window_focus_lost();
            }
        }

        void on_key_down(const SDL_Event &event)
        {
            auto key = translate_key(event.key.keysym.scancode);
            uint16_t modifiers = event.key.keysym.mod;
            uint8_t repeat = event.key.repeat;
            if (keyboard_event_logging)
            {
                LOG_E_DEBUG("Key down; key %d; modifiers: %d; repeat %d", key, modifiers, repeat);
            }
            for (auto p: on_key_down_event_listeners)
            {
                p->on_key_down(key, modifiers, repeat);
            }
        }

        void on_key_up(const SDL_Event &event)
        {
            auto key = translate_key(event.key.keysym.scancode);
            uint16_t modifiers = event.key.keysym.mod;
            if (keyboard_event_logging)
            {
                LOG_E_DEBUG("Key up; key %d; modifiers %d", key, modifiers);
            }

            for (auto p: on_key_up_event_listeners)
            {
                p->on_key_up(key, modifiers);
            }
        }

        void on_mouse_motion(const SDL_Event &event)
        {
            uint32_t button_states = 0;
            button_states += (event.motion.state & SDL_BUTTON_LMASK) ? static_cast<uint32_t>(MouseButton::Left) : 0;
            button_states += (event.motion.state & SDL_BUTTON_RMASK) ? static_cast<uint32_t>(MouseButton::Right) : 0;
            button_states += (event.motion.state & SDL_BUTTON_MMASK) ? static_cast<uint32_t>(MouseButton::Middle) : 0;
            button_states += (event.motion.state & SDL_BUTTON_X1MASK) ? static_cast<uint32_t>(MouseButton::Extra1) : 0;
            button_states += (event.motion.state & SDL_BUTTON_X2MASK) ? static_cast<uint32_t>(MouseButton::Extra2) : 0;
            auto mouse_x = translate_mouse_x(static_cast<float>(event.motion.x));
            auto mouse_y = translate_mouse_y(static_cast<float>(event.motion.y));
            auto mouse_x_delta = static_cast<float>(event.motion.xrel);
            auto mouse_y_delta = static_cast<float>(event.motion.yrel);
            if (mouse_event_logging && motion_event_logging)
            {
                LOG_E_DEBUG("Mouse motion: button states %d; x %f; y %f; dX %f; dY %f", button_states,
                            mouse_x, mouse_y, mouse_x_delta, mouse_y_delta);
            }
            for (auto p: on_mouse_motion_event_listeners)
            {
                p->on_mouse_motion(translate_mouse_x(mouse_x), translate_mouse_y(mouse_y),
                                   mouse_x_delta, mouse_y_delta, button_states);
            }
        }

        void on_mouse_button_down(const SDL_Event &event)
        {
            MouseButton button = translate_mouse_button(event.button.button);
            auto mouse_x = translate_mouse_x(static_cast<float>(event.button.x));
            auto mouse_y = translate_mouse_y(static_cast<float>(event.button.y));
            uint8_t click_count = event.button.clicks;
            if (mouse_event_logging)
            {
                LOG_E_DEBUG("Mouse button down; button %d; clicks %d", button, click_count);
            }
            for (auto p: on_mouse_button_down_event_listeners)
            {
                p->on_mouse_button_down(button,  mouse_x, mouse_y, click_count);
            }
        }

        void on_mouse_button_up(const SDL_Event &event)
        {
            MouseButton button = translate_mouse_button(event.button.button);
            auto mouse_x = translate_mouse_x(static_cast<float>(event.button.x));
            auto mouse_y = translate_mouse_y(static_cast<float>(event.button.y));
            uint8_t click_count = event.button.clicks;
            if (mouse_event_logging)
            {
                LOG_E_DEBUG("Mouse button up; button %d; clicks %d", button, click_count);
            }
            for (auto p: on_mouse_button_up_event_listeners)
            {
                p->on_mouse_button_up(button, mouse_x, mouse_y, click_count);
            }
        }

        void on_mouse_wheel(const SDL_Event &event)
        {
            auto x = static_cast<float>(event.wheel.x);
            auto y = static_cast<float>(event.wheel.y);
            auto mouse_x = translate_mouse_x(static_cast<float>(event.wheel.mouseX));
            auto mouse_y = translate_mouse_y(static_cast<float>(event.wheel.mouseY));
            bool direction_flipped = event.wheel.direction != SDL_MOUSEWHEEL_NORMAL;
            if (mouse_event_logging)
            {
                LOG_E_DEBUG("Mouse weheel event; x %d; y %d; mouse_x %d;"
                            " mouse_y %d; direction flipped %b", x, y, mouse_x, mouse_y, direction_flipped);
            }
            for (auto p: on_mouse_wheel_event_listeners)
            {
                p->on_mouse_wheel(x, y, mouse_x, mouse_y, direction_flipped);
            }
        }

        void on_controller_axis_motion(const SDL_Event &event)
        {
            uint32_t controller_id = event.caxis.which;
            uint8_t axis_id = event.caxis.axis;
            float axis_value = translate_controller_axis_value(event.caxis.value);
            if (controller_event_logging)
            {
                LOG_E_DEBUG("Controller axis event; controller_id %d; axis_id %d; axis_value %f",
                            controller_id, axis_id, axis_value);
            }
            for (auto p: on_controller_axis_motion_event_listeners)
            {
                p->on_controller_axis_motion(controller_id, axis_id, axis_value);
            }
        }

        void on_controller_button_down(const SDL_Event &event)
        {
            uint32_t controller_id = event.cbutton.which;
            uint8_t button_id = event.cbutton.button;
            if (controller_event_logging)
            {
                LOG_E_DEBUG("Controller button down; controller id %d; button_id %d", controller_id, button_id);
            }
            for (auto p: on_controller_button_down_event_listeners)
            {
                p->on_controller_button_down(controller_id, button_id);
            }
        }

        void on_controller_button_up(const SDL_Event &event)
        {
            uint32_t controller_id = event.cbutton.which;
            uint8_t button_id = event.cbutton.button;
            if (controller_event_logging)
            {
                LOG_E_DEBUG("Controller button up; controller id %d; button_id %d", controller_id, button_id);
            }
            for (auto p: on_controller_button_up_event_listeners)
            {
                p->on_controller_button_up(controller_id, button_id);
            }
        }

        void on_controller_touchpad_down(const SDL_Event &event)
        {
            uint32_t controller_id = event.ctouchpad.which;
            int32_t touchpad_id = event.ctouchpad.touchpad;
            int32_t finger_id = event.ctouchpad.finger;
            float x = event.ctouchpad.x;
            float y = event.ctouchpad.y;
            float pressure = event.ctouchpad.pressure;;
            if (controller_event_logging)
            {
                LOG_E_DEBUG("Touch pad down; controller id %d; touchpad id %d; "
                            "finger id %d; x %f; y %f; pressure %f;", controller_id,
                            touchpad_id, finger_id, x, y, pressure);
            }
            for (auto p: on_controller_touchpad_down_event_listeners)
            {
                p->on_controller_touchpad_down(controller_id, touchpad_id, finger_id, x, y, pressure);
            }
        }

        void on_controller_touchpad_up(const SDL_Event &event)
        {
            uint32_t controller_id = event.ctouchpad.which;
            int32_t touchpad_id = event.ctouchpad.touchpad;
            int32_t finger_id = event.ctouchpad.finger;
            float x = event.ctouchpad.x;
            float y = event.ctouchpad.y;
            float pressure = event.ctouchpad.pressure;;
            if (controller_event_logging)
            {
                LOG_E_DEBUG("Touch pad up; controller id %d; touchpad id %d; "
                            "finger id %d; x %f; y %f; pressure %f;", controller_id,
                            touchpad_id, finger_id, x, y, pressure);
            }
            for (auto p: on_controller_touchpad_up_event_listeners)
            {
                p->on_controller_touchpad_up(controller_id, touchpad_id, finger_id, x, y, pressure);
            }
        }

        void on_controller_touchpad_motion(const SDL_Event &event)
        {
            uint32_t controller_id = event.ctouchpad.which;
            int32_t touchpad_id = event.ctouchpad.touchpad;
            int32_t finger_id = event.ctouchpad.finger;
            float x = event.ctouchpad.x;
            float y = event.ctouchpad.y;
            float pressure = event.ctouchpad.pressure;;
            if (controller_event_logging)
            {
                LOG_E_DEBUG("Touch pad motion; controller id %d; touchpad id %d; "
                            "finger id %d; x %f; y %f; pressure %f;", controller_id,
                            touchpad_id, finger_id, x, y, pressure);
            }
            for (auto p: on_controller_touchpad_motion_event_listeners)
            {
                p->on_controller_touchpad_motion(controller_id, touchpad_id, finger_id, x, y, pressure);
            }
        }

        void on_controller_sensor_update(const SDL_Event &event)
        {
            uint32_t controller_id = event.csensor.which;
            ControllerSensor sensor_id = translate_controller_sensor(event.csensor.sensor);
            float sensor_value_x = event.csensor.data[0];
            float sensor_value_y = event.csensor.data[1];
            float sensor_value_z = event.csensor.data[2];
            uint64_t timestamp = event.csensor.timestamp;
            if (controller_event_logging)
            {
                LOG_E_DEBUG("Controller sensor update; controller id %d; "
                            "sensor_id %d; sensor x %f; sensor y %f; sensor z %f; "
                            "timestamp %d", controller_id, sensor_id, sensor_value_x,
                            sensor_value_y, sensor_value_z, timestamp);
            }
            for (auto p: on_controller_sensor_update_event_listeners)
            {
                p->on_controller_sensor_update(controller_id, sensor_id, sensor_value_x,
                                               sensor_value_y, sensor_value_z, timestamp);
            }
        }


        static MouseButton translate_mouse_button(uint8_t button)
        {
            switch (button)
            {
                case SDL_BUTTON_LEFT: return MouseButton::Left;
                case SDL_BUTTON_MIDDLE: return MouseButton::Middle;
                case SDL_BUTTON_RIGHT: return MouseButton::Right;
                case SDL_BUTTON_X1: return MouseButton::Extra1;
                case SDL_BUTTON_X2: return MouseButton::Extra2;
                default: return MouseButton::Left;
            }
        }

        float translate_controller_axis_value(int16_t axis_value)
        {
            auto rectified = std::abs(axis_value) < SDL_JOYSTICK_AXIS_MAX / 5 ? 0 : axis_value;
            return (static_cast<float>(rectified) /
                static_cast<float>(SDL_JOYSTICK_AXIS_MAX - SDL_JOYSTICK_AXIS_MIN) * 2.0f) - 1.0f;
        }

        static float translate_mouse_x(float mouse_x)
        {
            return mouse_x;
        }

        static ControllerSensor translate_controller_sensor(Sint32 sensor_id)
        {
            switch (sensor_id)
            {
                case SDL_SENSOR_GYRO: return ControllerSensor::Gyroscope;
                case SDL_SENSOR_ACCEL: return ControllerSensor::Accelerometer;
                case SDL_SENSOR_GYRO_L: return ControllerSensor::LeftGyroscope;
                case SDL_SENSOR_ACCEL_L: return ControllerSensor::LeftAccelerometer;
                case SDL_SENSOR_GYRO_R: return ControllerSensor::RightGyroscope;
                case SDL_SENSOR_ACCEL_R: return ControllerSensor::RightAccelerometer;
                default: return ControllerSensor::Unknown;
            }
        }

        float translate_mouse_y(float mouse_y)
        {
            return static_cast<float>(window->get_height() - mouse_y);
        }

        static Key translate_key(SDL_Scancode scan_code)
        {
            switch (scan_code)
            {
                case SDL_SCANCODE_A: return Key::A;
                case SDL_SCANCODE_B: return Key::B;
                case SDL_SCANCODE_C: return Key::C;
                case SDL_SCANCODE_D: return Key::D;
                case SDL_SCANCODE_E: return Key::E;
                case SDL_SCANCODE_F: return Key::F;
                case SDL_SCANCODE_G: return Key::G;
                case SDL_SCANCODE_H: return Key::H;
                case SDL_SCANCODE_I: return Key::I;
                case SDL_SCANCODE_J: return Key::J;
                case SDL_SCANCODE_K: return Key::K;
                case SDL_SCANCODE_L: return Key::L;
                case SDL_SCANCODE_M: return Key::M;
                case SDL_SCANCODE_N: return Key::N;
                case SDL_SCANCODE_O: return Key::O;
                case SDL_SCANCODE_P: return Key::P;
                case SDL_SCANCODE_Q: return Key::Q;
                case SDL_SCANCODE_R: return Key::R;
                case SDL_SCANCODE_S: return Key::S;
                case SDL_SCANCODE_T: return Key::T;
                case SDL_SCANCODE_U: return Key::U;
                case SDL_SCANCODE_V: return Key::V;
                case SDL_SCANCODE_W: return Key::W;
                case SDL_SCANCODE_X: return Key::X;
                case SDL_SCANCODE_Y: return Key::Y;
                case SDL_SCANCODE_Z: return Key::Z;
                case SDL_SCANCODE_SLASH: return Key::ForwardSlash;
                case SDL_SCANCODE_BACKSLASH: return Key::BackSlash;
                case SDL_SCANCODE_NUMLOCKCLEAR: return Key::NumLock;
                case SDL_SCANCODE_KP_DIVIDE: return Key::KeypadDivide;
                case SDL_SCANCODE_KP_MULTIPLY: return Key::KeypadMultiply;
                case SDL_SCANCODE_KP_MINUS: return Key::KeypadMinus;
                case SDL_SCANCODE_KP_PLUS: return Key::KeypadPlus;
                case SDL_SCANCODE_KP_ENTER: return Key::KeypadEnter;
                case SDL_SCANCODE_KP_1: return Key::Keypad1;
                case SDL_SCANCODE_KP_2: return Key::Keypad2;
                case SDL_SCANCODE_KP_3: return Key::Keypad3;
                case SDL_SCANCODE_KP_4: return Key::Keypad4;
                case SDL_SCANCODE_KP_5: return Key::Keypad5;
                case SDL_SCANCODE_KP_6: return Key::Keypad6;
                case SDL_SCANCODE_KP_7: return Key::Keypad7;
                case SDL_SCANCODE_KP_8: return Key::Keypad8;
                case SDL_SCANCODE_KP_9: return Key::Keypad9;
                case SDL_SCANCODE_KP_0: return Key::Keypad0;
                case SDL_SCANCODE_KP_PERIOD: return Key::KeypadPeriod;
                case SDL_SCANCODE_LCTRL: return Key::LeftCtrl;
                case SDL_SCANCODE_LSHIFT: return Key::LeftShift;
                case SDL_SCANCODE_LALT: return Key::LeftAlt;
                case SDL_SCANCODE_RCTRL: return Key::RightCtrl;
                case SDL_SCANCODE_RSHIFT: return Key::RightShift;
                case SDL_SCANCODE_RALT: return Key::RightAlt;
                case SDL_SCANCODE_GRAVE: return Key::Grave;
                case SDL_SCANCODE_SPACE: return Key::Space;
                case SDL_SCANCODE_CAPSLOCK: return Key::CapsLock;
                case SDL_SCANCODE_F1: return Key::F1;
                case SDL_SCANCODE_F2: return Key::F2;
                case SDL_SCANCODE_F3: return Key::F3;
                case SDL_SCANCODE_F4: return Key::F4;
                case SDL_SCANCODE_F5: return Key::F5;
                case SDL_SCANCODE_F6: return Key::F6;
                case SDL_SCANCODE_F7: return Key::F7;
                case SDL_SCANCODE_F8: return Key::F8;
                case SDL_SCANCODE_F9: return Key::F9;
                case SDL_SCANCODE_F10: return Key::F10;
                case SDL_SCANCODE_F11: return Key::F11;
                case SDL_SCANCODE_F12: return Key::F12;
                case SDL_SCANCODE_PRINTSCREEN: return Key::PrintScreen;
                case SDL_SCANCODE_SCROLLLOCK: return Key::ScrollLock;
                case SDL_SCANCODE_PAUSE: return Key::Pause;
                case SDL_SCANCODE_INSERT: return Key::Insert;
                case SDL_SCANCODE_DELETE: return Key::Delete;
                case SDL_SCANCODE_HOME: return Key::Home;
                case SDL_SCANCODE_PAGEUP: return Key::PageUp;
                case SDL_SCANCODE_PAGEDOWN: return Key::PageDown;
                case SDL_SCANCODE_END: return Key::End;
                case SDL_SCANCODE_MODE: return Key::AltGr;
                case SDL_SCANCODE_0: return Key::Zero;
                case SDL_SCANCODE_1: return Key::One;
                case SDL_SCANCODE_2: return Key::Two;
                case SDL_SCANCODE_3: return Key::Three;
                case SDL_SCANCODE_4: return Key::Four;
                case SDL_SCANCODE_5: return Key::Five;
                case SDL_SCANCODE_6: return Key::Six;
                case SDL_SCANCODE_7: return Key::Seven;
                case SDL_SCANCODE_8: return Key::Eight;
                case SDL_SCANCODE_9: return Key::Nine;
                case SDL_SCANCODE_RETURN: return Key::Return;
                case SDL_SCANCODE_ESCAPE: return Key::Escape;
                case SDL_SCANCODE_BACKSPACE: return Key::Backspace;
                case SDL_SCANCODE_TAB: return Key::Tab;
                case SDL_SCANCODE_MINUS: return Key::Minus;
                case SDL_SCANCODE_EQUALS: return Key::Equals;
                case SDL_SCANCODE_LEFTBRACKET: return Key::LeftBracket;
                case SDL_SCANCODE_RIGHTBRACKET: return Key::RightBracket;
                case SDL_SCANCODE_SEMICOLON: return Key::Semicolon;
                case SDL_SCANCODE_APOSTROPHE: return Key::Apostrophe;
                case SDL_SCANCODE_COMMA: return Key::Comma;
                case SDL_SCANCODE_PERIOD: return Key::Period;
                case SDL_SCANCODE_RIGHT: return Key::Right;
                case SDL_SCANCODE_LEFT: return Key::Left;
                case SDL_SCANCODE_DOWN: return Key::Down;
                case SDL_SCANCODE_UP: return Key::Up;
                case SDL_SCANCODE_LGUI: return Key::LeftGui;
                case SDL_SCANCODE_RGUI: return Key::RightGui;
                default: return Key::UnknownKey;
            }
        }
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
                case SDL_WINDOW_MINIMIZED:
                    _impl->on_window_minimized();
                    break;
                case SDL_WINDOW_MAXIMIZED:
                    _impl->on_window_maximized();
                    break;
                case SDL_WINDOWEVENT_RESTORED:
                    _impl->on_window_restored();
                    break;
                case SDL_WINDOWEVENT_ENTER:
                    _impl->on_window_mouse_entered();
                    break;
                case SDL_WINDOWEVENT_LEAVE:
                    _impl->on_window_mouse_left();
                    break;
                case SDL_WINDOWEVENT_FOCUS_GAINED:
                    _impl->on_window_focus_gained();
                    break;
                case SDL_WINDOWEVENT_FOCUS_LOST:
                    _impl->on_window_focus_lost();
                    break;
                case SDL_WINDOWEVENT_RESIZED:
                    _impl->on_window_resized(event);
                    break;
                case SDL_KEYUP:
                    _impl->on_key_up(event);
                    break;
                case SDL_KEYDOWN:
                    _impl->on_key_down(event);
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    _impl->on_mouse_button_down(event);
                    break;
                case SDL_MOUSEBUTTONUP:
                    _impl->on_mouse_button_up(event);
                    break;
                case SDL_MOUSEWHEEL:
                    _impl->on_mouse_wheel(event);
                    break;
                case SDL_CONTROLLERAXISMOTION:
                    _impl->on_controller_axis_motion(event);
                    break;
                case SDL_CONTROLLERBUTTONDOWN:
                    _impl->on_controller_button_down(event);
                    break;
                case SDL_CONTROLLERBUTTONUP:
                    _impl->on_controller_button_up(event);
                    break;
                case SDL_CONTROLLERTOUCHPADDOWN:
                    _impl->on_controller_touchpad_down(event);
                    break;
                case SDL_CONTROLLERTOUCHPADMOTION:
                    _impl->on_controller_touchpad_motion(event);
                    break;
                case SDL_CONTROLLERTOUCHPADUP:
                    _impl->on_controller_touchpad_up(event);
                    break;
                case SDL_CONTROLLERSENSORUPDATE:
                    _impl->on_controller_sensor_update(event);
                    break;
                case SDL_QUIT:
                    _impl->on_app_closing();
                    break;
                case SDL_MOUSEMOTION:
                    _impl->on_mouse_motion(event);
                default: ;
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

    void EventManager::add_on_window_maximized_event_listener(OnWindowMaximizedEventListener *listener)
    {
        _impl->on_window_maximized_event_listeners.push_back(listener);
        LOG_E_INFO("OnWindowMaximizedEventListener added: %s", typeid(*listener).name());
    }

    void EventManager::add_on_window_minimized_event_listener(OnWindowMinimizedEventListener *listener)
    {
        _impl->on_window_minimized_event_listeners.push_back(listener);
        LOG_E_INFO("OnWindowMinimizedEventListener added: %s", typeid(*listener).name());
    }

    void EventManager::add_on_window_restored_event_listener(OnWindowRestoredEventListener *listener)
    {
        _impl->on_window_restored_event_listeners.push_back(listener);
        LOG_E_INFO("OnWindowRestoredEventListener added: %s", typeid(*listener).name());
    }

    void EventManager::add_on_window_mouse_entered_event_listener(OnWindowMouseEnteredEventListener *listener)
    {
        _impl->on_window_mouse_entered_event_listeners.push_back(listener);
        LOG_E_INFO("OnWindowMouseEnteredEventListener added: %s", typeid(*listener).name());
    }

    void EventManager::add_on_window_mouse_left_event_listener(OnWindowMouseLeftEventListener *listener)
    {
        _impl->on_window_mouse_left_event_listeners.push_back(listener);
        LOG_E_INFO("OnWindowMouseLeftEventListener added: %s", typeid(*listener).name());
    }

    void EventManager::add_on_window_focus_gained_event_listener(OnWindowFocusGainedEventListener *listener)
    {
        _impl->on_window_focus_gained_event_listeners.push_back(listener);
        LOG_E_INFO("OnWindowFocusGainedEventListener added: %s", typeid(*listener).name());
    }

    void EventManager::add_on_window_focus_lost_event_listener(OnWindowFocusLostEventListener *listener)
    {
        _impl->on_window_focus_lost_event_listeners.push_back(listener);
        LOG_E_INFO("OnWindowFocusGainedLostListener added: %s", typeid(*listener).name());

    }

    void EventManager::add_on_window_resized_event_listener(OnWindowResizedEventListener *listener)
    {
        _impl->on_window_resized_event_listeners.push_back(listener);
        LOG_E_INFO("OnWindowResizedEventListener added: %s", typeid(*listener).name());

    }

    void EventManager::add_on_key_down_event_listener(OnKeyDownEventListener *listener)
    {
        _impl->on_key_down_event_listeners.push_back(listener);
        LOG_E_INFO("OnKeyDownEventListener added: %s", typeid(*listener).name());

    }

    void EventManager::add_on_key_up_event_listener(OnKeyUpEventListener *listener)
    {
        _impl->on_key_up_event_listeners.push_back(listener);
        LOG_E_INFO("OnKeyUpEventListener added: %s", typeid(*listener).name());
    }

    void EventManager::add_on_mouse_motion_event_listener(OnMouseMotionEventListener *listener)
    {
        _impl->on_mouse_motion_event_listeners.push_back(listener);
        LOG_E_INFO("OnMouseMotionEventListener added: %s", typeid(*listener).name());
    }

    void EventManager::add_on_mouse_button_up_event_listener(OnMouseButtonUpEventListener *listener)
    {
        _impl->on_mouse_button_up_event_listeners.push_back(listener);
        LOG_E_INFO("OnMouseButtonUpListener added: %s", typeid(*listener).name());
    }

    void EventManager::add_on_mouse_button_down_event_listener(OnMouseButtonDownEventListener *listener)
    {
        _impl->on_mouse_button_down_event_listeners.push_back(listener);
        LOG_E_INFO("OnMouseButtonDownListener added: %s", typeid(*listener).name());
    }

    void EventManager::add_on_mouse_wheel_event_listener(OnMouseWheelEventListener *listener)
    {
        _impl->on_mouse_wheel_event_listeners.push_back(listener);
        LOG_E_INFO("OnMouseButtonWheelListener added: %s", typeid(*listener).name());
    }

    void EventManager::add_on_controller_axis_motion_event_listener(OnControllerAxisMotionEventListener *listener)
    {
        _impl->on_controller_axis_motion_event_listeners.push_back(listener);
        LOG_E_INFO("OnControllerAxisMotionEventListener added: %s", typeid(*listener).name());
    }

    void EventManager::add_on_controller_button_down_event_listener(OnControllerButtonDownEventListener *listener)
    {
        _impl->on_controller_button_down_event_listeners.push_back(listener);
        LOG_E_INFO("OnControllerButtonDownEventListener added: %s", typeid(*listener).name());
    }

    void EventManager::add_on_controller_button_up_event_listener(OnControllerButtonUpEventListener *listener)
    {
        _impl->on_controller_button_up_event_listeners.push_back(listener);
        LOG_E_INFO("OnControllerButtonUpEventListener added: %s", typeid(*listener).name());
    }

    void EventManager::add_on_controller_touchpad_down_event_listener(OnControllerTouchpadDownEventListener *listener)
    {
        _impl->on_controller_touchpad_down_event_listeners.push_back(listener);
        LOG_E_INFO("OnControllerTouchpadDownEventListener added: %s", typeid(*listener).name());
    }

    void EventManager::add_on_controller_touchpad_up_event_listener(OnControllerTouchpadUpEventListener *listener)
    {
        _impl->on_controller_touchpad_up_event_listeners.push_back(listener);
        LOG_E_INFO("OnControllerTouchpadUpEventListener added: %s", typeid(*listener).name());
    }

    void EventManager::add_on_controller_touchpad_motion_event_listener(
        OnControllerTouchpadMotionEventListener *listener)
    {
        _impl->on_controller_touchpad_motion_event_listeners.push_back(listener);
        LOG_E_INFO("OnControllerTouchpadMotionEventListener added: %s", typeid(*listener).name());
    }

    void EventManager::add_on_controller_sensor_update_event_listener(OnControllerSensorUpdateEventListener *listener)
    {
        _impl->on_controller_sensor_update_event_listeners.push_back(listener);
        LOG_E_INFO("OnControllerSensorEventListener added: %s", typeid(*listener).name());
    }
}
