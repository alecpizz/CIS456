#pragma once

#pragma warning(disable : 4100)

#include <Mage/Mage.h>
#include "GravitySystem.h"
#include "MovementSystem.h"
#include "ShapeRenderingSystem.h"
#include "PlayerSystem.h"
#include "TorqueSystem.h"
#include "LifetimeSystem.h"
#include "DeathByYSystem.h"
#include "CollisionSystem.h"
#include "DestructionNotificationSystem.h"
#include "EnemySpawningSystem.h"
#include "SpriteRenderingSystem.h"
#include "RandomWrapper.h"

class Game :
        public Mage::Application,
        public Mage::OnAppClosingEventListener
{
public:
    Game();
private:
    std::unique_ptr<GravitySystem> _gravity_system = nullptr;
    std::unique_ptr<ShapeRenderingSystem> _shape_rendering_system = nullptr;
    std::unique_ptr<MovementSystem> _movement_system = nullptr;
    std::unique_ptr<TorqueSystem> _torque_system = nullptr;
    std::unique_ptr<Mage::Controller> _controller = nullptr;
    std::unique_ptr<PlayerSystem> _player_system = nullptr;
    std::unique_ptr<SpriteRenderingSystem> _sprite_rendering_system = nullptr;
    std::unique_ptr<CollisionSystem> _collision_system = nullptr;
    std::unique_ptr<LifetimeSystem> _lifetime_system = nullptr;
    std::unique_ptr<DeathByYSystem> _death_by_y_system = nullptr;
    std::unique_ptr<DestructionNotificationSystem> _destruction_notification_system = nullptr;
    RandomWrapper _rands;
    void on_app_closing() override;
    void add_random_shape();
};