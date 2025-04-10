#include "EnemyControllerSystem.h"
#include "Galaga.h"


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

    void EnemyControllerSystem::shoot()
    {
        //Set up like the player shoot later
    }

    void EnemyControllerSystem::update_enemy_velocity(RigidBody2DComponent* r, float dt)
    {
        /*auto prior_x_vel = r->velocity.x;
        r->velocity.x = 0.0f;
        auto prior_y_vel = r->velocity.y;
        r->velocity.y = 0.0f;*/

    }

    void EnemyControllerSystem::update(Mage::ComponentManager& component_manager, float delta_time)
    {
        auto enemy_list = _game->get_entity_manager()->get_all_entities_by_type(69);

        for (auto e : enemy_list)
        {
            //This may not be needed, Enemy velocity may change based on collision
            //auto er = GPEC(RigidBody2DComponent);
            //update_enemy_velocity(er, delta_time);
        }
    }
}