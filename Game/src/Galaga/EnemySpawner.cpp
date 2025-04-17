//
// Created by alecp on 4/1/2025.
//
#include "EnemySpawner.h"

#include "Galaga.h"

#define VELOCITY_ENEMY 50.0f
#define SCALE_ENEMY 0.5f
#define OFFSET_ENEMY_CENTER 24.0f
#define BBOX_RIGHT_FACING_CENTER_X_ENEMY 0.5f
#define BBOX_LEFT_FACING_CENTER_X_ENEMY 77.0f
#define BBOX_CENTER_Y_ENEMY 0.5f
#define BBOX_HALF_WIDTH_ENEMY 0.5f
#define BBOX_HALF_HEIGHT_ENEMY 0.5f
#define ROWS 5
#define COLS 5

#define VELOCITY_BULLET 1000.0f
#define SCALE_BULLET 5.0f
#define LIFETIME_BULLET 3.0f
#define DURATION_SHOOTING 0.3f

#define GPEC(T) _game->get_component_manager()->get_component<T>(*_enemy_entity)

namespace Galaga
{
    EnemySpawner::EnemySpawner(Galaga *galaga) : _game(galaga), _rows(ROWS), _cols(COLS)
    {
        _rands.add_uniform_real_distribution("enemy_velocity", -1.0f, 2.0f);
        _rands.add_uniform_real_distribution("enemy_shooting_rate", 10.0f, 40.0f);
    }

    void EnemySpawner::initialize()
    {
        ///Create Sprite
        spawn();
    }

    void EnemySpawner::spawn()
    {
        uint32_t enemy_count = ROWS * COLS;
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
            
        }
    }

    void EnemySpawner::create_enemy_entity(glm::vec2 pos)
    {
        _enemy_entity = _game->get_entity_manager()->add_entity(Galaga::EntityType::Enemy);
        _game->get_component_manager()->add_component(*_enemy_entity, EnemyComponent{
            .enemy_shooting_rate = _rands.get_uniform_real("enemy_shooting_rate"),
            .last_bullet = 0.0f
            });
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
            .on_collided = [&](Mage::Entity* enemy, Mage::Entity* other, const glm::vec2 overlap)
            {
                collision_detected(enemy, other, overlap);
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
        Mage::EntityList enemy_list = _game->get_entity_manager()->get_all_entities_by_type(Galaga::EntityType::Enemy);

        _last_shot += deltaTime;
        for (auto e : enemy_list)
        {
            auto time_between_shots = ( _game->get_component_manager()->get_component<EnemyComponent>(*e) )->enemy_shooting_rate; // Float
            auto _of_last_bullet = (_game->get_component_manager()->get_component<EnemyComponent>(*e))->last_bullet; // Float

            if (_last_shot > DURATION_SHOOTING + time_between_shots) //Every 3 seconds?
            {
                _shooting = false;
            }
            else {
                shoot();
            }
        }
        /*
        //Shooting after a time
        _last_shot += deltaTime;
        if (_last_shot > DURATION_SHOOTING + 3.0f) //Every 3 seconds?
        {
            _shooting = false;
        }
        else {
            shoot();
        }*/
    }

    void EnemySpawner::collision_detected(Mage::Entity* enemy, Mage::Entity* other_entity, const glm::vec2& overlap)
    {
        if (other_entity->get_type() == Galaga::EntityType::Bullet)
        {
            LOG_INFO("Enemy hit!");
            return;
        }

        if (other_entity->get_type() == Galaga::EntityType::Wall || other_entity->get_type() == Galaga::EntityType::Enemy)
        {
            //LOG_INFO("Enemy Hit Wall.");
            auto oe_bb = _game->get_component_manager()->get_component<BoundingBoxComponent>(*other_entity);
            auto oe_t = _game->get_component_manager()->get_component<Transform2DComponent>(*other_entity);
            

            //GPEC Cannot be used below otherwise collision will only apply to the last generated enemy
            auto r = _game->get_component_manager()->get_component<RigidBody2DComponent>(*enemy);
            auto bb = _game->get_component_manager()->get_component<BoundingBoxComponent>(*enemy);
            auto t = _game->get_component_manager()->get_component<Transform2DComponent>(*enemy);
            auto c = _game->get_component_manager()->get_component<ColorComponent>(*enemy);
            
            auto prev_overlap = CollisionSystem::calculate_overlap(t->prev_translation, t->scale,
                oe_t->translation, oe_t->scale, bb, oe_bb);

            //Need to look into enemies hitting other enemies from behind.
            //They even if hit from behind flip backwards
            if (prev_overlap.y > 0.1f)
            {
                c->color = Mage::Color::aquamarine;
                t->translation.x = t->prev_translation.x;
                r->velocity.x *= -1.0f ;
            }
            if (prev_overlap.x > 0.1f)
            {
                c->color = Mage::Color::magenta;
                t->translation.y = t->prev_translation.y;
                r->velocity.y *= -1.0f;
            }
            
        }

    }



    void EnemySpawner::shoot()
    {
        _shooting = true;
        ////_last_shot = 0.0f;
        auto t = GPEC(Transform2DComponent);
        auto e = _game->get_entity_manager()->add_entity(Galaga::EntityType::Bullet);
        //_game->get_component_manager()->add_component<SpriteComponent>(*e, { .sprite = _player_sprites["bullet"].get() });
        _game->get_component_manager()->add_component<ColorComponent>(*e,
            {
               .color = Mage::Color::custom(0.7f, 0.7f, 0.0f, 0.7f)
            });
        _game->get_component_manager()->add_component<RigidBody2DComponent>(*e,
            {
                .velocity = {0.0f, VELOCITY_BULLET}
            });
        _game->get_component_manager()->add_component<Transform2DComponent>(*e,
            {
                .translation = {t->translation.x + 7.5f, t->translation.y + 20.0f},
                .scale = {SCALE_BULLET, SCALE_BULLET}
            });
        _game->get_component_manager()->add_component<LifetimeComponent>(*e,
            {
                .remaining = LIFETIME_BULLET
            });
        auto bullet_half_x = 0.5f;
        auto bullet_half_y = 0.5f;
        _game->get_component_manager()->add_component<BoundingBoxComponent>(*e, {
            .center = {bullet_half_x, bullet_half_y},
            .half_size = {bullet_half_x, bullet_half_y},
            .on_collided = [&](Mage::Entity* bullet, Mage::Entity* other, const glm::vec2& overlap)
            {
                kill_player(bullet, other);
            }
            });
    }



    void EnemySpawner::kill_player(Mage::Entity* bullet, Mage::Entity* other)
    {
        if (other->get_type() != Galaga::EntityType::Player)
        {
            return;
        }

        bullet->destroy();
        other->destroy();
        //TODO: kill count
    }

}
