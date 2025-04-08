#include "PlayerSystem.h"
#include "Game.h"

#define DURATION_JUMPING 0.15f
#define DURATION_JUMPING_SMALL 0.15f
#define VELOCITY_JUMP 500.0f
#define VELOCITY_JUMP_SMALL 350.0f
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
    _player_sprites["hero_idle"] = std::make_shared<Mage::Sprite>("res/sprites/hero_idle.png", 9, 0.15f);
    _player_sprites["hero_run"] = std::make_shared<Mage::Sprite>("res/sprites/hero_run.png", 7, 0.15f);
    _player_sprites["hero_shoot"] = std::make_shared<Mage::Sprite>("res/sprites/hero_shoot.png", 6, 0.05f);
    _player_sprites["hero_jump"] = std::make_shared<Mage::Sprite>("res/sprites/hero_jump.png", 4, 0.15f);
    _player_sprites["hero_fall"] = std::make_shared<Mage::Sprite>("res/sprites/hero_fall.png", 6, 0.15f);
    _player_sprites["hero_run_shoot"] = std::make_shared<Mage::Sprite>("res/sprites/hero_run_shoot.png", 7, 0.15f);
    _player_sprites["hero_fall_shoot"] = std::make_shared<Mage::Sprite>("res/sprites/hero_fall_shoot.png", 6, 0.05f);
    _player_sprites["explosion"] = std::make_shared<Mage::Sprite>("res/sprites/explosion.png", 17, 0.08f);
    _player_sprites["bullet"] = std::make_shared<Mage::Sprite>("res/sprites/bullet.png", 7, 0.05f);
    _font = std::make_unique<Mage::Font>("res/fonts/OpenSans-Regular.ttf");
    spawn();
}

void PlayerSystem::shoot()
{
    _shooting = true;
    _last_shot = 0.0f;
    add_bullet();
}

void PlayerSystem::spawn()
{
    create_player_entity();
    reset_player_entity();
}

void PlayerSystem::jump()
{
    _jump_button_down_at = std::chrono::system_clock::now();
    _jumping = true;
    _last_jump = 0.0f;
    _player_sprites["hero_jump"]->start_over();
}

void PlayerSystem::add_bullet()
{
    auto t = GPEC(Transform2DComponent);
    auto bullet_dir = _left_facing ? -1.0f : 1.0f;
    auto e = _game->get_entity_manager()->add_entity(1);
    _game->get_component_manager()->add_component<SpriteComponent>(*e, {.sprite = _player_sprites["bullet"].get()});
    _game->get_component_manager()->add_component<RigidBody2DComponent>(*e, {
                                                                            .velocity = {
                                                                                bullet_dir * VELOCITY_BULLET, 0.0f
                                                                            }
                                                                        });
    _game->get_component_manager()->add_component<Transform2DComponent>(*e, {
                                                                            .translation =
                                                                            {
                                                                                t->translation.x + (
                                                                                    BULLET_REL_2_PLAYER_X * bullet_dir),
                                                                                t->translation.y + BULLET_REL_2_PLAYER_Y
                                                                            },
                                                                            .scale = {SCALE_BULLET, SCALE_BULLET}
                                                                        });
    _game->get_component_manager()->add_component<LifetimeComponent>(*e,
                                                                     {
                                                                         .remaining = LIFETIME_BULLET
                                                                     });
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

    _game->get_component_manager()->add_component(*_player_entity, DestructionNotificationComponent
                                                  {
                                                      .on_destroyed = [&]()
                                                      {
                                                          _deaths++;
                                                          spawn();
                                                      }
                                                  });
}

void PlayerSystem::reset_player_entity()
{
    auto g = GPEC(GravityComponent);
    auto s = GPEC(SpriteComponent);
    auto t = GPEC(Transform2DComponent);
    auto r = GPEC(RigidBody2DComponent);
    auto b = GPEC(BoundingBoxComponent);

    s->sprite = _player_sprites["hero_idle"].get();
    g->force = glm::vec2(0.0f, -GRAVITY_PLAYER);
    t->scale = glm::vec2(SCALE_PLAYER, SCALE_PLAYER);
    t->prev_translation = t->translation = {
                              (_game->get_camera()->left + 0.15 * _game->get_window()->get_width()),
                              (_game->get_camera()->top - static_cast<float>(s->sprite->get_height()) * SCALE_PLAYER
                                  - static_cast<float>(_game->get_window()->get_height()) * 0.667f)
                          };
    b->center = {BBOX_RIGHT_FACING_CENTER_X_PLAYER, BBOX_CENTER_Y_PLAYER};
    b->half_size = {BBOX_HALF_WIDTH_PLAYER, BBOX_HALF_HEIGHT_PLAYER};
    r->velocity = glm::vec2(0.0f, 0.0f);
}

