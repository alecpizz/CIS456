#pragma once
#include <Mage/Mage.h>
#include "ShapeRenderingSystem.h"
#include "MovementSystem.h"
#include "CollisionSystem.h"
#include "PlayerSystem.h"
#include "EnemyControllerSystem.h"
#include "LifetimeSystem.h"
#include "EnemySpawner.h"

namespace Galaga
{
    class Galaga :
            public Mage::Application,
            public Mage::OnAppClosingEventListener
    {
    public:
        enum EntityType : uint32_t
        {
            Player = 0,
            Bullet,
            Enemy,
            Explosion
        };

        Galaga();

    private:
        std::unique_ptr<ShapeRenderingSystem> _shape_rendering_system = nullptr;
        std::unique_ptr<MovementSystem> _movement_system = nullptr;
        std::unique_ptr<CollisionSystem> _collision_system = nullptr;
        std::unique_ptr<PlayerSystem> _player_system = nullptr;
        std::unique_ptr<EnemyControllerSystem> _enemy_controller_system = nullptr;
        std::unique_ptr<LifetimeSystem> _lifetime_system = nullptr;
        std::unique_ptr<EnemySpawner> _enemy_spawning_system = nullptr
            ;
        void on_app_closing() override;
    };
}
