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
#define ROWS 5
#define COLS 5

#define GPEC(T) _game->get_component_manager()->get_component<T>(*_enemy_entity)

namespace Galaga
{
    EnemySpawner::EnemySpawner(Galaga *galaga) : _game(galaga), _rows(ROWS), _cols(COLS)
    {
        _rands.add_uniform_real_distribution("enemy_velocity", -1.0f, 2.0f);
    }

    void EnemySpawner::initialize()
    {
        spawn();
    }

    void EnemySpawner::spawn()
    {
        uint32_t enemy_count = 30;
        uint32_t width = 1;
        uint32_t height = 1;
        while (true)
        {
            if (width * height >= enemy_count)
            {
                break;
            }
            width++;
            if (width * height >= enemy_count)
            {
                break;
            }
            height++;
        }
        float buffer_x = 1.5f;
        float buffer_y = 1.5f;
        float enemy_width = 60;
        float enemy_height = 60;
        glm::vec2 origin = glm::vec2(_game->get_window()->get_width() / 2, _game->get_window()->get_height() / 2);
        float x_first = origin.x - (width * enemy_width / 2) - ((width - 1) * buffer_x / 2);
        float y_first = origin.y - (height * enemy_height / 2) - ((height - 1) * buffer_y / 2);

        //assume all entites have been destroyed...?
        _enemies.clear();
        for (uint32_t i = 0; i < enemy_count; i++)
        {
            uint32_t x = i % width;
            uint32_t y = i / width;
            float xPos = x_first + (x * (enemy_width * buffer_x));
            float yPos = y_first + (y * (enemy_height * buffer_y));
            glm::vec2 pos = glm::vec2(xPos, yPos);

            create_enemy_entity(pos);
            //Go through place enemy entity and adjust the settings into create_enemy_entity gotten from spawn
        }
    }

    void EnemySpawner::create_enemy_entity(glm::vec2 pos)
    {
        _enemy_entity = _game->get_entity_manager()->add_entity(Galaga::EntityType::Enemy);
        _game->get_component_manager()->add_component(*_enemy_entity, EnemyComponent{});
        _game->get_component_manager()->add_component(*_enemy_entity, SpriteComponent{
            //.sprite = _enemy_sprites["#"].get()
            });
        _game->get_component_manager()->add_component(*_enemy_entity, RigidBody2DComponent{
            .velocity = { _rands.get_uniform_real("enemy_velocity") * VELOCITY_ENEMY,
                        _rands.get_uniform_real("enemy_velocity") * VELOCITY_ENEMY }
        });
        _game->get_component_manager()->add_component(*_enemy_entity, Transform2DComponent{
            .translation = pos,
            .scale = glm::vec2(20.0f, 20.0f)
        });
        _game->get_component_manager()->add_component(*_enemy_entity, BoundingBoxComponent{
            .center = { BBOX_RIGHT_FACING_CENTER_X_ENEMY, BBOX_CENTER_Y_ENEMY },
            .half_size = { BBOX_HALF_WIDTH_ENEMY, BBOX_HALF_HEIGHT_ENEMY },
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
        _enemies.push_back(_enemy_entity);
    }

    void EnemySpawner::update(Mage::ComponentManager& componentManager, float deltaTime)
    {
    }

    void EnemySpawner::collision_detected(Mage::Entity* other_entity, const glm::vec2& overlap)
    {
        if (other_entity->get_type() == Galaga::EntityType::Bullet)
        {
            //Currently for some reason it once starting to log enemy hit logs it endlessly
            //LOG_INFO("Enemy hit!");
            return;
        }
        else
        {
            auto oe_bb = _game->get_component_manager()->get_component<BoundingBoxComponent>(*other_entity);
            auto oe_t = _game->get_component_manager()->get_component<Transform2DComponent>(*other_entity);

            auto r = GPEC(RigidBody2DComponent);
            auto bb = GPEC(BoundingBoxComponent);
            auto t = GPEC(Transform2DComponent);

            auto prev_overlap = CollisionSystem::calculate_overlap(t->prev_translation, t->scale,
                oe_t->translation, oe_t->scale, bb, oe_bb);

            if (prev_overlap.x = 0.0f)
            {
                t->translation.x = t->prev_translation.x;
                r->velocity.x *= -1.0f;
            }
            else if (prev_overlap.y = 0.0f)
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
        }

    }
}
