#include "Game.h"
#include "Components.h"

Game::Game() : Application("Game", 1024, 768, 0)
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
    get_component_manager()->register_component<GravityComponent>();
    get_component_manager()->register_component<ColorComponent>();
    get_component_manager()->register_component<TorqueComponent>();
    get_component_manager()->register_component<SpriteComponent>();
    get_component_manager()->register_component<PlayerComponent>();
    get_component_manager()->register_component<BoundingBoxComponent>();
    get_component_manager()->register_component<LifetimeComponent>();
    get_component_manager()->register_component<DestructionNotificationComponent>();

    _gravity_system = std::make_unique<GravitySystem>();
    _torque_system = std::make_unique<TorqueSystem>();
    _shape_rendering_system = std::make_unique<ShapeRenderingSystem>(*get_shape_renderer());
    _sprite_rendering_system = std::make_unique<SpriteRenderingSystem>(*get_sprite_renderer());
    _movement_system = std::make_unique<MovementSystem>();
    _player_system = std::make_unique<PlayerSystem>(this);
    _controller = std::make_unique<Mage::Controller>(0);
    _collision_system = std::make_unique<CollisionSystem>(*this);
    _lifetime_system = std::make_unique<LifetimeSystem>();
    _death_by_y_system = std::make_unique<DeathByYSystem>(*this);
    _destruction_notification_system = std::make_unique<DestructionNotificationSystem>();
    LOG_INFO("Controller created for controller index 0; result: ", _controller.get());
    LOG_INFO("Controller has rumble: %d", _controller->has_rumble());

    get_system_manager()->register_system<RigidBody2DComponent, GravityComponent>(*_gravity_system);
    get_system_manager()->register_system<Transform2DComponent, ColorComponent>(*_shape_rendering_system);
    get_system_manager()->register_system<SpriteComponent, Transform2DComponent>(*_sprite_rendering_system);
    get_system_manager()->register_system<RigidBody2DComponent, Transform2DComponent>(*_movement_system);
    get_system_manager()->register_system<RigidBody2DComponent, TorqueComponent>(*_torque_system);
    get_system_manager()->register_system<PlayerComponent,
        SpriteComponent, GravityComponent, Transform2DComponent, RigidBody2DComponent>(*_player_system);
    get_system_manager()->register_system<BoundingBoxComponent, Transform2DComponent>(*_collision_system);
    get_system_manager()->register_system<LifetimeComponent>(*_lifetime_system);
    get_system_manager()->register_system<PlayerComponent, Transform2DComponent, SpriteComponent>(*_death_by_y_system);
    get_system_manager()->register_system<DestructionNotificationComponent>(*_destruction_notification_system);

    _player_system->initialize();

    _rands.add_uniform_real_distribution("rotation", -90.0f, 90.0f);
    _rands.add_uniform_real_distribution("scale_x", 10.0f, 50.0f);
    _rands.add_uniform_real_distribution("scale_y", 1.0f, 10.0f);
    _rands.add_uniform_real_distribution("color", 0.5f, 1.0f);
    _rands.add_uniform_real_distribution("gravity", -50.0f, -5.0f);
    _rands.add_uniform_real_distribution("pos_x", 0.0f,
                                         static_cast<float>(get_window()->get_width()));
    _rands.add_uniform_real_distribution("pos_y", static_cast<float>(get_window()->get_height()),
                                         static_cast<float>(get_window()->get_height()) * 2.0f);
    _rands.add_uniform_real_distribution("torque", -45.0, 45.0f);

    auto e = get_entity_manager()->add_entity(1);
    get_component_manager()->add_component<BoundingBoxComponent>(*e, {
                                                                     .center = glm::vec2(0.5f, 0.5f),
                                                                     .half_size = glm::vec2(0.5f, 0.5f)
                                                                 });
    get_component_manager()->add_component<Transform2DComponent>(*e, {.scale = {2000.0f, 10.0f}});
    get_component_manager()->add_component<ColorComponent>(*e, {
                                                               .color =
                                                               Mage::Color::custom(_rands.get_uniform_real("color"),
                                                                   _rands.get_uniform_real("color"),
                                                                   _rands.get_uniform_real("color"),
                                                                   _rands.get_uniform_real("color"))
                                                           });
    // for (auto i = 0; i < 5000; i++)
    // {
    //     add_random_shape();
    // }
    // Mage::Log::get().set_engine_log_level(Mage::Log::Level::Debug);
}

void Game::on_app_closing()
{
    LOG_INFO("Game closing");
    close();
}

void Game::add_random_shape()
{
    auto e = get_entity_manager()->add_entity(1);
    get_component_manager()->add_component<GravityComponent>(*e, {
                                                                 .force = glm::vec2(0.0f, _rands.get_uniform_real(
                                                                         "gravity"))
                                                             });
    get_component_manager()->add_component<TorqueComponent>(*e, {.torque = _rands.get_uniform_real("torque")});
    get_component_manager()->add_component<Transform2DComponent>(*e,
                                                                 {
                                                                     .translation = glm::vec2(
                                                                         _rands.get_uniform_real("pos_x"),
                                                                         _rands.get_uniform_real("pos_y")),
                                                                     .scale = glm::vec2(
                                                                         _rands.get_uniform_real("scale_x"),
                                                                         _rands.get_uniform_real("scale_y")),
                                                                     .rotation = _rands.get_uniform_real("rotation")
                                                                 });

    get_component_manager()->add_component<ColorComponent>(*e, {
                                                               .color =
                                                               Mage::Color::custom(_rands.get_uniform_real("color"),
                                                                   _rands.get_uniform_real("color"),
                                                                   _rands.get_uniform_real("color"),
                                                                   _rands.get_uniform_real("color"))
                                                           });

    get_component_manager()->add_component<RigidBody2DComponent>(*e, {
                                                                     .velocity = glm::vec2(0.0f, 0.0f),
                                                                     .angular_velocity = 0.0f
                                                                 });
}
