#pragma once

#pragma warning(disable : 4100)

#include <Mage/Mage.h>
#include <random>
#include "GravitySystem.h"
#include "MovementSystem.h"
#include "ShapeRenderingSystem.h"
#include "PlayerSystem.h"
#include "LifetimeSystem.h"
#include "DeathByYSystem.h"
#include "CollisionSystem.h"
#include "DestructionNotificationSystem.h"
#include "EnemySpawningSystem.h"
#include "SpriteRenderingSystem.h"

class Game : public Mage::Application
{
public:
    Game();
};