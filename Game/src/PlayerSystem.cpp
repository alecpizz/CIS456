#include "PlayerSystem.h"
#include "Game.h"

#define GPEC(T) _game->get_component_manager()->get_component<T>(*_player_entity)


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
    _player_sprites = std::map<std::string, std::shared_ptr<Mage::Sprite> >();
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

void PlayerSystem::jump()
{
    _jumping = true;
    _last_jump = 0.0f;
}

void PlayerSystem::create_player_entity()
{
    _player_entity = _game->get_entity_manager()->add_entity(0);
    _game->get_component_manager()->add_component(*_player_entity, PlayerComponent{});
    _game->get_component_manager()->add_component(*_player_entity, GravityComponent{});
    _game->get_component_manager()->add_component(*_player_entity, SpriteComponent{});
    _game->get_component_manager()->add_component(*_player_entity, RigidBody2DComponent{});
    _game->get_component_manager()->add_component(*_player_entity, Transform2DComponent{});
    _game->get_component_manager()->add_component(*_player_entity, BoundingBoxComponent
                                                  {
                                                      .on_collided = [&](
                                                  Mage::Entity *self, Mage::Entity *other, const glm::vec2 &overlap)
                                                      {
                                                          collision_detected(other, overlap);
                                                      }
                                                  });
}

void PlayerSystem::reset_player_entity()
{
    auto g = GPEC(GravityComponent);
    auto s = GPEC(SpriteComponent);
    auto t = GPEC(Transform2DComponent);
    auto r = GPEC(RigidBody2DComponent);

    s->sprite = _player_sprites["hero_idle"].get();
    g->force = glm::vec2(0.0f, -1000.0f);
    t->scale = glm::vec2(0.5f, 0.5f);
    t->prev_translation = t->translation = {
                              (_game->get_window()->get_width() - static_cast<float>(s->sprite->get_width()) * 0.25f) /
                              2.0f,
                              (_game->get_window()->get_height() - static_cast<float>(s->sprite->get_height()) * 0.5f) /
                              2.0f
                          };
    r->velocity = glm::vec2(0.0f, 0.0f);
}

void PlayerSystem::on_key_down(Mage::Key key, uint16_t key_modifiers, uint8_t repeat_count)
{
    if (repeat_count > 0) return;
    LOG_INFO("Key down: %d", key);
    _wasd_states |= (key == Mage::Key::W) ? 0x01 : 0;
    _wasd_states |= (key == Mage::Key::A) ? 0x02 : 0;
    _wasd_states |= (key == Mage::Key::S) ? 0x04 : 0;
    _wasd_states |= (key == Mage::Key::D) ? 0x08 : 0;
    LOG_INFO("%d", _wasd_states);
    if (key == Mage::Key::Space)
    {
        jump();
    }
}

void PlayerSystem::on_key_up(Mage::Key key, uint16_t key_modifiers)
{
    _wasd_states &= (key == Mage::Key::W) ? ~0x01 : 0xFF;
    _wasd_states &= (key == Mage::Key::A) ? ~0x02 : 0xFF;
    _wasd_states &= (key == Mage::Key::S) ? ~0x04 : 0xFF;
    _wasd_states &= (key == Mage::Key::D) ? ~0x08 : 0xFF;
    LOG_INFO("%d", _wasd_states);
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
    //TODO: change player velocity based on _wasd_states
    auto r = GPEC(RigidBody2DComponent);
    auto prior_vel = r->velocity.x;
    r->velocity.x = 0.0f;
    if (_wasd_states & 0x02)
    {
        r->velocity.x += -1.0f;
    }
    if (_wasd_states & 0x08)
    {
        r->velocity.x += 1.0f;
    }

    r->velocity.x *= 500.0f;


    _last_jump += deltaTime;
    LOG_INFO("%f", _last_jump);
    if (_jumping && _last_jump > 0.15f)
    {
        _jumping = false;
        r->velocity.y = 500.0f;
    }

    auto s = GPEC(SpriteComponent);
    auto t = GPEC(Transform2DComponent);
    auto window_pos_x = _game->get_camera()->right - t->translation.x;
    auto window_width = static_cast<float>(_game->get_window()->get_width());
    auto scaled_sprite_width = static_cast<float>(s->sprite->get_width()) * t->scale.x;
    auto scrolling_margin = window_width * 0.33f;
    if ((r->velocity.x > 0.0f && window_pos_x - scaled_sprite_width < scrolling_margin) || (
            r->velocity.x < 0.0f && window_pos_x > window_width - scrolling_margin))
    {
        _game->get_camera()->left += r->velocity.x * deltaTime;
        _game->get_camera()->right += r->velocity.x * deltaTime;
    }
}

void PlayerSystem::on_mouse_button_down(Mage::MouseButton button, float x, float y, uint8_t click_count)
{
    if (button == Mage::MouseButton::Left)
    {
        LOG_INFO("Mouse left clicked (%f, %f)", x, y);
        auto t = GPEC(Transform2DComponent);
        auto r = GPEC(RigidBody2DComponent);
        t->prev_translation = t->translation = {x, y};
        r->velocity = glm::vec2(0.0f, 0.0f);
    }
}

void PlayerSystem::collision_detected(Mage::Entity *other_entity, const glm::vec2 &overlap)
{
    auto oe_bb = _game->get_component_manager()->get_component<BoundingBoxComponent>(*other_entity);
    auto oe_t = _game->get_component_manager()->get_component<Transform2DComponent>(*other_entity);

    auto r = GPEC(RigidBody2DComponent);
    auto bb = GPEC(BoundingBoxComponent);
    auto t = GPEC(Transform2DComponent);

    auto prev_overlap = CollisionSystem::calculate_overlap(t->prev_translation, t->scale, oe_t->translation,
                                                           oe_t->scale, bb, oe_bb);

    auto push_x = true;
    if (prev_overlap.x > 0.0f)
    {
        //previous position was above or below
        push_x = false;
    } else if (prev_overlap.y < 0.0f)
    {
        // previous position didn't overlap in either x or y
        // so push in whichever direction has the greater current overlap
        push_x = overlap.x > overlap.y;
    }

    if (push_x && overlap.y > 0.1f)
    {
        t->translation.x = t->prev_translation.x;
        r->velocity.x = 0.0f;
    } else if (overlap.x > 0.1f)
    {
        if (r->velocity.y <= 0.0f)
        {
            //TODO: update our state to falling
            r->velocity.y = 0.0f;
        }
        t->translation.y = t->prev_translation.y;
    }
}
