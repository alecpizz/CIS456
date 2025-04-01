#include "StarfighterSystem.h"
#include "Galaga.h"

#define VELOCITY_PLAYER 500.0f
#define SCALE_PLAYER 0.5f
#define OFFSET_PLAYER_CENTER 24.0f
#define GRAVITY_PLAYER 1000.0f
#define SCROLL_MARGIN 0.33f
#define BBOX_RIGHT_FACING_CENTER_X_PLAYER 47.0f
#define BBOX_LEFT_FACING_CENTER_X_PLAYER 77.0f
#define BBOX_CENTER_Y_PLAYER 79.0f
#define BBOX_HALF_WIDTH_PLAYER 37.0f
#define BBOX_HALF_HEIGHT_PLAYER 79.0f
#define BULLET_REL_2_PLAYER_X 36.00f
#define BULLET_REL_2_PLAYER_Y 27.00f
#define VELOCITY_BULLET 1000.0f
#define SCALE_BULLET 0.33f
#define LIFETIME_BULLET 1.00f
#define DURATION_SHOOTING 0.30f

#define GPEC(T) _game->get_component_manager()->get_component<T>(*_player_entity)

StarfighterSystem::StarfighterSystem(Galaga* game) : _game(game)
{
}

void PlayerSystem::initialize()
{
    //register event handlers
    _game->get_event_manager()->add_on_key_down_event_listener(this);
    _game->get_event_manager()->add_on_key_up_event_listener(this);
    _game->get_event_manager()->add_on_controller_axis_motion_event_listener(this);
    _game->get_event_manager()->add_on_controller_button_down_event_listener(this);
    _game->get_event_manager()->add_on_controller_button_up_event_listener(this);
    _game->get_event_manager()->add_on_mouse_button_down_event_listener(this);

    //create sprite
    //_player_sprites = std::map<std::string, std::shared_ptr<Mage::Sprite> >();
    //_player_sprites["hero_idle"] = std::make_shared<Mage::Sprite>("res/sprites/hero_idle.png", 9, 0.15f);

    spawn();
}