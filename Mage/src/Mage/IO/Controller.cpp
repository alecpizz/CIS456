#include "Controller.h"

namespace Mage
{
    struct Controller::Impl
    {
        SDL_GameController *controller_id;
        SDL_Joystick *joystick_id;
    };

    Controller::Controller(uint32_t controller_idx)
    {
        _impl = new Impl();
        _impl->controller_id = SDL_GameControllerOpen(static_cast<int>(controller_idx));
        _impl->joystick_id = SDL_JoystickOpen(static_cast<int>(controller_idx));
        if (_impl->controller_id == nullptr || _impl->joystick_id == nullptr)
        {
            LOG_E_WARN("Controller constructed with index %d, but a controller couldn't be found.", controller_idx);
        }
        else
        {
            LOG_E_INFO("Controller created with index %d.", controller_idx);
        }
    }

    Controller::~Controller()
    {
        SDL_GameControllerClose(_impl->controller_id);
        SDL_JoystickClose(_impl->joystick_id);
        delete _impl;
        LOG_E_INFO("Controller destroyed.");
    }

    uint8_t Controller::get_controller_count()
    {
        return static_cast<uint8_t>(SDL_NumJoysticks());
    }

    bool Controller::has_rumble() const
    {
        return _impl->controller_id != nullptr &&
            SDL_GameControllerHasRumble(_impl->controller_id);
    }

    bool Controller::has_accelerometer() const
    {
        return _impl->controller_id != nullptr &&
            SDL_GameControllerHasSensor(_impl->controller_id, SDL_SENSOR_ACCEL);
    }

    bool Controller::has_gyroscope() const
    {
        return _impl->controller_id != nullptr &&
                SDL_GameControllerHasSensor(_impl->controller_id, SDL_SENSOR_GYRO);
    }

    uint16_t Controller::get_number_of_axes() const
    {
        if (_impl->joystick_id == nullptr)
        {
            return 0;
        }
        return static_cast<uint16_t>(SDL_JoystickNumAxes(_impl->joystick_id));
    }

    uint16_t Controller::get_number_of_buttons() const
    {
        if (_impl->joystick_id == nullptr)
        {
            return 0;
        }
        return static_cast<uint16_t>(SDL_JoystickNumButtons(_impl->joystick_id));
    }

    void Controller::rumble(uint32_t ms_to_rumble, uint16_t high_freq_intensity, uint16_t low_freq_intensity) const
    {
        if (_impl->controller_id == nullptr)
        {
            return;
        }

        if (!has_rumble())
        {
            return;
        }
        SDL_GameControllerRumble(_impl->controller_id, low_freq_intensity, high_freq_intensity, ms_to_rumble);
    }
}
