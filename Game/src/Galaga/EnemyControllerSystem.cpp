#include "EnemyControllerSystem.h"
#include "Galaga.h"

#define VELOCITY_ENEMY 500.0f
#define SCALE_ENEMY 0.5f
#define OFFSET_ENEMY_CENTER 24.0f
#define GRAVITY_ENEMY 1000.0f
#define SCROLL_MARGIN 0.33f
#define BBOX_RIGHT_FACING_CENTER_X_ENEMY 47.0f
#define BBOX_LEFT_FACING_CENTER_X_ENEMY 77.0f
#define BBOX_CENTER_Y_ENEMY 79.0f
#define BBOX_HALF_WIDTH_ENEMY 37.0f
#define BBOX_HALF_HEIGHT_ENEMY 79.0f
#define BULLET_REL_2_ENEMY_X 36.0f
#define BULLET_REL_2_ENEMY_Y 27.0f
#define VELOCITY_BULLET 1000.0f
#define SCALE_BULLET 0.33f
#define LIFETIME_BULLET 3.0f
#define DURATION_SHOOTING 0.3f

#define GPEC(T) _game->get_component_manager()->get_component<T>(*_enemy_entity)

namespace Galaga
{
    EnemyControllerSystem::EnemyControllerSystem(Galaga* game) : _game(game)
    {
        auto window_height = _game->get_window()->get_height();
        auto window_width = _game->get_window()->get_width();
    }

    void EnemyControllerSystem::shoot()
    {
        _shooting = true;
        _last_shot = 0.0f;
        auto t = GPEC(Transform2DComponent);
        auto e = _game->get_entity_manager()->add_entity(1);
        //_game->get_component_manager()->add_component<SpriteComponent>(*e, { .sprite = _enemy_sprites["bullet"].get() });
        _game->get_component_manager()->add_component<ColorComponent>(*e,
            {
               .color = Mage::Color::custom(0.7f, 0.7f, 0.0f, 0.7f)
            });
        _game->get_component_manager()->add_component<RigidBody2DComponent>(*e,
            {
                .velocity = {0.0f, -VELOCITY_BULLET}
            });
        _game->get_component_manager()->add_component<Transform2DComponent>(*e,
            {
                .translation = {t->translation.x - 7.5f, t->translation.y - 20.0f},
                .scale = {5.0f, 5.0f}
            });
        _game->get_component_manager()->add_component<LifetimeComponent>(*e,
            {
                .remaining = LIFETIME_BULLET
            });
    }

    void EnemyControllerSystem::update_enemy_velocity(RigidBody2DComponent* r, float dt)
    {
        auto prior_vel = r->velocity.x;
        r->velocity.x = 0.0f;
        //update r-velocity.x to 1.0f or -1.0f to change the direction
        

        r->velocity.x *= VELOCITY_ENEMY;
    }

    void EnemyControllerSystem::update(Mage::ComponentManager& component_manager, float delta_time)
    {
    }
}