void PlayerSystem::update_player_velocity(RigidBody2DComponent *r, float dt)
{
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

    r->velocity.x *= VELOCITY_PLAYER;


    _last_jump += dt;
    if (_jumping && _last_jump > DURATION_JUMPING)
    {
        _falling = true;
        _jumping = false;

        r->velocity.y = VELOCITY_JUMP;
        auto jump_button_down_duration =
                std::chrono::duration<float>(_jump_button_up_at - _jump_button_down_at).count();
        if (jump_button_down_duration > 0.0f && jump_button_down_duration < DURATION_JUMPING_SMALL)
        {
            r->velocity.y = VELOCITY_JUMP_SMALL;
        }
    }

    // if (r->velocity.y < 0.0f)
    // {
    //     _falling = true;
    // }
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

    if (key == Mage::Key::Return && !_jumping)
    {
        shoot();
    } else if (key == Mage::Key::Space && !(_jumping || _falling))
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

    if (key == Mage::Key::Space)
    {
        _jump_button_up_at = std::chrono::system_clock::now();
    }
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
    auto s = GPEC(SpriteComponent);
    auto t = GPEC(Transform2DComponent);
    auto b = GPEC(BoundingBoxComponent);

    _last_shot += deltaTime;
    if (_last_shot > DURATION_SHOOTING)
    {
        _shooting = false;
    }

    update_player_velocity(r, deltaTime);
    update_player_sprite(r, s, t, b);
    update_camera(r, s, t, deltaTime);

    _game->get_text_renderer()->render_text(*_font, std::string("Deaths: " + std::to_string(_deaths)).c_str(),
                                            20.0f, 20.0f + static_cast<float>(_font->get_line_height()), 1.0f,
                                            Mage::Color::white);
    // auto bullets = _game->get_entity_manager()->get_all_entities_by_type(1).size();
    // LOG_INFO("Living bullets %d", bullets);
}

void PlayerSystem::update_player_sprite(const RigidBody2DComponent *r, SpriteComponent *sprite, Transform2DComponent *t,
                                        BoundingBoxComponent *b)
{
    auto running = std::abs(r->velocity.x) > 0.1f;
    if (_jumping)
    {
        sprite->sprite = _player_sprites["hero_jump"].get();
    } else if (_falling && !_shooting)
    {
        //else if falling and not shooting
        sprite->sprite = _player_sprites["hero_fall"].get();
    } else if (_falling && _shooting)
    {
        //else if falling and shooting
        sprite->sprite = _player_sprites["hero_fall_shoot"].get();
    } else if (running && !_shooting)
    {
        //else if running and not shooting
        sprite->sprite = _player_sprites["hero_run"].get();
    } else if (running && _shooting)
    {
        //else if running and shooting
        sprite->sprite = _player_sprites["hero_run_shoot"].get();
    } else if (_shooting)
    {
        //else if shooting
        sprite->sprite = _player_sprites["hero_shoot"].get();
    } else
    {
        sprite->sprite = _player_sprites["hero_idle"].get();
    }

    auto prior_left_facing = _left_facing;
    if (r->velocity.x < -0.1f)
    {
        _left_facing = true;
    } else if (r->velocity.x > 0.1f)
    {
        _left_facing = false;
    }

    if (_left_facing && !prior_left_facing)
    {
        t->translation.x -= OFFSET_PLAYER_CENTER;
    } else if (!_left_facing && prior_left_facing)
    {
        t->translation.x += OFFSET_PLAYER_CENTER;
    }

    sprite->sprite->set_flip_x(_left_facing);
    b->center.x = _left_facing ? BBOX_LEFT_FACING_CENTER_X_PLAYER : BBOX_RIGHT_FACING_CENTER_X_PLAYER;
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

void PlayerSystem::update_camera(const RigidBody2DComponent *r, const SpriteComponent *s, const Transform2DComponent *t,
                                 float dt)
{
    auto window_pos_x = _game->get_camera()->right - t->translation.x;
    auto window_width = static_cast<float>(_game->get_window()->get_width());
    auto scaled_sprite_width = static_cast<float>(s->sprite->get_width()) * t->scale.x;
    auto scrolling_margin = window_width * SCROLL_MARGIN;
    if ((r->velocity.x > 0.0f && window_pos_x - scaled_sprite_width < scrolling_margin) || (
            r->velocity.x < 0.0f && window_pos_x > window_width - scrolling_margin))
    {
        _game->get_camera()->left += r->velocity.x * dt;
        _game->get_camera()->right += r->velocity.x * dt;
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
            _falling = false;
        }
        t->translation.y = t->prev_translation.y;
    }
}
