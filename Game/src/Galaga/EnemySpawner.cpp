//
// Created by alecp on 4/1/2025.
//
#include "EnemySpawner.h"

#include "Galaga.h"

#define SCALE_ENEMY 0.5f
#define OFFSET_ENEMY_CENTER 24.0f
#define BBOX_RIGHT_FACING_CENTER_X_ENEMY 47.0f
#define BBOX_LEFT_FACING_CENTER_X_ENEMY 77.0f
#define BBOX_CENTER_Y_ENEMY 79.0f
#define BBOX_HALF_WIDTH_ENEMY 37.0f
#define BBOX_HALF_HEIGHT_ENEMY 79.0f

#define GPEC(T) _game->get_component_manager()->get_component<T>(*_enemy_entity)

namespace Galaga
{
    EnemySpawner::EnemySpawner(Galaga *galaga) : _game(galaga)
    {
    }

    void EnemySpawner::initialize()
    {
        spawn();
    }

    void EnemySpawner::spawn()
    {
        create_enemy_entity();
        place_enemy_entity();
    }

    void EnemySpawner::create_enemy_entity()
    {
        _enemy_entity = _game->get_entity_manager()->add_entity(Galaga::EntityType::Enemy);
        _game->get_component_manager()->add_component(*_enemy_entity, EnemyComponent{});
        _game->get_component_manager()->add_component(*_enemy_entity, SpriteComponent{});
        _game->get_component_manager()->add_component(*_enemy_entity, RigidBody2DComponent{});
        _game->get_component_manager()->add_component(*_enemy_entity, Transform2DComponent{});
        _game->get_component_manager()->add_component(*_enemy_entity, BoundingBoxComponent{
            .on_collided = [&](Mage::Entity* hero, Mage::Entity* other, const glm::vec2 overlap)
            {
                //collision_detected(other, overlap);
            } });
        _game->get_component_manager()->add_component(*_enemy_entity, ColorComponent{
               .color = Mage::Color::custom(0.7f, 0.2f, 0.2f, 0.7f)
            });
        _game->get_component_manager()->add_component(*_enemy_entity, DestructionNotificationComponent
            {
                .on_destroyed = [&]()
                {
                //TODO: Do Something? Or not
                    
                }
            });
    }

    void EnemySpawner::place_enemy_entity()
    {
        auto s = GPEC(SpriteComponent);
        auto t = GPEC(Transform2DComponent);
        auto r = GPEC(RigidBody2DComponent);
        auto b = GPEC(BoundingBoxComponent);

        //s->sprite = _enemy_sprites["#"].get();
        r->velocity = { 0.0f, 0.0f };
        t->scale = { SCALE_ENEMY, SCALE_ENEMY };
        b->center = { BBOX_RIGHT_FACING_CENTER_X_ENEMY, BBOX_CENTER_Y_ENEMY };
        b->half_size = { BBOX_HALF_WIDTH_ENEMY, BBOX_HALF_HEIGHT_ENEMY };
        //Will need to change where the enemy spawns in by changing the translation 
        t->scale = glm::vec2(20.0f, 20.0f);
        t->prev_translation = t->translation = {
            (_game->get_window()->get_width() - 20.0f * 0.25f) / 2.0f,
            (_game->get_window()->get_height() - 40.0f * 0.25f) /2.0f
        };
    }

    void EnemySpawner::update(Mage::ComponentManager& componentManager, float deltaTime)
    {
    }
}
