#pragma once
#include <Mage/Mage.h>
#include "ShapeRenderingSystem.h"
#include "MovementSystem.h"
#include "StarfighterSystem.h"
#include "EnemyControllerSystem.h"
#include "LifetimeSystem.h"

namespace Galaga
{
    class Galaga :
            public Mage::Application,
            public Mage::OnAppClosingEventListener
    {
    public:
        Galaga();

    private:
        std::unique_ptr<ShapeRenderingSystem> _shape_rendering_system = nullptr;
        std::unique_ptr<MovementSystem> _movement_system = nullptr;
        std::unique_ptr<StarfighterSystem> _starfighter_system = nullptr;
        std::unique_ptr<EnemyControllerSystem> _enemy_controller_system = nullptr;
        std::unique_ptr<LifetimeSystem> _lifetime_system = nullptr;

        void on_app_closing() override;
    };
}
