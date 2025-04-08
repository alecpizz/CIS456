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
        //use this to determine when an enemy hits the edge of the screen if needed
        auto window_height = _game->get_window()->get_height();
        auto window_width = _game->get_window()->get_width();
    }

    void EnemyControllerSystem::create_enemy_entity()
    {
        _enemy_entity = _game->get_entity_manager()->add_entity(69);
        _game->get_component_manager()->add_component(*_enemy_entity, EnemyComponent{});
        _game->get_component_manager()->add_component(*_enemy_entity, SpriteComponent{});
        _game->get_component_manager()->add_component(*_enemy_entity, RigidBody2DComponent{});
        _game->get_component_manager()->add_component(*_enemy_entity, Transform2DComponent{});
        _game->get_component_manager()->add_component(*_enemy_entity, BoundingBoxComponent{
            .on_collided = [&](Mage::Entity* hero, Mage::Entity* other, const glm::vec2 overlap)
            {
                collision_detected(other, overlap);
            } });
        _game->get_component_manager()->add_component(*_enemy_entity, DestructionNotificationComponent
            {
                .on_destroyed = [&]()
                    {
                        //TODO: Do Something? Or not
                    }
            });
    }

    void EnemyControllerSystem::place_enemy_entity()
    {
        auto s = GPEC(SpriteComponent);
        auto t = GPEC(Transform2DComponent);
        auto r = GPEC(RigidBody2DComponent);
        auto b = GPEC(BoundingBoxComponent);

        s->sprite = _enemy_sprites["#"].get();
        r->velocity = { 0.0f, 0.0f };
        t->scale = { SCALE_ENEMY, SCALE_ENEMY };
        b->center = { BBOX_RIGHT_FACING_CENTER_X_ENEMY, BBOX_CENTER_Y_ENEMY };
        b->half_size = { BBOX_HALF_WIDTH_ENEMY, BBOX_HALF_HEIGHT_ENEMY };
        //Will need to change where the enemy spawns in by changing the translation 
        t->prev_translation = t->translation = {
            (static_cast<float>(_game->get_window()->get_width())
                - static_cast<float>(s->sprite->get_width()) * 0.25f) / 2.0f,
            (static_cast<float>(_game->get_window()->get_height())
                - static_cast<float>(s->sprite->get_height()) * 0.5f) / 2.0f
        };
    }

    void EnemyControllerSystem::spawn()
    {
        create_enemy_entity();
        place_enemy_entity();
    }

    void EnemyControllerSystem::shoot()
    {
        //Set up like the player shoot later
    }

    void EnemyControllerSystem::update_enemy_velocity(RigidBody2DComponent* r, float dt)
    {
        auto prior_x_vel = r->velocity.x;
        r->velocity.x = 0.0f;
        auto prior_y_vel = r->velocity.y;
        r->velocity.y = 0.0f;
        
        //TODO: update r-velocity.x/y to 1.0f or -1.0f to change the direction

        r->velocity.x *= VELOCITY_ENEMY;
        r->velocity.y *= VELOCITY_ENEMY;
    }

    void EnemyControllerSystem::update(Mage::ComponentManager& component_manager, float delta_time)
    {
        auto enemy_list = _game->get_entity_manager()->get_all_entities_by_type(69);

        for (auto e : enemy_list)
        {
            //This may not be needed, Enemy velocity may change based on collision
            auto er = GPEC(RigidBody2DComponent);
            update_enemy_velocity(er, delta_time);
        }
    }

    void EnemyControllerSystem::collision_detected(Mage::Entity* other_entity, const glm::vec2& overlap)
    {
        //have any collision reverse the direction of travel, either velocity.x or velocity.y or both
    }
}