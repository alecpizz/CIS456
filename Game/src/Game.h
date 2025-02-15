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

class Game : public Mage::Application
{
public:
    Game();
private:
    std::unique_ptr<GravitySystem> _gravity_system = nullptr;
    std::unique_ptr<ShapeRenderingSystem> _shape_rendering_system = nullptr;
    std::unique_ptr<MovementSystem> _movement_system = nullptr;
    std::unique_ptr<TorqueSystem> _torque_system = nullptr;
    RandomWrapper _rands;
    void add_random_shape();
};