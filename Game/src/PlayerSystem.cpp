#include "PlayerSystem.h"
#include "Game.h"

PlayerSystem::PlayerSystem(Game *game) : _game(game)
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
    _player_sprites = std::map<std::string, std::shared_ptr<Mage::Sprite>>();
    _player_sprites["hero_idle"] = std::make_shared<Mage::Sprite>("res/sprites/hero_idle.png", 9, 0.15f);
    _player_sprites["hero_run"] = std::make_shared<Mage::Sprite>("res/sprites/hero_run.png", 7, 0.15f);
    _player_sprites["hero_shoot"] = std::make_shared<Mage::Sprite>("res/sprites/hero_shoot.png", 6, 0.05f);
    _player_sprites["hero_jump"] = std::make_shared<Mage::Sprite>("res/sprites/hero_jump.png", 4, 0.15f);
    _player_sprites["hero_fall"] = std::make_shared<Mage::Sprite>("res/sprites/hero_fall.png", 6, 0.15f);
    _player_sprites["hero_run_shoot"] = std::make_shared<Mage::Sprite>("res/sprites/hero_run_shoot.png", 7, 0.15f);
    _player_sprites["hero_fall_shoot"] = std::make_shared<Mage::Sprite>("res/sprites/hero_fall_shoot.png", 6, 0.05f);
    _player_sprites["explosion"] = std::make_shared<Mage::Sprite>("res/sprites/explosion.png", 17, 0.08f);
    _player_sprites["bullet"] = std::make_shared<Mage::Sprite>("res/sprites/bullet.png", 7, 0.05f);
    //create player entity
    create_player_entity();
    reset_player_entity();

}

void PlayerSystem::create_player_entity()
{
    _player_entity = _game->get_entity_manager()->add_entity(0);
    _game->get_component_manager()->add_component(*_player_entity, PlayerComponent{});
    _game->get_component_manager()->add_component(*_player_entity, GravityComponent{});
    _game->get_component_manager()->add_component(*_player_entity, SpriteComponent{});
    _game->get_component_manager()->add_component(*_player_entity, RigidBody2DComponent{});
    _game->get_component_manager()->add_component(*_player_entity, Transform2DComponent{});
    _game->get_component_manager()->add_component(*_player_entity, BoundingBoxComponent{});
}

void PlayerSystem::reset_player_entity()
{
    auto g = _game->get_component_manager()->get_component<GravityComponent>(*_player_entity);
    auto s = _game->get_component_manager()->get_component<SpriteComponent>(*_player_entity);
    auto t = _game->get_component_manager()->get_component<Transform2DComponent>(*_player_entity);
    auto r = _game->get_component_manager()->get_component<RigidBody2DComponent>(*_player_entity);

    s->sprite = _player_sprites["hero_idle"].get();
    g->force = glm::vec2(0.0f, -1000.0f);
    t->scale = glm::vec2(0.5f, 0.5f);
    t->prev_translation = t->translation = {
        (_game->get_window()->get_width() - static_cast<float>(s->sprite->get_width()) * 0.25f ) / 2.0f,
        (_game->get_window()->get_height() - static_cast<float>(s->sprite->get_height()) * 0.5f) / 2.0f
    };
    r->velocity = glm::vec2(0.0f, 0.0f);
}

void PlayerSystem::on_key_down(Mage::Key key, uint16_t key_modifiers, uint8_t repeat_count)
{
}

void PlayerSystem::on_key_up(Mage::Key key, uint16_t key_modifiers)
{
}

void PlayerSystem::on_controller_axis_motion(uint32_t controller_id, uint8_t axis_id, float axis_value)
{
}

void PlayerSystem::on_controller_button_down(uint32_t controller_id, uint8_t button_id)
{
}

void PlayerSystem::on_controller_button_up(uint32_t controller_id, uint8_t button_id)
{
}

void PlayerSystem::update(Mage::ComponentManager &componentManager, float deltaTime)
{
}

void PlayerSystem::on_mouse_button_down(Mage::MouseButton button, float x, float y, uint8_t click_count)
{
}


