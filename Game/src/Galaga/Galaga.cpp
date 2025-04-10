//
// Created by alecp on 4/1/2025.
//

#include "Galaga.h"
#include "Components.h"

#include "../Components.h"

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
        get_component_manager()->register_component<EnemyComponent>();
        get_component_manager()->register_component<BoundingBoxComponent>();
        get_component_manager()->register_component<LifetimeComponent>();
        get_component_manager()->register_component<DestructionNotificationComponent>();

        _shape_rendering_system = std::make_unique<ShapeRenderingSystem>(*get_shape_renderer());
        _movement_system = std::make_unique<MovementSystem>();
        _collision_system = std::make_unique<CollisionSystem>(*this);
        _player_system = std::make_unique<PlayerSystem>(this);
        _enemy_controller_system = std::make_unique<EnemyControllerSystem>(this);
        _enemy_spawning_system = std::make_unique<EnemySpawner>(this);
        _lifetime_system = std::make_unique<LifetimeSystem>();

        get_system_manager()->register_system<Transform2DComponent, ColorComponent>(*_shape_rendering_system);
        get_system_manager()->register_system<RigidBody2DComponent, Transform2DComponent>(*_movement_system);
        get_system_manager()->register_system<BoundingBoxComponent, Transform2DComponent>(*_collision_system);
        get_system_manager()->register_system<PlayerComponent,
            SpriteComponent, Transform2DComponent, RigidBody2DComponent>(*_player_system);
        get_system_manager()->register_system<EnemyComponent,
            SpriteComponent, Transform2DComponent, RigidBody2DComponent>(*_enemy_controller_system);
        get_system_manager()->register_system<EnemyComponent,
            SpriteComponent, Transform2DComponent, RigidBody2DComponent>(*_enemy_spawning_system);
        get_system_manager()->register_system<LifetimeComponent>(*_lifetime_system);

        _player_system->initialize();
        _enemy_spawning_system->initialize();
    }

    void Galaga::on_app_closing()
    {
        close();
    }
}
