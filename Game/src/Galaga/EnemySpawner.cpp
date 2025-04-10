//
// Created by alecp on 4/1/2025.
//
#include "EnemySpawner.h"

#include "Galaga.h"

#define VELOCITY_ENEMY 50.0f
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
        _rands.add_uniform_real_distribution("enemy_velocity", -1.0f, 2.0f);
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
            .on_collided = [&](Mage::Entity* player, Mage::Entity* other, const glm::vec2 overlap)
            {
                collision_detected(other, overlap);
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
        r->velocity = { _rands.get_uniform_real("enemy_velocity") * VELOCITY_ENEMY,
                        _rands.get_uniform_real("enemy_velocity") * VELOCITY_ENEMY };
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

    void EnemySpawner::collision_detected(Mage::Entity* other_entity, const glm::vec2& overlap)
    {
        if (other_entity->get_type() == Galaga::EntityType::Bullet)
        {
            LOG_INFO("Enemy hit!");
            return;
        }

        auto oe_bb = _game->get_component_manager()->get_component<BoundingBoxComponent>(*other_entity);
        auto oe_t = _game->get_component_manager()->get_component<Transform2DComponent>(*other_entity);

        auto r = GPEC(RigidBody2DComponent);
        auto bb = GPEC(BoundingBoxComponent);
        auto t = GPEC(Transform2DComponent);

        auto prev_overlap = CollisionSystem::calculate_overlap(t->prev_translation, t->scale,
            oe_t->translation, oe_t->scale, bb, oe_bb);

        if (prev_overlap.x > 0.0f)
        {
            t->translation.x = t->prev_translation.x;
            r->velocity.x *= -1.0f;
        }
        else if (prev_overlap.y > 0.0f)
        {
            t->translation.y = t->prev_translation.y;
            r->velocity.y *= -1.0f;
        }
        else
        {
            t->translation.x = t->prev_translation.x;
            r->velocity.x *= -1.0f;
            t->translation.y = t->prev_translation.y;
            r->velocity.y *= -1.0f;
        }

        //auto push_x = true;
        //if (prev_overlap.x > 0.0f)
        //{	//previous position was above or below
        //    push_x = false;
        //}
        //else if (prev_overlap.y < 0.0f)
        //{	//The previous position didnt overlap in x or y, 
        //    //push in direction of greater current overlap
        //    push_x = overlap.x > overlap.y;
        //}

        //if (push_x && overlap.y > 0.1f)
        //{
        //    //Which direction is it hitting from?
        //    t->translation.x = t->prev_translation.x;
        //    r->velocity.x = 0.0f;
        //}
        //else if (overlap.x > 0.1f)
        //{
        //    //Which direction is it hitting from
        //    if (r->velocity.y <= 0.0f)
        //    {
        //        r->velocity.y = 0.0f;
        //    }
        //    t->translation.y = t->prev_translation.y;
        //    r->velocity.y = 0.0f;
        //}
    }
}
