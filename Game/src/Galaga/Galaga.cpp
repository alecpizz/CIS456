//
// Created by alecp on 4/1/2025.
//

#include "Galaga.h"

namespace Galaga
{
    Galaga::Galaga() : Application("Galaga", 1024, 768, 0)
    {
        Mage::Log::get().set_engine_log_level(Mage::Log::Level::Info);
        get_event_manager()->controller_event_logging(true);
        get_event_manager()->keyboard_event_logging(true);
        get_event_manager()->motion_event_logging(true);
        get_event_manager()->sensor_event_logging(true);
        get_event_manager()->mouse_event_logging(true);
        get_event_manager()->window_event_logging(true);
        get_event_manager()->add_on_app_closing_event_listener(this);
    }

    void Galaga::on_app_closing()
    {
        close();
    }
}
