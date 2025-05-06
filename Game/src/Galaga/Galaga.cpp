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
        get_component_manager()->register_component<ScoreComponent>();

        _shape_rendering_system = std::make_unique<ShapeRenderingSystem>(*get_shape_renderer());
        _movement_system = std::make_unique<MovementSystem>();
        _collision_system = std::make_unique<CollisionSystem>(*this);
        _player_system = std::make_unique<PlayerSystem>(this);
        _sprite_rendering_system = std::make_unique<SpriteRenderingSystem>(*get_sprite_renderer());
        _enemy_spawning_system = std::make_unique<EnemySpawner>(this);
        _lifetime_system = std::make_unique<LifetimeSystem>();
        _background_sprite = std::make_unique<Mage::Sprite>("res/sprites/bg.png", 1, 0.0f);
		_score_system = std::make_unique<ScoreSystem>(this);

        get_system_manager()->register_system<SpriteComponent, Transform2DComponent>(*_sprite_rendering_system);
        get_system_manager()->register_system<Transform2DComponent, ColorComponent>(*_shape_rendering_system);
        get_system_manager()->register_system<RigidBody2DComponent, Transform2DComponent>(*_movement_system);
        get_system_manager()->register_system<PlayerComponent,
            SpriteComponent, Transform2DComponent, RigidBody2DComponent>(*_player_system);
        get_system_manager()->register_system<EnemyComponent,
            SpriteComponent, Transform2DComponent, RigidBody2DComponent>(*_enemy_spawning_system);
        get_system_manager()->register_system<LifetimeComponent>(*_lifetime_system);
        get_system_manager()->register_system<BoundingBoxComponent, Transform2DComponent>(*_collision_system);
		get_system_manager()->register_system<ScoreComponent>(*_score_system);


        auto bg_entity = get_entity_manager()->add_entity(EntityType::Background);
        get_component_manager()->add_component<SpriteComponent>(*bg_entity, {_background_sprite.get()});
        auto bg_scale = glm::vec2(static_cast<float>(get_window()->get_width()) /_background_sprite->get_width(),
            static_cast<float>(get_window()->get_height()) /_background_sprite->get_height());
        // auto bg_pos = glm::vec2(get_window()->get_width() * 0.5f,
            // get_window()->get_height() * 0.5f );
        auto bg_pos = glm::vec2(0.0f);
        get_component_manager()->add_component<Transform2DComponent>(*bg_entity, Transform2DComponent(bg_pos, bg_scale));

        _player_system->initialize();
        _enemy_spawning_system->initialize();

        get_audio_manager()->set_mixer_group_volume("music", 0.05f);
        get_audio_manager()->set_mixer_group_volume("effects", 1.0f);
        get_audio_manager()->load_sound("player_shoot", "effects", "res/sounds/whoosh.mp3");
        get_audio_manager()->load_sound("enemy_shoot", "effects", "res/sounds/whoosh.mp3");
        // get_audio_manager()->load_sound("player_death", "effects", "res/sounds/player_shoot.mp3");
        get_audio_manager()->load_sound("enemy_collide", "effects", "res/sounds/wet_slap.mp3");
        get_audio_manager()->load_sound("enemy_death", "effects", "res/sounds/short_yell.mp3");
        get_audio_manager()->load_sound("game_music", "music", "res/sounds/synth.mp3");
        get_audio_manager()->play_sound("game_music");
		_score_system->set_player_entity(_player_system->get_player_entity());

        //The Bottom
        auto e = get_entity_manager()->add_entity(EntityType::Wall);
        get_component_manager()->add_component<BoundingBoxComponent>(*e, {
                                                                         .center = glm::vec2(0.5f, 0.5f),
                                                                         .half_size = glm::vec2(0.5f, 0.5f)
            });
        get_component_manager()->add_component<Transform2DComponent>(*e, { .scale = {2000.0f, 10.0f} });
        get_component_manager()->add_component<ColorComponent>(*e, {
                                                                   .color =
                                                                   Mage::Color::custom(0.1f, 0.7f, 0.1f, 0.7f)
            });


        
        // Fortnite Boxing
        // Top Wall
        auto top_wall = get_entity_manager()->add_entity(EntityType::Wall);
        get_component_manager()->add_component<BoundingBoxComponent>(*top_wall, {
                                                                         .center = glm::vec2(0.5f, 0.5f),
                                                                         .half_size = glm::vec2(0.5f, 0.5f)
            });
        get_component_manager()->add_component<Transform2DComponent>(*top_wall, {
            .translation = glm::vec2(0.0f, static_cast<float>(get_window()->get_height()) - 10.0f  ),
            .scale = {2000.0f, 10.0f}
            });
        get_component_manager()->add_component<ColorComponent>(*top_wall, {
                                                                   .color =
                                                                   Mage::Color::custom(0.1f, 0.7f, 0.1f, 0.7f)
            });



        //Divider between Enemies and Player
        auto divider_wall = get_entity_manager()->add_entity(EntityType::Wall);
        get_component_manager()->add_component<BoundingBoxComponent>(*divider_wall, {
                                                                         .center = glm::vec2(0.5f, 0.5f),
                                                                         .half_size = glm::vec2(0.5f, 0.5f)
            });
        get_component_manager()->add_component<Transform2DComponent>(*divider_wall, {
            .translation = glm::vec2(0.0f, static_cast<float>(get_window()->get_height()) - 10.0f) / 2.0f,
            .scale = {2000.0f, 10.0f}
            });
        get_component_manager()->add_component<ColorComponent>(*divider_wall, {
                                                                   .color =
                                                                   Mage::Color::custom(0.1f, 0.7f, 0.1f, 0.7f)
            });


        //Left Wall
        auto left_wall = get_entity_manager()->add_entity(EntityType::Wall);
        get_component_manager()->add_component<BoundingBoxComponent>(*left_wall, {
                                                                         .center = glm::vec2(0.5f, 0.5f),
                                                                         .half_size = glm::vec2(0.5f, 0.5f)
            });
        get_component_manager()->add_component<Transform2DComponent>(*left_wall, {
            .translation = glm::vec2(0.0f, 0.0f),
            .scale = {10.0f, 2000.0f}
            });
        get_component_manager()->add_component<ColorComponent>(*left_wall, {
                                                                   .color =
                                                                   Mage::Color::custom(0.1f, 0.7f, 0.1f, 0.7f)
            });



        //Right Wall --> WORKS
        auto right_wall = get_entity_manager()->add_entity(EntityType::Wall);
        get_component_manager()->add_component<BoundingBoxComponent>(*right_wall, {
                                                                         .center = glm::vec2(0.5f, 0.5f),
                                                                         .half_size = glm::vec2(0.5f, 0.5f)
            });
        get_component_manager()->add_component<Transform2DComponent>(*right_wall, {
            .translation = glm::vec2(static_cast<float>(get_window()->get_width()) - 10, 0.0f),
            .scale = {10.0f, 2000.0f}
            });
        get_component_manager()->add_component<ColorComponent>(*right_wall, {
                                                                   .color =
                                                                   Mage::Color::custom(0.1f, 0.7f, 0.1f, 0.7f)
            });


    }




    void Galaga::on_app_closing()
    {
        close();
    }
}
