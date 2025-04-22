//
// Created by alecp on 4/1/2025.
//
#include "EnemySpawner.h"

#include "Galaga.h"

#define VELOCITY_ENEMY          50.0f
#define SCALE_ENEMY             0.15f
#define BULLET_PROBABILITY      0.01f
#define MAX_FIRST_THROW_DELAY   2.0f
#define MIN_TIME_BETWEEN_BULLET 3.0f
#define VELOCITY_BULLET         400.0f
#define SCALE_BULLET            0.5f
#define LIFETIME_BULLET         5.0f
#define BULLET_X_VELOCITY       150.0f
#define ROWS                    3
#define COLS                    5


namespace Galaga
{
    EnemySpawner::EnemySpawner(Galaga *galaga) : _game(galaga), _rows(ROWS), _cols(COLS)
    {
        _rands.add_uniform_real_distribution("enemy_velocity", -1.0f, 2.0f);
        _rands.add_uniform_real_distribution("bullet_probability", 0.0f, 1.0f);
        _rands.add_uniform_real_distribution("bullet_x_velocity", -BULLET_X_VELOCITY, BULLET_X_VELOCITY);
        _rands.add_uniform_real_distribution("first_throw_delay", 0.0f, MAX_FIRST_THROW_DELAY);
    }

    void EnemySpawner::initialize()
    {
        //create sprite
        _enemy_sprites = std::map<std::string, std::shared_ptr<Mage::Sprite> >();
        _enemy_sprites["penguin"] = std::make_shared<Mage::Sprite>("res/sprites/penguin.png", 1, 0.0f);
        _enemy_sprites["snowball"] = std::make_shared<Mage::Sprite>("res/sprites/snowball.png", 1, 0.0f);
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
        float buffer_x = 1.0f;
        float buffer_y = 1.0f;
        float enemy_width = 60;
        float enemy_height = 60;
        glm::vec2 origin = glm::vec2(_game->get_window()->get_width() / 2, _game->get_window()->get_height() - _game->get_window()->get_height()/4);
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
        _enemy_instances[_enemy_entity->get_id()] = std::make_unique<Mage::Sprite>(_enemy_sprites["penguin"].get());
        auto sprite = _enemy_instances[_enemy_entity->get_id()].get();
        _game->get_component_manager()->add_component(*_enemy_entity, EnemyComponent
            {
                .first_throw_delay = _rands.get_uniform_real("first_throw_delay")
        });
        _game->get_component_manager()->add_component(*_enemy_entity, SpriteComponent{
            .sprite = sprite
            });
        _game->get_component_manager()->add_component(*_enemy_entity, RigidBody2DComponent{
            .velocity = { _rands.get_uniform_real("enemy_velocity") * VELOCITY_ENEMY,
                        _rands.get_uniform_real("enemy_velocity") * VELOCITY_ENEMY }
            });
        _game->get_component_manager()->add_component(*_enemy_entity, Transform2DComponent{
            .translation = pos,
            .scale = glm::vec2(SCALE_ENEMY, SCALE_ENEMY )
            });
        auto enemy_half_x = static_cast<float>(sprite->get_width() * SCALE_ENEMY) ;
        auto enemy_half_y = static_cast<float>(sprite->get_height() * SCALE_ENEMY) ;
        _game->get_component_manager()->add_component(*_enemy_entity, BoundingBoxComponent{
            .center = { enemy_half_x + 85, enemy_half_y + 70 },
            .half_size = { enemy_half_x + 40, enemy_half_y + 80},
            .on_collided = [&](Mage::Entity* enemy, Mage::Entity* other, const glm::vec2 overlap)
            {
                collision_detected(enemy, other, overlap);
            } });
        /*_game->get_component_manager()->add_component(*_enemy_entity, ColorComponent{
               .color = Mage::Color::custom(0.7f, 0.2f, 0.2f, 0.7f)
            });*/
        _game->get_component_manager()->add_component(*_enemy_entity, DestructionNotificationComponent
            {
                .on_destroyed = [&]()
                {
                //TODO: Do Something? Or not

                }
            });
        _enemies.push_back(_enemy_entity);
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
            
            auto r = _game->get_component_manager()->get_component<RigidBody2DComponent>(*enemy);
            auto bb = _game->get_component_manager()->get_component<BoundingBoxComponent>(*enemy);
            auto t = _game->get_component_manager()->get_component<Transform2DComponent>(*enemy);
            //auto c = _game->get_component_manager()->get_component<ColorComponent>(*enemy);
            
            auto prev_overlap = CollisionSystem::calculate_overlap(t->prev_translation, t->scale,
                oe_t->translation, oe_t->scale, bb, oe_bb);

            //Need to look into enemies hitting other enemies from behind.
            //They even if hit from behind flip backwards
            //c-color is only used when we have a color component instead of sprite
            if (prev_overlap.y > 0.1f)
            {
                //c->color = Mage::Color::aquamarine;
                t->translation.x = t->prev_translation.x;
                r->velocity.x *= -1.0f ;
            }
            if (prev_overlap.x > 0.1f)
            {
                //c->color = Mage::Color::magenta;
                t->translation.y = t->prev_translation.y;
                r->velocity.y *= -1.0f;
            }
        }
    }

    void EnemySpawner::shoot(float delta_time)
    {
        Mage::EntityList enemy_list = _game->get_entity_manager()->get_all_entities_by_type(Galaga::EntityType::Enemy);
        for (auto e : enemy_list)
        {
            //This is where we will set up the enemies shooting.
            //set it up the same way as the frequency of bats spawning
            //in the in class game, also set up angle of shooting to be random
            auto ec = _game->get_component_manager()->get_component<EnemyComponent>(*e);
            ec->last_bullet += delta_time;
            ec->first_throw_delay -= delta_time;

            if (ec->last_bullet >= MIN_TIME_BETWEEN_BULLET
                && _rands.get_uniform_real("bullet_probability") < BULLET_PROBABILITY
                && ec->first_throw_delay <= 0.0f)
            {
                ec->last_bullet = 0.0f;

                //This is the spawn bullet logic
                auto s = _enemy_sprites["snowball"].get();
                auto t = _game->get_component_manager()->get_component<Transform2DComponent>(*e);
                auto eb = _game->get_entity_manager()->add_entity(Galaga::EntityType::Bullet);
                _game->get_component_manager()->add_component<SpriteComponent>(*eb, { .sprite = s });
                /*_game->get_component_manager()->add_component<ColorComponent>(*eb,
                    {
                       .color = Mage::Color::custom(0.7f, 0.7f, 0.0f, 0.7f)
                    });*/
                _game->get_component_manager()->add_component<RigidBody2DComponent>(*eb,
                    {
                        //use _rands to change x for angles
                            .velocity = {_rands.get_uniform_real("bullet_x_velocity"), -VELOCITY_BULLET}
                    });
                _game->get_component_manager()->add_component<Transform2DComponent>(*eb,
                    {
                        .translation = {t->translation.x - 7.5f, t->translation.y - 20.0f},
                        .scale = {SCALE_BULLET, SCALE_BULLET}
                    });
                _game->get_component_manager()->add_component<LifetimeComponent>(*eb,
                    {
                        .remaining = LIFETIME_BULLET
                    });
                auto bullet_half_x = 0.5f;
                auto bullet_half_y = 0.5f;
                _game->get_component_manager()->add_component<BoundingBoxComponent>(*eb, {
                    .center = {bullet_half_x, bullet_half_y},
                    .half_size = {bullet_half_x, bullet_half_y},
                    .on_collided = [&](Mage::Entity* bullet, Mage::Entity* other, const glm::vec2& overlap)
                    {
                        //kill_player(bullet, other);
                    }
                    });
            }
        }
    }

    void EnemySpawner::update(Mage::ComponentManager& componentManager, float delta_time)
    {
        shoot(delta_time);
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
