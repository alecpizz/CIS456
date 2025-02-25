#pragma once

#include "../Core/Core.h"

namespace Mage
{
    class MAGE_API Controller
    {
    public:
        explicit Controller(uint32_t controller_idx);

        ~Controller();

        Controller(Controller &&) = delete;

        Controller(const Controller &) = delete;

        Controller &operator=(Controller &&) = delete;

        Controller &operator=(const Controller &) = delete;

        static uint8_t get_controller_count();

        bool has_rumble() const;
        bool has_accelerometer() const;
        bool has_gyroscope() const;

        uint16_t get_number_of_axes() const;
        uint16_t get_number_of_buttons() const;
        void rumble(uint32_t ms_to_rumble, uint16_t high_freq_intensity,
            uint16_t low_freq_intensity) const;
    private:
        struct Impl;
        Impl *_impl;
    };
}
