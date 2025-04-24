#include "PlayerSystem.h"
#include "Galaga.h"

#define VELOCITY_PLAYER 500.0f
#define SCALE_PLAYER 0.25f //.25 //.5
#define BBOX_CENTER_X_PLAYER 20.0f
#define BBOX_CENTER_Y_PLAYER 20.0f
#define BBOX_HALF_WIDTH_PLAYER 117.5f//20.0f
#define BBOX_HALF_HEIGHT_PLAYER 117.5f//20.0f
#define VELOCITY_BULLET 1000.0f
#define SCALE_BULLET 0.5f
#define LIFETIME_BULLET 3.0f
#define DURATION_SHOOTING 0.1

#define GPEC(T) _game->get_component_manager()->get_component<T>(*_player_entity)

namespace Galaga
{
    PlayerSystem::PlayerSystem(Galaga* game) : _game(game)
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
        _player_sprites["player_idle"] = std::make_shared<Mage::Sprite>("res/sprites/snowmanPlayer.png", 1, 0.0f);
        _player_sprites["snowball"] = std::make_shared<Mage::Sprite>("res/sprites/snowball.png", 1, 0.0f);

        spawn();
    }

    void PlayerSystem::spawn()
    {
        create_player_entity();
        reset_player_entity();
    }

    void PlayerSystem::shoot()
    {
        _shooting = true;
        _last_shot = 0.0f;
        auto t = GPEC(Transform2DComponent);
        auto b = GPEC(BoundingBoxComponent);
        auto e = _game->get_entity_manager()->add_entity(Galaga::EntityType::Bullet);
        auto s = _player_sprites["snowball"].get();
        _game->get_component_manager()->add_component<SpriteComponent>(*e, { .sprite = s });
        _game->get_component_manager()->add_component<RigidBody2DComponent>(*e,
        {
	        .velocity = {0.0f, VELOCITY_BULLET}
        });
        _game->get_component_manager()->add_component<Transform2DComponent>(*e,
        {
        	.translation = {t->translation.x + b->center.x, t->translation.y + 1.5f * b->center.y},
        	.scale = {SCALE_BULLET, SCALE_BULLET}
        });
        _game->get_component_manager()->add_component<LifetimeComponent>(*e,
        {
            .remaining = LIFETIME_BULLET
        });
        auto bullet_half_x = static_cast<float>(s->get_width()) * 0.5f * SCALE_BULLET;
        auto bullet_half_y = static_cast<float>(s->get_height()) * 0.5f * SCALE_BULLET;
        _game->get_component_manager()->add_component<BoundingBoxComponent>(*e, {
            .center = {bullet_half_x, bullet_half_y},
            .half_size = {bullet_half_x, bullet_half_y},
            .on_collided = [&](Mage::Entity* bullet, Mage::Entity* other, const glm::vec2& overlap)
            {
            	kill_enemy(bullet, other);
            }
        });
    }

    void PlayerSystem::create_player_entity()
    {
        _player_entity = _game->get_entity_manager()->add_entity(Galaga::EntityType::Player);
        _game->get_component_manager()->add_component(*_player_entity, PlayerComponent{});
        _game->get_component_manager()->add_component(*_player_entity, SpriteComponent{});
        _game->get_component_manager()->add_component(*_player_entity, RigidBody2DComponent{});
        _game->get_component_manager()->add_component(*_player_entity, Transform2DComponent{});
        _game->get_component_manager()->add_component(*_player_entity, BoundingBoxComponent
	        {
	            .on_collided = [&](Mage::Entity* self, Mage::Entity* other, const glm::vec2& overlap)
	            {
	                collision_detected(other);
	            }
	        });
        _game->get_component_manager()->add_component(*_player_entity, DestructionNotificationComponent
            {
                .on_destroyed = [&]()
                {
                    spawn();
                }
            });
    }

    void PlayerSystem::reset_player_entity()
    {
        auto s = GPEC(SpriteComponent);
        auto t = GPEC(Transform2DComponent);
        auto r = GPEC(RigidBody2DComponent);
        auto b = GPEC(BoundingBoxComponent);
        auto sprite = _player_sprites["player_idle"].get();

        s->sprite = sprite;
        t->scale = glm::vec2(SCALE_PLAYER, SCALE_PLAYER);
        t->prev_translation = t->translation = {
        	(_game->get_window()->get_width() - 20.0f * 0.25f) / 2.0f,
        	20.0f
        };
        auto player_half_x = static_cast<float>(sprite->get_width()) * 0.5f * SCALE_PLAYER;
        auto player_half_y = static_cast<float>(sprite->get_height()) * 0.5f * SCALE_PLAYER;
        b->center = { player_half_x, player_half_y };
        b->half_size = { player_half_x, player_half_y };
        r->velocity = glm::vec2(0.0f, 0.0f);
    }

    void PlayerSystem::update_player_velocity(RigidBody2DComponent* r, Transform2DComponent* t, float delta_time)
    {
        auto s = GPEC(SpriteComponent);
        r->velocity.x = 0.0f;
        if (_wasd_states & 0x02 && t->translation.x > 0.0f)
        {
            r->velocity.x += -1.0f;
        }
		if (_wasd_states & 0x08 && t->translation.x < _game->get_window()->get_width() - s->sprite->get_width() * SCALE_PLAYER)
        {
            r->velocity.x += 1.0f;
        }

        r->velocity.x *= VELOCITY_PLAYER;
    }

    void PlayerSystem::update_player_sprite(const RigidBody2DComponent* r, SpriteComponent* sprite, Transform2DComponent* t,
        BoundingBoxComponent* b)
    {
    	//sprite->sprite = _player_sprites["player_idle"].get();
    }

    void PlayerSystem::kill_enemy(Mage::Entity* bullet, Mage::Entity* other)
    {
        if (other->get_type() != Galaga::EntityType::Enemy)
        {
            return;
        }

        bullet->destroy();
        other->destroy();
        //TODO: kill count
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

        if (key == Mage::Key::Return)
            _shooting = true;
    }

    void PlayerSystem::on_key_up(Mage::Key key, uint16_t key_modifiers)
    {
        _wasd_states &= (key == Mage::Key::W) ? ~0x01 : 0xFF;
        _wasd_states &= (key == Mage::Key::A) ? ~0x02 : 0xFF;
        _wasd_states &= (key == Mage::Key::S) ? ~0x04 : 0xFF;
        _wasd_states &= (key == Mage::Key::D) ? ~0x08 : 0xFF;

        if (key == Mage::Key::Return)
            _shooting = false;
    }

    void PlayerSystem::on_controller_axis_motion(uint32_t controller_id, uint8_t axis_id, float axis_value) {}
    void PlayerSystem::on_controller_button_down(uint32_t controller_id, uint8_t button_id) {}
    void PlayerSystem::on_controller_button_up(uint32_t controller_id, uint8_t button_id) {}
    void PlayerSystem::on_mouse_button_down(Mage::MouseButton button, float x, float y, uint8_t click_count)
    {
        /*if (button == Mage::MouseButton::Left)
        {
            LOG_INFO("Mouse left clicked (%f, %f)", x, y);
            auto t = GPEC(Transform2DComponent);
            auto r = GPEC(RigidBody2DComponent);
            t->prev_translation = t->translation = { x, y };
            r->velocity = glm::vec2(0.0f, 0.0f);
        }*/
    }

    void PlayerSystem::update(Mage::ComponentManager& component_manager, float delta_time)
    {
        auto r = GPEC(RigidBody2DComponent);
        auto s = GPEC(SpriteComponent);
        auto t = GPEC(Transform2DComponent);
        auto b = GPEC(BoundingBoxComponent);

        _last_shot += delta_time;
        if (_shooting && _last_shot > DURATION_SHOOTING)
            shoot();

        update_player_velocity(r, t, delta_time);
        update_player_sprite(r, s, t, b);
    }

    void PlayerSystem::collision_detected(Mage::Entity* other_entity)
    {
        if (other_entity->get_type() == Galaga::EntityType::Enemy)
        {
            //other_entity->destroy();
            //_player_entity->destroy();
            LOG_INFO("You hit an enemy!");
            return;
        }

        if (other_entity->get_type() == Galaga::EntityType::Bullet)
        {
            //_player_entity->destroy();
            //LOG_INFO("You died!");
            return;
        }
    }
}
