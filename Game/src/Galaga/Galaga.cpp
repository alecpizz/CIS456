//
// Created by alecp on 4/1/2025.
//

#include "Galaga.h"
#include "Components.h"

namespace Galaga
{
    Galaga::Galaga() : Application("Galaga", 800, 600, 0)
    {
        Mage::Log::get().set_engine_log_level(Mage::Log::Level::Info);
        get_event_manager()->controller_event_logging(true);
        get_event_manager()->keyboard_event_logging(true);
        get_event_manager()->motion_event_logging(true);
        get_event_manager()->sensor_event_logging(true);
        get_event_manager()->mouse_event_logging(true);
        get_event_manager()->window_event_logging(true);
        get_event_manager()->add_on_app_closing_event_listener(this);

        get_component_manager()->register_component<Transform2DComponent>();
        get_component_manager()->register_component<RigidBody2DComponent>();
        get_component_manager()->register_component<ColorComponent>();
        get_component_manager()->register_component<TorqueComponent>();
        get_component_manager()->register_component<SpriteComponent>();
        get_component_manager()->register_component<PlayerComponent>();
        get_component_manager()->register_component<BoundingBoxComponent>();
        get_component_manager()->register_component<LifetimeComponent>();
        get_component_manager()->register_component<DestructionNotificationComponent>();

        _shape_rendering_system = std::make_unique<ShapeRenderingSystem>(*get_shape_renderer());
        _movement_system = std::make_unique<MovementSystem>();
        _starfighter_system = std::make_unique<StarfighterSystem>(this);
        _lifetime_system = std::make_unique<LifetimeSystem>();

        get_system_manager()->register_system<Transform2DComponent, ColorComponent>(*_shape_rendering_system);
        get_system_manager()->register_system<RigidBody2DComponent, Transform2DComponent>(*_movement_system);
        get_system_manager()->register_system<PlayerComponent,
            SpriteComponent, Transform2DComponent, RigidBody2DComponent>(*_starfighter_system);
        get_system_manager()->register_system<LifetimeComponent>(*_lifetime_system);

        _starfighter_system->initialize();
    }

    void Galaga::on_app_closing()
    {
        close();
    }
}
