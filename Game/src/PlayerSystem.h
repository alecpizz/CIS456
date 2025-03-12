#pragma once

#include <Mage/Mage.h>
#include "Components.h"

class Game;

class PlayerSystem final :
        public Mage::System,
        public Mage::OnKeyDownEventListener,
        public Mage::OnKeyUpEventListener,
        public Mage::OnControllerAxisMotionEventListener,
        public Mage::OnControllerButtonDownEventListener,
        public Mage::OnControllerButtonUpEventListener,
        public Mage::OnMouseButtonDownEventListener
{
public:
    explicit PlayerSystem(Game *game);

    ~PlayerSystem() override = default;

    void initialize();

    PlayerSystem(PlayerSystem &&) = delete;

    PlayerSystem(const PlayerSystem &) = delete;

    PlayerSystem &operator=(PlayerSystem &&) = delete;

    PlayerSystem &operator=(const PlayerSystem &) = delete;

    void on_key_down(Mage::Key key, uint16_t key_modifiers, uint8_t repeat_count) override;

    void on_key_up(Mage::Key key, uint16_t key_modifiers) override;

    void on_controller_axis_motion(uint32_t controller_id, uint8_t axis_id, float axis_value) override;

    void on_controller_button_down(uint32_t controller_id, uint8_t button_id) override;

    void on_controller_button_up(uint32_t controller_id, uint8_t button_id) override;

    void update(Mage::ComponentManager &componentManager, float deltaTime) override;

    void on_mouse_button_down(Mage::MouseButton button, float x, float y, uint8_t click_count) override;

    void collision_detected(Mage::Entity* other_entity, const glm::vec2& overlap);

private:
    Game* _game;
    Mage::Entity* _player_entity;
    std::map<std::string, std::shared_ptr<Mage::Sprite>> _player_sprites;
    uint8_t _wasd_states = 0;
    void create_player_entity();
    void reset_player_entity();
};
