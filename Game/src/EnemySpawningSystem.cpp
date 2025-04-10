#include "EnemySpawningSystem.h"
#include "Game.h"

#define MIN_TIME_BETWEEN_SPAWNS 1.0f
#define SPAWN_PROBABILITY 0.3f

EnemySpawningSystem::EnemySpawningSystem(Game &game) : _game(&game)
{
    _enemy_sprite = std::make_unique<Mage::Sprite>("res/sprites/enemy.png", 4, 0.05);
    _game->get_rands()->add_uniform_real_distribution("spawn_probability", 0.0f, 1.0f);
    _game->get_rands()->add_uniform_real_distribution("spawn_y_pos", 50.0f, _game->get_camera()->top);
}

void EnemySpawningSystem::update(Mage::ComponentManager &componentManager, float deltaTime)
{
    _last_spawn += deltaTime;

    if (_last_spawn >= MIN_TIME_BETWEEN_SPAWNS && _game->get_rands()->get_uniform_real("spawn_probability") <
        SPAWN_PROBABILITY)
    {
        _last_spawn = 0.0f;

        //make the enemy
        auto e = _game->get_entity_manager()->add_entity(Game::EntityType::Enemy);
        _enemy_instances[e->get_id()] = std::make_unique<Mage::Sprite>(_enemy_sprite.get());

        componentManager.add_component<EnemyComponent>(*e, {});

        componentManager.add_component<Transform2DComponent>(*e, {
                                                                 .translation = glm::vec2(_game->get_camera()->right,
                                                                     _game->get_rands()->
                                                                     get_uniform_real("spawn_y_pos"))
                                                             });
        componentManager.add_component<RigidBody2DComponent>(*e,
                                                             {.velocity = glm::vec2(-500.0f, 0.0f)});

        componentManager.add_component<SpriteComponent>(*e, {
                                                            .sprite = _enemy_instances[e->get_id()].get()
                                                        });
        componentManager.add_component<BoundingBoxComponent>(*e, {.center = glm::vec2(22.0f, 17.5f),
            .half_size = glm::vec2(22.0f, 17.5f)});
    }

    for (auto& e : get_entities())
    {
        if (e->is_destroyed())
        {
            _enemy_instances.erase(e->get_id());
            continue;
        }
        auto t = componentManager.get_component<Transform2DComponent>(*e);
        if (t->translation.x < _game->get_camera()->left)
        {
            e->destroy();
        }
    }
}
